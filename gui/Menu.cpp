#include "Menu.hpp"

#include "../entry/Config.hpp"



void Menu::Spacing(int Spaces) {
	for (int i = 0; i < Spaces; i++) {
		ImGui::Spacing();
	}
}

void SideButton(int MenuIndex,std::string Name) {
	ImGui::PushFont(Menu::MenuTextButton);
	ImGui::PushStyleColor(ImGuiCol_Button, Menu::CurrentMenu == MenuIndex ? ImColor(0, 160, 255, 255).Value : ImColor(30, 30, 30, 255).Value);
	if (ImGui::Button(Name.c_str(), Menu::StandardButton)) Menu::CurrentMenu = MenuIndex;
	ImGui::PopStyleColor();
	ImGui::PopFont();
}

void VerticalSplitter(float fWidth, float fHeight) {
	ImGui::SameLine();
	ImGui::InvisibleButton("vsplitter", ImVec2(1.f, fHeight));
	ImGui::SameLine();
}

void HorizontalSplitter(float fHeight) {
	ImGui::InvisibleButton("hsplitter", ImVec2(-1, 8.0f));
	if (ImGui::IsItemActive())
		fHeight += ImGui::GetIO().MouseDelta.y;
}

void Menu::SetStyles() {
	ImGuiStyle* style = &ImGui::GetStyle();

	// Sizes
	style->FramePadding = ImVec2(5, 5);
	style->FrameBorderSize = 1.f;
	style->FrameRounding = 1.f;

	style->WindowPadding = ImVec2(6, 6);

	style->GrabRounding = 1.f;
	style->GrabMinSize = 20.f;

	style->ScrollbarSize = 5.f;

	style->ButtonTextAlign = ImVec2(0, 0.5);

	style->ItemSpacing = ImVec2(10, 5);

	// Colours
	ImColor Main = ImColor(0, 185, 255, 255);
	ImColor MainHover = ImColor(0, 185, 255, 200);
	ImColor MainLight = ImColor(0, 215, 255, 255);
	ImColor MainActive = ImColor(0, 160, 255, 255);
	ImColor DarkGrey = ImColor(30, 30, 30, 255);
	ImColor LightGrey = ImColor(60, 60, 60, 255);
	ImColor Black = ImColor(5, 5, 5, 255);
	ImColor BlackLight = ImColor(20, 20, 20, 255);
	ImColor Transparent = ImColor(0, 0, 0, 0);

	// Colour GUI
	style->Colors[ImGuiCol_FrameBg] = DarkGrey;
	style->Colors[ImGuiCol_FrameBgHovered] = DarkGrey;
	style->Colors[ImGuiCol_FrameBgActive] = DarkGrey;

	style->Colors[ImGuiCol_TitleBgActive] = Black;
	style->Colors[ImGuiCol_TitleBgCollapsed] = Black;

	style->Colors[ImGuiCol_ChildBg] = BlackLight;

	style->Colors[ImGuiCol_Border] = Transparent;

	style->Colors[ImGuiCol_MenuBarBg] = DarkGrey;
	style->Colors[ImGuiCol_WindowBg] = DarkGrey;

	style->Colors[ImGuiCol_CheckMark] = Main;

	style->Colors[ImGuiCol_Button] = DarkGrey;
	style->Colors[ImGuiCol_ButtonActive] = MainActive;
	style->Colors[ImGuiCol_ButtonHovered] = MainActive;

	style->Colors[ImGuiCol_Header] = DarkGrey;
	style->Colors[ImGuiCol_HeaderHovered] = LightGrey;
	style->Colors[ImGuiCol_HeaderActive] = LightGrey;

	style->Colors[ImGuiCol_SliderGrab] = Main;
	style->Colors[ImGuiCol_SliderGrabActive] = MainActive;

	style->Colors[ImGuiCol_Separator] = Transparent;
	style->Colors[ImGuiCol_SeparatorHovered] = Transparent;
	style->Colors[ImGuiCol_SeparatorActive] = Transparent;

	style->Colors[ImGuiCol_ResizeGrip] = Main;
	style->Colors[ImGuiCol_ResizeGripHovered] = MainHover;
	style->Colors[ImGuiCol_ResizeGripActive] = MainActive;

	style->Colors[ImGuiCol_HeaderHovered] = Main;
	style->Colors[ImGuiCol_HeaderActive] = MainActive;
}


