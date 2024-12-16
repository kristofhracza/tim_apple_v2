#include "Hooks.hpp"

#include "../entry/Config.hpp"

#include "../../utilities/Virtual.hpp"
#include "../../utilities/Scanner.hpp"
#include "../../utilities/Vectors.h"
#include "../../utilities/Logger.hpp"

#include "../../sdk/Attributes.hpp"
#include "../../sdk/interfaces/Interfaces.hpp"

#include "../../gui/Gui.hpp"
#include "../../gui/Menu.hpp"

#include "../../entry/SharedData.hpp"

#include "../../entry/features/Aim.hpp"
#include "../../entry/features/Movement.hpp"
#include "../../entry/features/Visuals.hpp"

#include "../utilities/Logger.hpp"




void Hooks::Setup() {
	Logger::Write("Hooks", "Setup");
	MH_Initialize();

	hkPresent.Create(Virtual::Get(Interfaces::SwapChain->pDXGISwapChain, VTABLE::D3D::PRESENT), reinterpret_cast<void*>(&Present));
	Logger::Write("Hooks", "Present");

	hkResizeBuffers.Create(Virtual::Get(Interfaces::SwapChain->pDXGISwapChain, VTABLE::D3D::RESIZEBUFFERS), reinterpret_cast<void*>(&ResizeBuffers));
	Logger::Write("Hooks", "ResizeBuffers");


	IDXGIDevice* pDXGIDevice = nullptr;
	Interfaces::Device->QueryInterface(IID_PPV_ARGS(&pDXGIDevice));
	IDXGIAdapter* pDXGIAdapter = nullptr;
	pDXGIDevice->GetAdapter(&pDXGIAdapter);
	IDXGIFactory* pIDXGIFactory = nullptr;
	pDXGIAdapter->GetParent(IID_PPV_ARGS(&pIDXGIFactory));

	hkCreateSwapChain.Create(Virtual::Get(pIDXGIFactory, VTABLE::DXGI::CREATESWAPCHAIN), reinterpret_cast<void*>(&CreateSwapChain));
	Logger::Write("Hooks", "CreateSwapChain");

	pDXGIDevice->Release();
	pDXGIDevice = nullptr;
	pDXGIAdapter->Release();
	pDXGIAdapter = nullptr;
	pIDXGIFactory->Release();
	pIDXGIFactory = nullptr;

	// IDA (CViewRender->OnRenderStart) and (FSA): #STR: "C:\\buildworker\\csgo_rel_win64\\build\\src\\game\\client\ 
	hkGetMatrixForView.Create(Scanner::PatternScan("client.dll", "40 53 48 81 EC ? ? ? ? 49 8B C1"), reinterpret_cast<void*>(&GetMatrixForView));
	Logger::Write("Hooks", "GetMatrixView");
	
	// IDA (FSA Plugin): #STR: "cl: CreateMove clamped invalid attack history %d index in , "cl: CreateMove
	hkCreateMove.Create(Scanner::PatternScan("client.dll", "48 8B C4 4C 89 40 ? 48 89 48 ? 55 53 56 57 48 8D A8"), reinterpret_cast<void*>(&CreateMove));
	Logger::Write("Hooks", "CreateMove");
	
	hkValidateInput.Create(Scanner::PatternScan("client.dll", "40 53 48 83 EC 20 48 8B D9 E8 ? ? ? ? C6 83 ? ? ? ? ? 33 C0"), reinterpret_cast<void*>(&ValidateInput));
	//hkValidateInput.Create(Virtual::Get(Interfaces::Input, VTABLE::CLIENT::VALIDATEINPUT), reinterpret_cast<void*>(&ValidateInput));
	Logger::Write("Hooks", "ValidateInput");
	
	hkMouseInputEnabled.Create(Virtual::Get(Interfaces::Input, VTABLE::CLIENT::MOUSEINPUTENABLED), reinterpret_cast<void*>(&MouseInputEnabled));
	Logger::Write("Hooks", "MouseInputEnabled");
	
	hkIsRelativeMouseMode.Create(Virtual::Get(Interfaces::InputSystem, VTABLE::INPUTSYSTEM::ISRELATIVEMOUSEMODE), reinterpret_cast<void*>(&IsRelativeMouseMode));
	Logger::Write("Hooks", "IsRelativeMouseMode");
	
	hkFrameStageNotify.Create(Virtual::Get(Interfaces::Source2Client, VTABLE::CLIENT::FRAMESTAGENOTIFY), reinterpret_cast<void*>(&FrameStageNotify));
	Logger::Write("Hooks", "FrameStageNotify");
	
	hkDrawObject.Create(Scanner::PatternScan("scenesystem.dll", "48 8B C4 53 41 54 41 55 48 81 EC ? ? ? ? 4D 63 E1"), reinterpret_cast<void*>(&DrawObject));
	Logger::Write("Hooks", "DrawObject");

	// IDA (FSA Plugin): ; #STR: "LockDynamicConstantBuffer failed in %s\n", "ParticleShadowBuffer", "SmokeVolumeTexA_Atlas", "SmokeVolumeLPV_Atlas", "SmokeLPV_Atlas", "Unable to render more than %d smokes.\n", "SmokeRenderer",
	hkDrawArraySmokeVolume.Create(Scanner::PatternScan("client.dll", "48 89 54 24 10 55 41 55 48"), reinterpret_cast<void*>(&DrawArraySmokeVolume));
	Logger::Write("Hooks", "DrawArraySmokeVolume");
}


