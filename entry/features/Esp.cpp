#include <format>
#include <algorithm>
#include <map>

#include "Esp.hpp"

#include "../Config.hpp"
#include "../SharedData.hpp"

#include "../../dependencies/imgui/imgui.h"

#include "../../sdk//interfaces/Interfaces.hpp"
#include "../../sdk/Attributes.hpp"

#include "../../utilities/Helpers.hpp"

#include "../../gui/Gui.hpp"



void ESP::MainLoop() {
	if (Interfaces::EngineClient->IsConnected() && Interfaces::EngineClient->IsInGame()) {

		CCSPlayerController* pLocalPlayerController = CCSPlayerController::GetLocalPlayerController();
		if (pLocalPlayerController == nullptr) return;
		C_CSPlayerPawn* pLocalPlayerPawn = Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>(pLocalPlayerController->GetPawnHandle());
		if (pLocalPlayerPawn == nullptr) return;

		// Entity loop
		for (int i = 1; i <= Interfaces::GameResourceService->m_pGameEntitySystem->GetHighestEntityIndex(); i++) {

			C_BaseEntity* pBaseEntity = Interfaces::GameResourceService->m_pGameEntitySystem->Get(i);
			if (pBaseEntity == nullptr) continue;

			// Get class info
			SchemaClassInfoData_t* pClassInfo = nullptr;
			pBaseEntity->GetSchemaClassInfo(&pClassInfo);
			if (pClassInfo == nullptr) continue;
	
			// Dropped Item ESP
			if (pBaseEntity->IsWeapon() && Config::ESP.IsGroundESP) {
				if (pBaseEntity->GetOwnerHandle().IsValid() == 0) {
					Vector3 vOriginAddy = pBaseEntity->GetSceneOrigin();
					if (vOriginAddy.IsZero()) continue;
					Vector3 vOrigin = vOriginAddy.WorldToScreen(Shared::dwViewMatrix);
					if (vOrigin.z >= 0.01f)  WeaponName(pClassInfo->m_szName, vOrigin, true);
				}
			}

			// Player related
			if (std::string(pClassInfo->m_szName) != std::string("CCSPlayerController")) continue;

			CCSPlayerController* pPlayerController = reinterpret_cast<CCSPlayerController*>(pBaseEntity);
			if (pPlayerController == nullptr) continue;

			C_CSPlayerPawn* pPlayerPawn = Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>(pPlayerController->GetPawnHandle());
			if (pPlayerPawn == nullptr) continue;

			// Checks
			if (pPlayerPawn->GetHealth() > 100 || pPlayerPawn->GetHealth() <= 0) continue;
			if (pLocalPlayerPawn->GetTeam() == pPlayerPawn->GetTeam()) continue;
			if (pPlayerPawn->GetGameSceneNode() == nullptr) continue;

			// Calculating shared values
			{
				if (pPlayerPawn->GetOrigin().IsZero()) continue;
				Shared::vOrigin = pPlayerPawn->GetOrigin();

				Vector3 vOriginScreen = Shared::vOrigin.WorldToScreen(Shared::dwViewMatrix);
				Shared::vOriginScreen = vOriginScreen;

				Vector3 vHeadPos = { Shared::vOrigin.x,Shared::vOrigin.y,Shared::vOrigin.z + 75.f };
				Vector3 vHeadPosScreen = vHeadPos.WorldToScreen(Shared::dwViewMatrix);
				Shared::vHeadPos = vHeadPos;
				Shared::vHeadPosScreen = vHeadPosScreen;

				Shared::BoxWidth = (vOriginScreen.y - vHeadPosScreen.y) * Config::ESP.BoxWidth / 10;
				Shared::BoxHeight = vOriginScreen.y - vHeadPosScreen.y;

				Shared::Distance = static_cast<int>(Helpers::GetDistance(pLocalPlayerPawn->GetOrigin(), Shared::vOrigin)) / 100;
			}

			// Render
			// Need to check for z value otherwise the render will "mirror" -- fucking stupid but works, don't ask why
			if (Shared::vOriginScreen.z >= 0.01f) {
				if (Config::ESP.IsBoundingBox) BoundingBox(pPlayerPawn->GetOrigin(), Shared::dwViewMatrix);
				if (Config::ESP.IsHealthBar) HealthBar(pPlayerPawn->GetHealth());
				if (Config::ESP.IsPlayerName) PlayerName(pPlayerController->GetPlayerName());
				if (Config::ESP.IsSkeleton) Skeleton(pPlayerPawn->GetGameSceneNode()->GetSkeletonInstance()->pBoneCache);
				if (Config::ESP.SnapLines) SnapLines();
				if (Config::ESP.Distance) Distance();
				if (Config::ESP.IsWeaponName) {
					CPlayer_WeaponServices* pWeaponServices = pPlayerPawn->GetWeaponServices();
					if (pWeaponServices == nullptr) continue;

					C_BaseEntity* pWeaponBaseEntity =  Interfaces::GameResourceService->m_pGameEntitySystem->Get(pWeaponServices->GetActiveWeapon().GetEntryIndex());
					if (pWeaponBaseEntity == nullptr) continue;
					
					SchemaClassInfoData_t* pClassInfo = nullptr;
					pWeaponBaseEntity->GetSchemaClassInfo(&pClassInfo);
					if (pClassInfo == nullptr) continue;

					WeaponName(pClassInfo->m_szName, Shared::vHeadPosScreen);
				}
			}
		}
	}
}