void Menu::Main() {
	if (IsOpen) {
		ImGui::PushFont(MenuText);
		ImGui::SetNextWindowSize({ Width,Height }); // ImGuiCond_FirstUseEver
		ImGui::Begin("Tim Apple v2", &IsOpen, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);

		SetStyles();

		ImGui::Columns(2, "MAIN");
		ImGui::SetColumnOffset(1, Menu::StandardButton.x + ColumnsOffsetX + 20);

		Menu::Spacing(6);

		ImGui::SetCursorPosX(ColumnsOffsetX);
		ImGui::PushFont(MenuTitle);
		ImGui::Text("Tim Apple v2");
		ImGui::PopFont();

		Menu::Spacing(12);

		SideButton(0, "ESP");
		SideButton(1, "Aim");
		SideButton(2, "Visuals");
		SideButton(3, "Movement");
		SideButton(4, "Miscellaneous");
		SideButton(-1, "Settings");

		ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 30);
		ImGui::SetCursorPosX(ColumnsOffsetX);
		ImGui::Text("Version 1.0.1");

		ImGui::NextColumn();

		switch (Menu::CurrentMenu) {
		case 0:
			Menu::ESP();
			break;
		case 1:
			Menu::Aim();
			break;
		case 2:
			Menu::Visuals();
			break;
		case 3:
			Menu::Movement();
			break;
		case 4:
			Menu::Misc();
			break;
		case -1:
			Menu::Settings();
			break;
		}

		ImGui::Columns();

		ImGui::PopFont();

		ImGui::End();
	}
}


void Menu::ESP() {
	ImGui::BeginChild("#", ImVec2(WidthSeparator,HeightSeparator), true);
	
	ImGui::PushFont(MenuTextBold);
	ImGui::Text("ESP Details");
	Menu::Spacing(4);
	ImGui::PopFont();

	ImGui::Checkbox("ESP Enabled", &Config::ESP.State);
	Menu::Spacing(2);
	ImGui::Checkbox("Bounding Box", &Config::ESP.IsBoundingBox);
	Menu::Spacing(2);
	ImGui::Checkbox("Colour Fill", &Config::ESP.FilledBox);
	Menu::Spacing(2);
	ImGui::Checkbox("Health Bar", &Config::ESP.IsHealthBar);
	Menu::Spacing(2);
	ImGui::Checkbox("Skeleton", &Config::ESP.IsSkeleton);
	Menu::Spacing(2);
	ImGui::Checkbox("Joints", &Config::ESP.IsJoint);
	Menu::Spacing(2);
	ImGui::EndChild();
	
	VerticalSplitter(WidthSeparator, HeightSeparator);

	ImGui::BeginChild("##", ImVec2(0, HeightSeparator), true);
	ImGui::PushFont(MenuTextBold);
	ImGui::Text("Player Details");
	Menu::Spacing(4);
	ImGui::PopFont();

	ImGui::Checkbox("Player Names", &Config::ESP.IsPlayerName);
	Menu::Spacing(2);
	ImGui::Checkbox("Weapon Names", &Config::ESP.IsWeaponName);
	Menu::Spacing(2);
	ImGui::Checkbox("Distance", &Config::ESP.Distance);
	Menu::Spacing(2);
	ImGui::Checkbox("Snap Lines", &Config::ESP.SnapLines);
	
	Menu::Spacing(4);

	ImGui::PushFont(MenuTextBold);
	ImGui::Text("Dropped Items");
	Menu::Spacing(2);
	ImGui::PopFont();

	ImGui::Checkbox("Enable", &Config::ESP.IsGroundESP);
	Menu::Spacing(2);
	ImGui::EndChild();

	ImGui::BeginChild("###", ImVec2(0, 0), true);

	ImGui::PushFont(MenuTextBold);
	ImGui::Text("Colours and Attributes");
	Menu::Spacing(4);
	ImGui::PopFont();

	ImGui::SliderFloat("Box Width", &Config::ESP.BoxWidth, 2.f, 4.f);
	Menu::Spacing(2);
	ImGui::ColorEdit3("Skeleton", (float*)&Config::ESP.SkeletonColours);
	Menu::Spacing(2);
	ImGui::ColorEdit3("Joint", (float*)&Config::ESP.JointColours);
	Menu::Spacing(2);
	ImGui::ColorEdit3("Attributes", (float*)&Config::ESP.AttributeColours);
	Menu::Spacing(2);
	ImGui::ColorEdit3("Filled Box", (float*)&Config::ESP.FilledBoxColours);
	Menu::Spacing(2);
	ImGui::SliderFloat("Filled Box Alpha", &Config::ESP.FilledBoxAlpha, 0.f, 1.f);
	Menu::Spacing(2);

	ImGui::EndChild();
}


