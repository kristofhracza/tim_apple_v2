#pragma once

#include "../../utilities/Vectors.h"

#include "../../sdk/Attributes.hpp"



namespace MOVEMENT {
	void AntiAim(CUserCmd* pCmd, CBaseUserCmdPB* pBaseCmd, CCSGOInput* pInput);
	void BunnyHop(C_CSPlayerPawn* pLocalPlayerPawn,CUserCmd* pCmd, CBaseUserCmdPB* pBaseCmd);
}