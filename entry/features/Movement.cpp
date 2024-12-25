#include <cstdlib>

#include "Movement.hpp"

#include "../Config.hpp"

#include "../../sdk/tools/CUserCMD.hpp"


void MOVEMENT::AntiAim(CUserCmd* pCmd, CBaseUserCmdPB* pBaseCmd, CCSGOInput* pInput) {
    static bool bSwitch = false;

    // Random angles (harder to guess)
    float fRandomX = static_cast<float>((std::rand() % 50) + 89);
    float fRandomY = static_cast<float>((std::rand() % 110) + 180);

    // AA logic
    Vector3 VAntiAimAngle = {Config::MOVEMENT.Pitch, Config::MOVEMENT.Yaw ,0};
    Vector3 vNormalAngles = pBaseCmd->pViewAngles->angValue;
    if (Config::MOVEMENT.Jitter) {
        if (bSwitch) {
            VAntiAimAngle.y = pBaseCmd->pViewAngles->angValue.y + Config::MOVEMENT.Yaw - fRandomY;
            VAntiAimAngle.x = pBaseCmd->pViewAngles->angValue.x + Config::MOVEMENT.Pitch - fRandomX;
        }
        else {
            VAntiAimAngle.y = pBaseCmd->pViewAngles->angValue.y + Config::MOVEMENT.Yaw;
            VAntiAimAngle.x = pBaseCmd->pViewAngles->angValue.x + Config::MOVEMENT.Pitch;
        }
        bSwitch = !bSwitch;
    }
    else {
        VAntiAimAngle.y = pBaseCmd->pViewAngles->angValue.y - Config::MOVEMENT.Yaw;
    }
    VAntiAimAngle = NormalizeAngles(ClampAngles(VAntiAimAngle));

    // Set angles
    pBaseCmd->pViewAngles->angValue = VAntiAimAngle;
    
    // Movement Fix
    float fDelta = vNormalAngles.y - pBaseCmd->pViewAngles->angValue.y;
    Vector3 vOldMove = { pBaseCmd->flForwardMove,pBaseCmd->flSideMove,pBaseCmd->flUpMove };
    Vector3 vNewMove = {
       std::cos(Deg2Rad(fDelta)) * vOldMove.x + std::cos(Deg2Rad(fDelta + 90.f)) * vOldMove.y,
       std::sin(Deg2Rad(fDelta)) * vOldMove.x + std::sin(Deg2Rad(fDelta + 90.f)) * vOldMove.y,
       0.f,
    };
    pBaseCmd->flForwardMove = vNewMove.x;
    pBaseCmd->flSideMove = vNewMove.y;
}

void MOVEMENT::BunnyHop(C_CSPlayerPawn* pLocalPlayerPawn,CUserCmd* pCmd, CBaseUserCmdPB* pBaseCmd) {
    if (pLocalPlayerPawn->GetFlags() & FL_ONGROUND){
        pCmd->nButtons.nValue &= ~IN_JUMP;
    }
}