void Menu::Aim() {
	ImGui::BeginChild("Aim", ImVec2(0, HeightSeparator + 20.f), true);

	ImGui::PushFont(MenuTextBold);
	ImGui::Text("Features");
	Menu::Spacing(4);
	ImGui::PopFont();

	ImGui::Checkbox("Aim Enabled", &Config::AIM.State);
	Menu::Spacing(2);
	ImGui::Checkbox("Silent Aim", &Config::AIM.Silent);
	Menu::Spacing(2);
	ImGui::Checkbox("Spotted Check", &Config::AIM.Spotted);
	Menu::Spacing(2);
	ImGui::Checkbox("FOV Circle", &Config::AIM.FOVCircle);
	Menu::Spacing(2);
	ImGui::SliderFloat("FOV", &Config::AIM.FOV, 1.f, 25.f);
	Menu::Spacing(2);
	ImGui::SliderFloat("Smoothing", &Config::AIM.Smoothing, 1.f, 15.f);
	Menu::Spacing(2);
	if (ImGui::BeginCombo("Hit Boxes", Config::AIM.Bones[Config::AIM.BoneSelect].c_str())) {
		for (int i = 0; i < Config::AIM.Bones.size(); ++i) {
			const bool isSelected = (Config::AIM.Bone == i);

			if (ImGui::Selectable(Config::AIM.Bones[i].c_str(), isSelected)) Config::AIM.BoneSelect = i;
			if (isSelected) ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::EndChild();

	ImGui::BeginChild("##", ImVec2(0, 0), true);

	ImGui::PushFont(MenuTextBold);
	ImGui::Text("Miscallenous");
	Menu::Spacing(4);
	ImGui::PopFont();

	ImGui::Checkbox("Recoil Control", &Config::AIM.IsRCS);
	Menu::Spacing(2);
	ImGui::Checkbox("Trigger Bot", &Config::AIM.IsTrigger);
	Menu::Spacing(2);

	ImGui::EndChild();
}

void Menu::Movement() {
	ImGui::BeginChild("Anti Aim", ImVec2(0, HeightSeparator), true);

	ImGui::PushFont(MenuTextBold);
	ImGui::Text("Anti Aim");
	Menu::Spacing(4);
	ImGui::PopFont();

	ImGui::Checkbox("Enabled", &Config::MOVEMENT.IsAA);
	Menu::Spacing(2);
	ImGui::Checkbox("Jitter", &Config::MOVEMENT.Jitter);
	Menu::Spacing(2);
	ImGui::SliderFloat("Yaw", &Config::MOVEMENT.Yaw, -180.f, 180.f);
	Menu::Spacing(2);
	ImGui::SliderFloat("Pitch", &Config::MOVEMENT.Pitch, -89.f, 89.f);
	Menu::Spacing(2);

	ImGui::EndChild();

	ImGui::BeginChild("Miscellaneous", ImVec2(0, 0), true);

	ImGui::PushFont(MenuTextBold);
	ImGui::Text("Miscellaneous");
	Menu::Spacing(4);
	ImGui::PopFont();

	ImGui::Checkbox("Bunny Hop", &Config::MOVEMENT.IsBHOP);
	Menu::Spacing(2);

	ImGui::EndChild();

}

void Menu::Visuals() {
	ImGui::BeginChild("Map", ImVec2(0, 0), true);

	ImGui::PushFont(MenuTextBold);
	ImGui::Text("Map Visuals");
	Menu::Spacing(4);
	ImGui::PopFont();

	ImGui::Checkbox("Invisible Smoke", &Config::VISUALS.IsSmoke);
	Menu::Spacing(2);
	ImGui::Checkbox("Coloured Smoke", &Config::VISUALS.IsSmokeColour);
	Menu::Spacing(2);
	ImGui::ColorEdit3("Smoke Colours", (float*)&Config::VISUALS.SmokeColours);
	Menu::Spacing(2);

	ImGui::EndChild();
}

void Menu::Misc() {
	ImGui::BeginChild("Miscellaneous", ImVec2(0, 0), true);
	ImGui::PushFont(MenuTextBold);
	ImGui::Text("Visuals");
	Menu::Spacing(4);
	ImGui::PopFont();

	ImGui::Checkbox("Anti Flash", &Config::MISC.IsFlashed);
	Menu::Spacing(2);

	ImGui::EndChild();
}

void Menu::Settings() {
	ImGui::BeginChild("Settings", ImVec2(0, 0), true);

	ImGui::PushFont(MenuTextBold);
	ImGui::Text("Main Settings");
	Menu::Spacing(4);
	ImGui::PopFont();

	ImGui::Checkbox("Debug Log", &Config::SETTINGS.IsDebug);
	ImGui::PushFont(MenuTextItalic);
	ImGui::Text("This will open a console!");
	ImGui::PopFont();
	Menu::Spacing(2);

	ImGui::EndChild();
}