#pragma once

#include "../utilities/Vectors.h"



namespace Shared {
	inline ViewMatrix_t dwViewMatrix;
	inline  Vector3 vecCameraPos;
	
	inline Vector3 vOrigin;
	inline Vector3 vOriginScreen;
	inline Vector3 vHeadPos;
	inline Vector3 vHeadPosScreen;
	
	inline float BoxWidth;
	inline float BoxHeight;

	inline int Distance;

    enum Bones : int {
        Head = 6,
        Neck = 5,
        Chest = 4,
        ShoulderRight = 8,
        ShoulderLeft = 13,
        ElbowRight = 9,
        ElbowLeft = 14,
        HandRight = 11,
        HandLeft = 16,
        Crotch = 0,
        KneeRight = 23,
        KneeLeft = 26,
        AnkleRight = 24,
        AnkleLeft = 27,
    };

    inline namespace BoneGroups {
        inline std::vector<int> Mid = { Bones::Head, Bones::Neck, Bones::Chest, Bones::Crotch };
        inline std::vector<int> LeftArm = { Bones::Neck, Bones::ShoulderLeft, Bones::ElbowLeft, Bones::HandLeft };
        inline std::vector<int> RightArm = { Bones::Neck, Bones::ShoulderRight, Bones::ElbowRight, Bones::HandRight };
        inline std::vector<int> LeftLeg = { Bones::Crotch, Bones::KneeLeft, Bones::AnkleLeft };
        inline std::vector<int> RightLeg = { Bones::Crotch, Bones::KneeRight, Bones::AnkleRight };
        inline std::vector<std::vector<int>> AllGroups = { Mid, LeftArm, RightArm, LeftLeg, RightLeg };
    }
}