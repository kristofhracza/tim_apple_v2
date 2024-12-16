#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "../dependencies/imgui/imgui.h"
#include "../dependencies/imgui/imgui_impl_dx11.h"
#include "../dependencies/imgui/imgui_impl_win32.h"



extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace GUI {
	inline HWND hWindow = nullptr;
	inline WNDPROC pOldWndProc = nullptr;

	inline bool IsActive = false;

	inline ImFont* ESPText;
	inline ImFont* ESPTextSmall;
	inline ImFont* ESPTextBold;
	inline ImFont* WeaponIcons;

	LRESULT GuiWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool Setup(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Destroy();

	void Render();
}