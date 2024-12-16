#pragma once

#include <string>

#include "../../utilities/Vectors.h"

#include "../../sdk/Attributes.hpp"



namespace AIM {
	void Aimbot(C_CSPlayerPawn* pLocalPlayerPawn, CUserCmd* pCmd, CBaseUserCmdPB* pBaseCmd,CCSGOInput* pInput);
	void RCS(C_CSPlayerPawn* pLocalPlayerPawn, CUserCmd* pCmd, CBaseUserCmdPB* pBaseCmd, CCSGOInput* pInput);
	void TriggerBot(C_CSPlayerPawn* pLocalPlayerPawn, CUserCmd* pCmd, CBaseUserCmdPB* pBaseCmd, CCSGOInput* pInput);
}