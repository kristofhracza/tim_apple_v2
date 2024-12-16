#include "Interfaces.hpp"

#include "../../hooks/Hooks.hpp"

#include "../../gui/Gui.hpp"
#include "../../gui/Gui.hpp"
#include "../../utilities/Logger.hpp"



template <typename T>
T* InterfaceFunctions::GetInterface(const char* interfaceName, const char* dll) {
	tCreateInterface CreateInterface = reinterpret_cast<tCreateInterface>(GetProcAddress(GetModuleHandle(dll), "CreateInterface"));
	int retCode = 0;
	void* interFace = CreateInterface(interfaceName, &retCode);
	return reinterpret_cast<T*>(interFace);
};

void InterfaceFunctions::SetInterfaces() {
	using namespace Interfaces;

	Source2Client = GetInterface<ISource2Client>(SOURCE2_CLIENT, "client.dll");
    Logger::Write("SetInterfaces", "Source2Client");
	
    // IDA (FSA Plugin): ; #STR: "BeginDebuggerInspect", "CloseDebuggerWindow", "SystemInputLanguageChanged", "CSGOInput"  AND  mov     rcx, cs:qword_181B14A68
    Input = *reinterpret_cast<CCSGOInput**>(Scanner::ResolveRelativeAddress(Scanner::PatternScan("client.dll", "48 8B 0D ? ? ? ? 4C 8D 8F ? ? ? ? 45 33 FF"), 0x3, 0x7));
    Logger::Write("SetInterfaces", "Input");

	GlobalVars = *reinterpret_cast<IGlobalVars**>(Scanner::ResolveRelativeAddress(Scanner::PatternScan("client.dll", "48 8B 05 ? ? ? ? 8B 48 04 FF C1"), 0x3, 0x7));
    Logger::Write("SetInterfaces", "GlobalVars");

	GameTraceManager = *reinterpret_cast<CGameTraceManager**>(Scanner::GetAbsoluteAddress(Scanner::PatternScan("client.dll", "48 8B 0D ? ? ? ? 4C 8B C3 66 89 44 24"), 0x3, 0x0));
    Logger::Write("SetInterfaces", "GameTraceManager");

	SchemaSystem = GetInterface<CSchemaSystem>(SCHEMA_SYSTEM, "schemasystem.dll");
    Logger::Write("SetInterfaces", "SchemaSystem");

	InputSystem = GetInterface<IInputSystem>(INPUT_SYSTEM_VERSION, "inputsystem.dll");
    Logger::Write("SetInterfaces", "InputSystem");

	NetworkClientService = GetInterface<INetworkClientService>(NETWORK_CLIENT_SERVICE, "engine2.dll");
    Logger::Write("SetInterfaces", "NetworkClientService");

	EngineClient = GetInterface<IEngineClient>(SOURCE2_ENGINE_TO_CLIENT, "engine2.dll");
    Logger::Write("SetInterfaces", "EngineClient");

	GameResourceService = GetInterface<IGameResourceService>(GAME_RESOURCE_SERVICE_CLIENT, "engine2.dll");
    Logger::Write("SetInterfaces", "GameResourceService");

	EngineCVar = GetInterface<IEngineCVar>(ENGINE_CVAR,"tier0.dll");
    Logger::Write("SetInterfaces", "EngineCVar");

	MaterialSystem2 = GetInterface<IMaterialSystem2>(MATERIAL_SYSTEM2, "materialsystem2.dll");
    Logger::Write("SetInterfaces", "MaterialSystem2");

	ResourceSystem = GetInterface<IResourceSystem>(RESOURCE_SYSTEM, "resourcesystem.dll");
    Logger::Write("SetInterfaces", "ResourceSystem");

	if (ResourceSystem != nullptr) ResourceHandleUtils = reinterpret_cast<CResourceHandleUtils*>(ResourceSystem->QueryInterface(RESOURCE_HANDLE_UTILS));
    Logger::Write("SetInterfaces", "ResourceHandleUtils");

	SwapChain = **reinterpret_cast<ISwapChainDx11***>(Scanner::ResolveRelativeAddress(Scanner::PatternScan("rendersystemdx11.dll", "66 0F 7F 0D ?? ?? ?? ?? 48 8B F7 66 0F 7F 05"), 0x4, 0x8));
    Logger::Write("SetInterfaces", "SwapChain");
    if (SwapChain != nullptr){
        if (SUCCEEDED(SwapChain->pDXGISwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&Device))) {
            Logger::Write("SetInterfaces - SwapChain", "GetDevice");
            Device->GetImmediateContext(&DeviceContext);
            Logger::Write("SetInterfaces - SwapChain", "GetImmediateContext");
        }
        else return;
    }
}

void Interfaces::CreateRenderTarget(IDXGISwapChain* pSwapChain) {
    DestroyRenderTarget();
    Logger::Write("CreateRenderTarget", "DestroyRenderTarget");

    // Get device
    if (FAILED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&Device))) {
        return;
    }
    Device->GetImmediateContext(&DeviceContext);
    Logger::Write("CreateRenderTarget", "GetImmediateContext");


    // Swapchain description
    DXGI_SWAP_CHAIN_DESC sd;
    pSwapChain->GetDesc(&sd);

    static const auto GetCorrectDXGIFormat = [](DXGI_FORMAT eCurrentFormat){
        switch (eCurrentFormat)
        {
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
            return DXGI_FORMAT_R8G8B8A8_UNORM;
        }

        return eCurrentFormat;
    };

    ID3D11Texture2D* pBackBuffer = nullptr;
    // Get RendertTargetView
    if (SUCCEEDED(pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)))) {
        Logger::Write("CreateRenderTarget", "GetBuffer");
        if (pBackBuffer) {
            D3D11_RENDER_TARGET_VIEW_DESC desc{};
            desc.Format = static_cast<DXGI_FORMAT>(GetCorrectDXGIFormat(sd.BufferDesc.Format));
            desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

            HRESULT pRenderTarget = Device->CreateRenderTargetView(pBackBuffer, &desc, &RenderTargetView);
            Logger::Write("CreateRenderTarget", "CreateRenderTargetView (1)");
            if (FAILED(pRenderTarget)) {
                desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS; // Retry with multisampled dimension
                pRenderTarget = Device->CreateRenderTargetView(pBackBuffer, &desc, &RenderTargetView);
                Logger::Write("CreateRenderTarget", "CreateRenderTargetView (2)");
                if (FAILED(pRenderTarget)) {
                    pRenderTarget = Device->CreateRenderTargetView(pBackBuffer, nullptr, &RenderTargetView); // Fallback
                    Logger::Write("CreateRenderTarget", "CreateRenderTargetView (3)");
                    if (FAILED(pRenderTarget)) {
                        pBackBuffer->Release();
                        return;
                    }
                }
            }

            pBackBuffer->Release();
            GUI::hWindow = sd.OutputWindow; 
            
            if (!GUI::pOldWndProc) {
                GUI::pOldWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(GUI::hWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Hooks::WndProc)));
            }
            GUI::Setup(GUI::hWindow, Device, DeviceContext);
            Logger::Write("CreateRenderTarget", " GUI::Setup");
        }
    }
}


void Interfaces::DestroyRenderTarget(){
	if (RenderTargetView != nullptr){
		RenderTargetView->Release();
		RenderTargetView = nullptr;
	}
}

void Interfaces::DestroyDeviceContext() {
	if (DeviceContext != nullptr) {
		DeviceContext->Release();
		DeviceContext = nullptr;
	}
}

void Interfaces::DestroyDevice() {
	if (Device != nullptr) {
		Device->Release();
		Device = nullptr;
	}
}
