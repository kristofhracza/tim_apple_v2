#include "Gui.hpp"
#include "Menu.hpp"

#include "../hooks/Hooks.hpp"

#include "../sdk/interfaces/Interfaces.hpp"

#include "../entry/features/Esp.hpp"
#include "../entry/features/Misc.hpp"

#include "../entry/Config.hpp"

#include "../utilities/WeaponFont.hpp"



LRESULT  GUI::GuiWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_KEYDOWN && wParam == VK_INSERT) {
		Menu::IsOpen = !Menu::IsOpen;
		Hooks::IsRelativeMouseMode(Interfaces::InputSystem, !Menu::IsOpen);
	}
	return ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
}


bool GUI::Setup(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext) {
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = io.LogFilename = nullptr;
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange; // Only when in game -- TODO

	io.Fonts->AddFontDefault();
	Menu::MenuText = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf",14.f);
	Menu::MenuTextBold = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdanab.ttf", 16.f);
	Menu::MenuTextItalic = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdanai.ttf", 13.f);
	Menu::MenuTextButton = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 16.f);
	Menu::MenuTitle = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdanab.ttf", 22.f);

	GUI::ESPText = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahomabd.ttf", 14.f);
	GUI::ESPTextSmall = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahomabd.ttf", 12.f);

	// Do NOT remove the config flag
	// It allows imgui to take ownership of the memory and will not crash the program
	ImFontConfig FontConfig;
	FontConfig.FontDataOwnedByAtlas = false;
	GUI::WeaponIcons = io.Fonts->AddFontFromMemoryTTF(WeaponImages, sizeof(WeaponImages), 20.f, &FontConfig);

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(pDevice, pContext);

	return true;
}	


void GUI::Destroy() {
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}


void GUI::Render() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowSize(ImVec2(Menu::Width, Menu::Height));

	// Cheat DX11 features
	MISC::AntiFlash(); // Not checking for state here --  TODO: find better way to improve performance
	if (ImGui::GetBackgroundDrawList()) {
		if (Config::ESP.state) {
			ESP::MainLoop();
			Logger::Write("GUI::Render", "ESP::MainLoop");
		}
		{
			if (Config::AIM.FOVCircle && Config::AIM.state && Interfaces::EngineClient->IsConnected() && Interfaces::EngineClient->IsInGame()) {
				float fScreenMidX = ImGui::GetIO().DisplaySize.x / 2.f;
				float fScreenMidY = ImGui::GetIO().DisplaySize.y / 2.f;

				ImGui::GetBackgroundDrawList()->AddCircle(
					{ fScreenMidX ,fScreenMidY },
					(Config::AIM.FOV * 10),
					ImColor(Config::ESP.AttributeColours[0], Config::ESP.AttributeColours[1], Config::ESP.AttributeColours[2])
					, 0, 1.f
				);
			}
		}
	}

	Menu::Main();

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}