void ESP::HealthBar(int32_t iHealth) {
	int iHealthBarYOff = (static_cast<float>(Shared::BoxHeight * iHealth * 0.01f));

	float fRed = (255.f - (iHealth * 2.55f)) - 100.f;
	float fGreen = (iHealth * 2.55f) / 100.f;
	
	ImGui::GetBackgroundDrawList()->AddRectFilled(
		{ Shared::vHeadPosScreen.x - (Shared::BoxWidth + 2.f), Shared::vOriginScreen.y - Shared::BoxHeight }, 
		{ Shared::vHeadPosScreen.x - (Shared::BoxWidth + 4.5f), Shared::vOriginScreen.y }, 
		ImColor(0.f, 0.f, 0.f, 0.3f)
	);
	ImGui::GetBackgroundDrawList()->AddRectFilled(
		{ Shared::vHeadPosScreen.x - (Shared::BoxWidth + 2.f), Shared::vOriginScreen.y - iHealthBarYOff }, 
		{ Shared::vHeadPosScreen.x - (Shared::BoxWidth + 4.5f), Shared::vOriginScreen.y }, 
		ImColor(fRed, fGreen, 0.f, 1.f)
	);
}


void ESP::BoundingBox(Vector3 vOrigin, ViewMatrix_t vViewMatrix) {
	ImGui::GetBackgroundDrawList()->AddRect(
		{ Shared::vHeadPosScreen.x - Shared::BoxWidth, Shared::vHeadPosScreen.y }, 
		{ Shared::vHeadPosScreen.x + Shared::BoxWidth, Shared::vOriginScreen.y }, 
		ImColor(Config::ESP.AttributeColours[0], Config::ESP.AttributeColours[1], Config::ESP.AttributeColours[2]), 0.f
	);

	if (Config::ESP.FilledBox) {
		ImGui::GetBackgroundDrawList()->AddRectFilled(
			{ Shared::vHeadPosScreen.x - Shared::BoxWidth, Shared::vHeadPosScreen.y },
			{ Shared::vHeadPosScreen.x + Shared::BoxWidth, Shared::vOriginScreen.y },
			ImColor(Config::ESP.FilledBoxColours[0], Config::ESP.FilledBoxColours[1], Config::ESP.FilledBoxColours[2], Config::ESP.FilledBoxAlpha)
		);
	}
}

void ESP::PlayerName(std::string szName) {
	ImVec2 VTextSize = ImGui::CalcTextSize(szName.c_str());
	auto [fHorizontalOffset, fVerticalOffset] = Helpers::GetTextOffsets(VTextSize.x, VTextSize.y, 3.f);

	ImGui::GetBackgroundDrawList()->AddText(
		GUI::ESPText, 
		Helpers::GetFontSize(Config::ESP.MainText, Shared::Distance), 
		{ Shared::vHeadPosScreen.x - fHorizontalOffset, Shared::vHeadPosScreen.y - fVerticalOffset},
		ImColor(Config::ESP.AttributeColours[0], Config::ESP.AttributeColours[1], Config::ESP.AttributeColours[2]), szName.c_str()
	);
}

void ESP::Skeleton(Matrix2x4_t* pBoneCache) {
	ImColor vSkeletonColour = ImColor(Config::ESP.SkeletonColours[0], Config::ESP.SkeletonColours[1], Config::ESP.SkeletonColours[2]);
	ImColor vJointColour = ImColor(Config::ESP.JointColours[0], Config::ESP.JointColours[1], Config::ESP.JointColours[2]);
	Vector3 vPrevious, vCurrent;

	for (std::vector<int>iCurrentGroup : Shared::BoneGroups::AllGroups) {
		vPrevious = { 0.f,0.f,0.f };

		for (int iCurrentBone : iCurrentGroup) {
			vCurrent = pBoneCache->GetOrigin(iCurrentBone);

			if (vPrevious.IsZero()) {
				vPrevious = vCurrent;
				continue;
			}

			Vector3 vCurrentScreenPos = vCurrent.WorldToScreen(Shared::dwViewMatrix);
			Vector3 vPreviousScreenPos = vPrevious.WorldToScreen(Shared::dwViewMatrix);

			if (vCurrentScreenPos.z >= 0.01f && vPreviousScreenPos.z >= 0.01f) {
				ImGui::GetBackgroundDrawList()->AddLine(
					{ vPreviousScreenPos.x, vPreviousScreenPos.y },
					{ vCurrentScreenPos.x, vCurrentScreenPos.y },
					vSkeletonColour, 1.5f
				);
			}

			if (Config::ESP.IsJoint) ImGui::GetBackgroundDrawList()->AddCircleFilled({ vCurrentScreenPos.x,vCurrentScreenPos.y }, Helpers::GetJointSize(5.f, Shared::Distance), vJointColour);

			vPrevious = vCurrent;
		}
	}
}

