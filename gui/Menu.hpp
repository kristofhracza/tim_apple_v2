#pragma once

#include "../dependencies/imgui/imgui.h"

namespace Menu {
	inline bool IsOpen = false;

	inline float Width = 750;
	inline float Height = 600;

	const float ColumnsOffsetX = 20.f;
	const float ButtonWidth = 140.f;
	const float ButtonHeight = 40.f;

	inline float WidthSeparator = Width / 2.f - ButtonWidth;
	inline float HeightSeparator = Height / 2.f;

	inline ImVec2 StandardButton = ImVec2(ButtonWidth, ButtonHeight);
	inline float ChildSize = Width;

	inline int CurrentMenu = 0;

	inline ImFont* MenuText;
	inline ImFont* MenuTitle;
	inline ImFont* MenuTextBold;
	inline ImFont* MenuTextButton;
	inline ImFont* MenuTextItalic;

	void SetStyles();

	void Spacing(int Spaces);

	void Main();
	void ESP();
	void Aim();
	void Visuals();
	void Movement();
	void Misc();

	void Settings();
}