void Hooks::Destroy() {
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);

	MH_Uninitialize();
}


HRESULT __stdcall Hooks::Present(IDXGISwapChain* pSwapChain, UINT uSyncInterval, UINT uFlags) {
	const auto oPresent = hkPresent.GetOriginal();
	Logger::Write("Hooks::Present", "GetOriginal");

	if (Interfaces::RenderTargetView == nullptr) {
		Interfaces::CreateRenderTarget(Interfaces::SwapChain->pDXGISwapChain);
		Logger::Write("Hooks::Present", "CreateRenderTarget");
	}
	if (Interfaces::RenderTargetView != nullptr) {
		Interfaces::DeviceContext->OMSetRenderTargets(1, &Interfaces::RenderTargetView, nullptr);
		Logger::Write("Hooks::Present", "OMSetRenderTargets");
	}
	// Rendering all ESP and menu
	GUI::Render();
	Logger::Write("Hooks::Present", "GUI::Render");

	// Smoke
	if (Config::VISUALS.IsSmokeColour) {
		VISUALS::SmokeColourMod();
		Logger::Write("Hooks::Present", "VISUALS::SmokeColourMod");
	}

	return oPresent(pSwapChain, uSyncInterval, uFlags); // Either use: Interfaces::SwapChain->pDXGISwapChain or pSwapChain argument -- Decide based on the amount of crashes lol
}


HRESULT __fastcall Hooks::ResizeBuffers(IDXGISwapChain* pSwapChain, std::uint32_t nBufferCount, std::uint32_t nWidth, std::uint32_t nHeight, DXGI_FORMAT newFormat, std::uint32_t nFlags) {
	const auto oResizeBuffers = hkResizeBuffers.GetOriginal();
	Logger::Write("Hooks::ResizeBuffers", "GetOriginal");

	if (Interfaces::RenderTargetView) Interfaces::DestroyRenderTarget();
	ImGui_ImplDX11_InvalidateDeviceObjects();

	auto hResult = oResizeBuffers(pSwapChain, nBufferCount, nWidth, nHeight, newFormat, nFlags);
	Logger::Write("Hooks::ResizeBuffers", "oResizeBuffers");

	if (SUCCEEDED(hResult)) {
		Interfaces::CreateRenderTarget(pSwapChain);
		ImGui_ImplDX11_CreateDeviceObjects();
		Logger::Write("Hooks::ResizeBuffers", "CreateRenderTarget");
	}

	return hResult;
}


HRESULT __stdcall Hooks::CreateSwapChain(IDXGIFactory* pFactory, IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain){
	const auto oCreateSwapChain = hkCreateSwapChain.GetOriginal();
	Logger::Write("Hooks::CreateSwapChain", "GetOriginal");

	Interfaces::DestroyRenderTarget();
	Logger::Write("Hooks::CreateSwapChain", "DestroyRenderTarget");

	return oCreateSwapChain(pFactory, pDevice, pDesc, ppSwapChain);
}


LRESULT Hooks::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	if (GUI::GuiWndProc(hWnd, uMsg, wParam, lParam)) {
		Logger::Write("Hooks::WndProc", "GuiWndProc");
		return 1L;
	};
	return CallWindowProc(GUI::pOldWndProc, hWnd, uMsg, wParam, lParam);
}