void ESP::SnapLines() {
	ImGui::GetBackgroundDrawList()->AddLine(
		{ Shared::vHeadPosScreen.x - (Shared::BoxWidth / 2.f), Shared::vOriginScreen.y },
		{ ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y },
		ImColor(Config::ESP.AttributeColours[0], Config::ESP.AttributeColours[1], Config::ESP.AttributeColours[2])
	);
}

void ESP::Distance() {
	std::string sDistanceText = std::format("[{} m]", std::to_string(Shared::Distance));
	ImVec2 vTextSize = ImGui::CalcTextSize(sDistanceText.c_str());

	auto [fHorizontalOffset, fVerticalOffset] = Helpers::GetTextOffsets(vTextSize.x, vTextSize.y, 3.f);

	ImGui::GetBackgroundDrawList()->AddText(
		GUI::ESPTextSmall, 
		Helpers::GetFontSize(Config::ESP.SideText, Shared::Distance),
		{ Shared::vHeadPosScreen.x - fHorizontalOffset, Shared::vHeadPosScreen.y - fVerticalOffset - 12.f },
		ImColor(Config::ESP.AttributeColours[0], Config::ESP.AttributeColours[1], Config::ESP.AttributeColours[2]), 
		sDistanceText.c_str()
	);
}

void ESP::WeaponName(std::string szName, Vector3 vOrigin, bool IsOnGround) {
	// Name cleaning
	std::transform(szName.begin(), szName.end(), szName.begin(), [](unsigned char c) { return std::tolower(c); });
	szName.erase(0, 2);
	if (szName.find("weapon") == 0) szName.erase(0, std::string("weapon").length());

	// Name to icon
	float fVerticalDivied = (IsOnGround == false) ? Shared::BoxHeight + 15.f : 1.f;
	std::string szWeaponIcon = GunIcon(szName);
	ImVec2 vTextSize = ImGui::CalcTextSize(szWeaponIcon.c_str());
	auto [fHorizontalOffset, fVerticalOffset] = GetTextOffsets(vTextSize.x, vTextSize.y, 2.f, fVerticalDivied);

	// Render
	ImGui::GetBackgroundDrawList()->AddText(
		GUI::WeaponIcons,
		GetFontSize(Config::ESP.MainText, Shared::Distance),
		{ vOrigin.x - fHorizontalOffset - 5.f, vOrigin.y - fVerticalOffset },
		ImColor(Config::ESP.AttributeColours[0], Config::ESP.AttributeColours[1], Config::ESP.AttributeColours[2]),
		szWeaponIcon.c_str()
	);
}


// These might not be correct !!
std::string ESP::Util::GunIcon(std::string szName) {
	std::map<std::string, const char*> Icons = {
		{"ak47", "W"},
		{"m4a4", "M"},
		{"mp9", "R"},
		{"famas", "H"},
		{"ump45", "b"},
		{"glock", "D"},
		{"knife_ct", "]"},
		{"knife_t", "["},
		{"knife", "]"},
		{"deagle", "A"},
		{"elite", "B"},
		{"fiveseven", "C"},
		{"revolver", "J"},
		{"hkp2000", "E"},
		{"p250", "F"},
		{"usp_silencer", "G"},
		{"tec9", "H"},
		{"cz75a", "I"},
		{"mac10", "K"},
		{"ump45", "L"},
		{"bizon", "M"},
		{"mp7", "N"},
		{"mp9", "R"},
		{"mp5sd","x"},
		{"p90", "P"},
		{"galilar", "Q"},
		{"famas", "R"},
		{"m4a1_silencer", "T"},
		{"m4a1", "S"},
		{"aug", "U"},
		{"sg556", "V"},
		{"ak47", "W"},
		{"g3sg1", "X"},
		{"scar20", "Y"},
		{"awp", "Z"},
		{"ssg08", "a"},
		{"ssg-08", "a"},
		{"xm1014", "b"},
		{"sawedoff", "c"},
		{"mag7", "d"},
		{"nova", "e"},
		{"negev", "f"},
		{"m249", "g"},
		{"taser", "h"},
		{"flashbang", "i"},
		{"hegrenade", "j"},
		{"smokegrenade", "k"},
		{"molotov", "l"},
		{"decoy", "m"},
		{"incgrenade", "n"},
		{"c4", "o"},
		{"defuse kit", "r" },
	};

	auto it = Icons.find(szName);
	if (it != Icons.end())  return it->second;

	return "";
}
