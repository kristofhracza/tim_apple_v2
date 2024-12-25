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
    Logger::Log("SetInterfaces - Source2Client");

    // IDA (FSA Plugin): ; #STR: "BeginDebuggerInspect", "CloseDebuggerWindow", "SystemInputLanguageChanged", "CSGOInput"  AND  mov     rcx, cs:qword_181B14A68
    Input = *reinterpret_cast<CCSGOInput**>(Scanner::ResolveRelativeAddress(Scanner::PatternScan("client.dll", "48 8B 0D ? ? ? ? 4C 8D 8F ? ? ? ? 45 33 FF"), 0x3, 0x7));
    Logger::Log("SetInterfaces - Input");

    GlobalVars = *reinterpret_cast<IGlobalVars**>(Scanner::ResolveRelativeAddress(Scanner::PatternScan("client.dll", "48 8B 05 ? ? ? ? 8B 48 04 FF C1"), 0x3, 0x7));
    Logger::Log("SetInterfaces - GlobalVars");

    GameTraceManager = *reinterpret_cast<CGameTraceManager**>(Scanner::GetAbsoluteAddress(Scanner::PatternScan("client.dll", "48 8B 0D ? ? ? ? 4C 8B C3 66 89 44 24"), 0x3, 0x0));
    Logger::Log("SetInterfaces - GameTraceManager");

    SchemaSystem = GetInterface<CSchemaSystem>(SCHEMA_SYSTEM, "schemasystem.dll");
    Logger::Log("SetInterfaces - SchemaSystem");

    InputSystem = GetInterface<IInputSystem>(INPUT_SYSTEM_VERSION, "inputsystem.dll");
    Logger::Log("SetInterfaces - InputSystem");

    NetworkClientService = GetInterface<INetworkClientService>(NETWORK_CLIENT_SERVICE, "engine2.dll");
    Logger::Log("SetInterfaces - NetworkClientService");

    EngineClient = GetInterface<IEngineClient>(SOURCE2_ENGINE_TO_CLIENT, "engine2.dll");
    Logger::Log("SetInterfaces - EngineClient");

    GameResourceService = GetInterface<IGameResourceService>(GAME_RESOURCE_SERVICE_CLIENT, "engine2.dll");
    Logger::Log("SetInterfaces - GameResourceService");

    EngineCVar = GetInterface<IEngineCVar>(ENGINE_CVAR, "tier0.dll");
    Logger::Log("SetInterfaces - EngineCVar");

    MaterialSystem2 = GetInterface<IMaterialSystem2>(MATERIAL_SYSTEM2, "materialsystem2.dll");
    Logger::Log("SetInterfaces - MaterialSystem2");

    ResourceSystem = GetInterface<IResourceSystem>(RESOURCE_SYSTEM, "resourcesystem.dll");
    Logger::Log("SetInterfaces - ResourceSystem");

    if (ResourceSystem != nullptr) ResourceHandleUtils = reinterpret_cast<CResourceHandleUtils*>(ResourceSystem->QueryInterface(RESOURCE_HANDLE_UTILS));
    Logger::Log("SetInterfaces - ResourceHandleUtils");

    SwapChain = **reinterpret_cast<ISwapChainDx11***>(Scanner::ResolveRelativeAddress(Scanner::PatternScan("rendersystemdx11.dll", "66 0F 7F 0D ?? ?? ?? ?? 48 8B F7 66 0F 7F 05"), 0x4, 0x8));
    Logger::Log("SetInterfaces - SwapChain");
    if (SwapChain != nullptr) {
        Logger::Log("SetInterfaces - SwapChain->GetDevice");
        if (SUCCEEDED(SwapChain->pDXGISwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&Device))) {
            Device->GetImmediateContext(&DeviceContext);
            Logger::Log("SetInterfaces - SwapChain->GetImmediateContext");
        }
        else {
            Logger::Log("SetInterfaces -  SwapChain->GetDevice", -1);
            return;
        };
    }
}


// FOR ME this is confusing af
// Here we setup everything needed for DX to work
// So, if any DX related issues are encountered this might be to blame
int Interfaces::CreateRenderTarget(IDXGISwapChain* pSwapChain) {
    if (FAILED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&Device)))
        Logger::Log("CreateRenderTarget - pSwapChain->GetDevice",-1);
    
    Device->GetImmediateContext(&DeviceContext);
    DXGI_SWAP_CHAIN_DESC pDesc;
    pSwapChain->GetDesc(&pDesc);
    
    ID3D11Texture2D* pBackBuffer = nullptr;
    
    if (SUCCEEDED(pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)))) {
        if (pBackBuffer) {

            D3D11_RENDER_TARGET_VIEW_DESC desc{};
            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

            Device->CreateRenderTargetView(pBackBuffer, &desc, &RenderTargetView);

            pBackBuffer->Release();
            pBackBuffer = nullptr;
            
            GUI::hWindow = pDesc.OutputWindow;
            GUI::pOldWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(GUI::hWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Hooks::WndProc)));
            
            GUI::Setup(GUI::hWindow, Device, DeviceContext);
        }
    }
    return 0;
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
