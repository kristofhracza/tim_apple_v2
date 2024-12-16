#pragma once

#include <d3d11.h>
#include <dxgi1_2.h>

#include <cstdint>

#include "detour.hpp"

#include "../utilities/Vectors.h"

#include "../sdk/interfaces/IViewRender.hpp"



namespace VTABLE{
	namespace D3D{
		enum{
			PRESENT = 8,
			RESIZEBUFFERS = 13,
			RESIZEBUFFERS_CSTYLE = 39,
		};
	}

	namespace DXGI{
		enum{
			CREATESWAPCHAIN = 10,
		};
	}

	namespace CLIENT{
		enum{
			CREATEMOVE = 21,
			VALIDATEINPUT = 7,
			MOUSEINPUTENABLED = 16,
			FRAMESTAGENOTIFY = 36,
		};
	}

	namespace INPUTSYSTEM{
		enum{
			ISRELATIVEMOUSEMODE = 76,
		};
	}
}

class CRenderGameSystem;
class CCSGOInput;
class CViewSetup;
class CMeshData;
class CUserCmd;

namespace Hooks {
	void Setup();
	void Destroy();

	HRESULT WINAPI Present(IDXGISwapChain* pSwapChain, UINT uSyncInterval, UINT uFlags);
	HRESULT __fastcall ResizeBuffers(IDXGISwapChain* pSwapChain, std::uint32_t nBufferCount, std::uint32_t nWidth, std::uint32_t nHeight, DXGI_FORMAT newFormat, std::uint32_t nFlags);
	HRESULT WINAPI CreateSwapChain(IDXGIFactory* pFactory, IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain);
	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	ViewMatrix_t* __fastcall GetMatrixForView(CRenderGameSystem* pRenderGameSystem, IViewRender* pViewRender, ViewMatrix_t* pOutWorldToView, ViewMatrix_t* pOutViewToProjection, ViewMatrix_t* pOutWorldToProjection, ViewMatrix_t* pOutWorldToPixels);
	bool __fastcall CreateMove(CCSGOInput* pInput, int nSlot, CUserCmd* pCmd);
	void __fastcall ValidateInput(CCSGOInput* pInput, int unk);
	bool __fastcall MouseInputEnabled(void* pThisptr);
	void __fastcall FrameStageNotify(void* rcx, int nFrameStage);
	//__int64* __fastcall LevelInit(void* pClientModeShared, const char* szNewMap);
	//__int64 __fastcall LevelShutdown(void* pClientModeShared);
	void __fastcall OverrideView(void* pClientModeCSNormal, CViewSetup* pSetup);
	void __fastcall DrawObject(void* pAnimatableSceneObjectDesc, void* pDx11, CMeshData* arrMeshDraw, int nDataCount, void* pSceneView, void* pSceneLayer, void* pUnk, void* pUnk2);
	void* IsRelativeMouseMode(void* pThisptr, bool bActive);
	void* __fastcall DrawArraySmokeVolume(void* pRCx, void* pThisptr1, void* pThisptr2, void* pThisptr3, void* pThisptr4, void* pThisptr5);


	inline CBaseHookObject<decltype(&Present)> hkPresent = {};
	inline CBaseHookObject<decltype(&ResizeBuffers)> hkResizeBuffers = {};
	inline CBaseHookObject<decltype(&CreateSwapChain)> hkCreateSwapChain = {};
	inline CBaseHookObject<decltype(&WndProc)> hkWndProc = {};

	inline CBaseHookObject<decltype(&GetMatrixForView)> hkGetMatrixForView = {};
	inline CBaseHookObject<decltype(&CreateMove)> hkCreateMove = {};
	inline CBaseHookObject<decltype(&ValidateInput)> hkValidateInput = {};
	inline CBaseHookObject<decltype(&MouseInputEnabled)> hkMouseInputEnabled = {};
	inline CBaseHookObject<decltype(&IsRelativeMouseMode)> hkIsRelativeMouseMode = {};
	inline CBaseHookObject<decltype(&FrameStageNotify)> hkFrameStageNotify = {};
	//inline CBaseHookObject<decltype(&LevelInit)> hkLevelInit = {};
	//inline CBaseHookObject<decltype(&LevelShutdown)> hkLevelShutdown = {};
	inline CBaseHookObject<decltype(&OverrideView)> hkOverrideView = {};
	inline CBaseHookObject<decltype(&DrawObject)> hkDrawObject = {};
	inline CBaseHookObject<decltype(&DrawArraySmokeVolume)> hkDrawArraySmokeVolume = {};
}