bool __fastcall Hooks::CreateMove(CCSGOInput* pInput, int nSlot, CUserCmd* pCmd){
	const auto oCreateMove = hkCreateMove.GetOriginal();
	Logger::Write("Hooks::CreateMove", "CreateMove");
	const bool bResult = oCreateMove(pInput, nSlot, pCmd);
	Logger::Write("Hooks::CreateMove", "oCreateMove");
	
	// Start the game loop here as we use functions from varying categories
	if (Interfaces::EngineClient->IsConnected() && Interfaces::EngineClient->IsInGame()) {

		// Local player
		CCSPlayerController* pLocalPlayerController = CCSPlayerController::GetLocalPlayerController();
		C_CSPlayerPawn* pLocalPlayerPawn = Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>(pLocalPlayerController->GetPawnHandle());
		if (!pLocalPlayerController->IsPawnAlive()) return bResult;

		CBaseUserCmdPB* pBaseCmd = pCmd->csgoUserCmd.pBaseCmd;

		// Calling IFs here looks ugly but we save some nano seconds
		if (Config::AIM.IsTrigger) {
			AIM::TriggerBot(pLocalPlayerPawn, pCmd, pBaseCmd, pInput);
			Logger::Write("Hooks::CreateMove", "AIM::TriggerBot");
		}
		
		if (Config::AIM.state) {
			AIM::Aimbot(pLocalPlayerPawn, pCmd, pBaseCmd, pInput);
			Logger::Write("Hooks::CreateMove", "AIM::Aimbot");
		}
		
		if (Config::AIM.IsRCS) { //  && !Config::AIM.state
			AIM::RCS(pLocalPlayerPawn, pCmd, pBaseCmd, pInput);
			Logger::Write("Hooks::CreateMove", "AIM::RCS");
		}
		
		if (Config::MOVEMENT.IsAA) {
			MOVEMENT::AntiAim(pCmd, pBaseCmd, pInput);
			Logger::Write("Hooks::CreateMove", "MOVEMENT::AntiAIm");
		}

		if (Config::MOVEMENT.IsBHOP) {
			MOVEMENT::BunnyHop(pLocalPlayerPawn,pCmd, pBaseCmd);
			Logger::Write("Hooks::CreateMove", "MOVEMENT::BunnyHop");
		}
	}
	
	return bResult;
}

void __fastcall Hooks::ValidateInput(CCSGOInput* pInput, int unk) {
	const auto oValidateInput = hkValidateInput.GetOriginal();
	Logger::Write("Hooks::ValidateInput", "GetOriginal");
	
	Vector3 vOriginalAngles = pInput->GetViewAngles();
	oValidateInput(pInput, unk);
	pInput->SetViewAngle(vOriginalAngles);

	return;
}

ViewMatrix_t* __fastcall Hooks::GetMatrixForView(CRenderGameSystem* pRenderGameSystem, IViewRender* pViewRender, ViewMatrix_t* pOutWorldToView, ViewMatrix_t* pOutViewToProjection, ViewMatrix_t* pOutWorldToProjection, ViewMatrix_t* pOutWorldToPixels) {
	const auto oGetMatrixForView = hkGetMatrixForView.GetOriginal();
	Logger::Write("Hooks::GetMatrixForView", "AIM::GetOriginal");

	ViewMatrix_t* matResult = oGetMatrixForView(pRenderGameSystem, pViewRender, pOutWorldToView, pOutViewToProjection, pOutWorldToProjection, pOutWorldToPixels);
	Logger::Write("Hooks::GetMatrixForView", "AIM::oGetMatrixForView");

	Shared::dwViewMatrix = *pOutWorldToProjection;

	return matResult;
}


void __fastcall Hooks::FrameStageNotify(void* rcx, int nFrameStage){
	const auto oFrameStageNotify = hkFrameStageNotify.GetOriginal();
	return oFrameStageNotify(rcx, nFrameStage);
}


void __fastcall Hooks::DrawObject(void* pAnimatableSceneObjectDesc, void* pDx11, CMeshData* arrMeshDraw, int nDataCount, void* pSceneView, void* pSceneLayer, void* pUnk, void* pUnk2) {
	const auto oDrawObject = hkDrawObject.GetOriginal();
	return oDrawObject(pAnimatableSceneObjectDesc,pDx11,arrMeshDraw, nDataCount,pSceneView,pSceneLayer,pUnk,pUnk2);
}


void* Hooks::IsRelativeMouseMode(void* pThisptr, bool bActive) {
	const auto oIsRelativeMouseMode = hkIsRelativeMouseMode.GetOriginal();
	return oIsRelativeMouseMode(pThisptr, bActive);
}


bool __fastcall Hooks::MouseInputEnabled(void* pThisptr) {
	const auto oMouseInputEnabled = hkMouseInputEnabled.GetOriginal();
	return Menu::IsOpen ? oMouseInputEnabled(Interfaces::Input) : oMouseInputEnabled(pThisptr);
}


void* __fastcall Hooks::DrawArraySmokeVolume(void* pRCx, void* pThisptr1, void* pThisptr2, void* pThisptr3, void* pThisptr4, void* pThisptr5) {
	const auto oDrawArraySmokeVolume = hkDrawArraySmokeVolume.GetOriginal();

	if (Config::VISUALS.IsSmoke) return nullptr;
	
	return oDrawArraySmokeVolume( pRCx,  pThisptr1,  pThisptr2,  pThisptr3,  pThisptr4,  pThisptr5);
}
