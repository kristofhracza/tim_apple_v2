#pragma once

#include <string>
#include <vector>
#include <map>



namespace Config {
	struct MenuSettings {
		bool IsDebug = false;
	};
	inline MenuSettings SETTINGS;

	struct ESPConfig {
		bool state = false;

		bool IsBoundingBox = false;
		float BoxWidth = 2.5f;

		bool FilledBox = false;
		float FilledBoxAlpha = 0.2f;
		float FilledBoxColours[3] = { 1.f,1.f,1.f };

		bool IsHealthBar = false;
		bool HPCounter = false;
		float Health[3];

		bool IsPlayerName;
		std::string PlayerName;

		bool IsWeaponName;
		std::string WeaponName;

		bool IsGroundESP;

		float AttributeColours[3] = { 1.f,1.f,1.f };

		bool IsSkeleton;
		float SkeletonColours[3] = { 1.f,1.f,1.f };

		bool IsJoint;
		float JointColours[3] = { 1.f,1.f,1.f };

		bool SnapLines;

		bool Distance;

		// Private
		float MainText = 15.f;
		float SideText = 12.f;
	};
	inline ESPConfig ESP = {};



	struct AimConfig {
		bool state = false;

		bool Silent = false;

		bool Spotted = false;

		float FOV = 2.5f;
		bool FOVCircle = true;
		float Smoothing = 1.f;

		int Bone;
		int	BoneSelect = 0;
		std::vector<std::string> Bones = { "Head", "Neck","Chest", "Crotch" };
		std::map <std::string, int> BoneMap = { {"Head",6},{"Neck",5},{"Chest",4},{"Crotch",0} };

		bool IsRCS = false;
		bool IsTrigger = false;
	};
	inline AimConfig AIM = {};



	struct MovementConfig {
		bool IsAA = false;

		bool bJitter = false;
		float fYaw = 180.f;
		float fPitch = 89.f;

		bool IsBHOP = false;
	}; inline MovementConfig MOVEMENT = {};



	struct VisualConfig {
		bool IsSmoke = false;
		bool IsSmokeColour = false;
		float SmokeColours[3] = { 0.f,1.f,1.f };

	}; inline VisualConfig VISUALS = {};



	struct MiscConfig {
		bool IsFlashed = false;
	}; inline MiscConfig MISC = {};
}