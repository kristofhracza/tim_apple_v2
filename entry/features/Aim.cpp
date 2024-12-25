#include "Aim.hpp"

#include "Movement.hpp"

#include "../Config.hpp"
#include "../SharedData.hpp"

#include "../../dependencies/imgui/imgui.h"

#include "../../sdk//interfaces/Interfaces.hpp"
#include "../../sdk/interfaces/CGameTraceManager.hpp"

#include "../../utilities/Logger.hpp"



void AIM::Aimbot(C_CSPlayerPawn* pLocalPlayerPawn, CUserCmd* pCmd, CBaseUserCmdPB* pBaseCmd, CCSGOInput* pInput) {
	Vector3 vAngleAimAt;
	Vector3 vvCurrentPunchRecoil;
	Vector3 vNewViewAngle;
	float fCurrentFov = Config::AIM.FOV;

	for (int i = 1; i < Interfaces::EngineClient->GetMaxClients(); i++) {

		C_BaseEntity* pBaseEntity = Interfaces::GameResourceService->m_pGameEntitySystem->Get(i);
		if (pBaseEntity == nullptr) continue;

		CCSPlayerController* pPlayerController = reinterpret_cast<CCSPlayerController*>(pBaseEntity);
		if (pPlayerController == nullptr) continue;

		C_CSPlayerPawn* pPlayerPawn = Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>(pPlayerController->GetPawnHandle());
		if (pPlayerPawn == nullptr) continue;

		if (!pPlayerController->IsPawnAlive()) continue;
		if (pLocalPlayerPawn->GetTeam() == pPlayerPawn->GetTeam()) continue;

		if (pPlayerPawn->GetGameSceneNode() == nullptr) continue;
		CSkeletonInstance* pSkeletonInstance = pPlayerPawn->GetGameSceneNode()->GetSkeletonInstance();

		Matrix2x4_t* pBoneCache = pSkeletonInstance->pBoneCache;
		if (pBoneCache == nullptr) continue;
		Vector3 pPlayerPawnBone = pBoneCache->GetOrigin(Config::AIM.BoneMap[Config::AIM.Bones[Config::AIM.BoneSelect]]);

		// Spotted entity check with Ray Trace
		if (Config::AIM.Spotted) {
			GameTrace_t Trace = GameTrace_t();
			TraceFilter_t Filter = TraceFilter_t(0x1C3003, pLocalPlayerPawn, nullptr, 4);
			Ray_t Ray = Ray_t();

			Interfaces::GameTraceManager->TraceShape(&Ray, pLocalPlayerPawn->GetEyePosition(), pPlayerPawn->GetGameSceneNode()->GetSkeletonInstance()->pBoneCache->GetOrigin(Config::AIM.BoneMap[Config::AIM.Bones[Config::AIM.BoneSelect]]), &Filter, &Trace);
			if (Trace.m_pHitEntity != pPlayerPawn || !Trace.IsVisible()) continue;
		}

		// Calculating angles
		if (pLocalPlayerPawn->GetShotsFired() >= 1) vvCurrentPunchRecoil = pLocalPlayerPawn->GetAimPunchAngle() * 2.f;
		Vector3 vAngleDifference = CalculateAngle(pLocalPlayerPawn->GetEyePosition(), pPlayerPawnBone, (pBaseCmd->pViewAngles->angValue + vvCurrentPunchRecoil));

		// This is basically CalculateBestAngle from Vectors.h
		// We need direct FOV calculations for calculating the closest player
		float fCalcFov = std::hypot(vAngleDifference.x, vAngleDifference.y);
		if (fCalcFov < fCurrentFov) {
			fCurrentFov = fCalcFov;
			vAngleAimAt = vAngleDifference;
		}
		else continue;

		if (vAngleAimAt.IsZero()) continue;
	}

	vNewViewAngle = ClampAngles(NormalizeAngles(vNewViewAngle));
	if (Config::AIM.Silent)
		vNewViewAngle = pBaseCmd->pViewAngles->angValue + vAngleAimAt;
	else
		vNewViewAngle = pBaseCmd->pViewAngles->angValue + vAngleAimAt / Config::AIM.Smoothing;

	pCmd->SetSubTickAngle(vNewViewAngle);

	// Normal aim -- we set our view angle as well
	if (!Config::AIM.Silent) {
		pBaseCmd->pViewAngles->angValue = vNewViewAngle;
		pInput->SetViewAngle(pBaseCmd->pViewAngles->angValue);
	}
}


void AIM::RCS(C_CSPlayerPawn* pLocalPlayerPawn, CUserCmd* pCmd, CBaseUserCmdPB* pBaseCmd, CCSGOInput* pInput) {
	static Vector3 VOldPunch = { 0.f, 0.f, 0.f };

	// Current angles
	Vector3 vCurrentPunch = pLocalPlayerPawn->GetAimPunchAngle();
	Vector3 vViewAngles = pBaseCmd->pViewAngles->angValue;

	// Update angles
	if (pLocalPlayerPawn->GetShotsFired() > 1) {
		Vector3 vRecoilVector = vViewAngles + VOldPunch - vCurrentPunch * 2.f;
		Vector3 vNewAngle = NormalizeAngles(ClampAngles(vRecoilVector));

		pCmd->SetSubTickAngle(vNewAngle);
		pInput->SetViewAngle(vNewAngle);
	}
	VOldPunch = vCurrentPunch * 2.f;
}


void AIM::TriggerBot(C_CSPlayerPawn* pLocalPlayerPawn, CUserCmd* pCmd, CBaseUserCmdPB* pBaseCmd, CCSGOInput* pInput) {
	// Tracer
	GameTrace_t Trace = GameTrace_t();
	TraceFilter_t Filter = TraceFilter_t(0x1C3003, pLocalPlayerPawn, nullptr, 4);
	Ray_t Ray = Ray_t();

	// Current angles
	Vector3 vCurrentPunch = pLocalPlayerPawn->GetAimPunchAngle();
	Vector3 vViewAngles = pInput->GetViewAngles();

	// Distance between entities
	const Vector3 vDistance = {
		std::cos(Deg2Rad(vViewAngles.x)) * std::cos(Deg2Rad(vViewAngles.y)) * 8192.f,
		std::cos(Deg2Rad(vViewAngles.x)) * std::sin(Deg2Rad(vViewAngles.y)) * 8192.f,
		-std::sin(Deg2Rad(vViewAngles.x)) * 8192.f
	};

	// Check entity and fire
	Interfaces::GameTraceManager->TraceShape(&Ray, pLocalPlayerPawn->GetEyePosition(), pLocalPlayerPawn->GetEyePosition() + vDistance, &Filter, &Trace);
	if (Trace.m_pHitEntity && Trace.m_pHitEntity->GetHealth() > 0 && Trace.m_pHitEntity->GetHealth() <= 100 && Trace.m_pHitEntity->GetTeam() != pLocalPlayerPawn->GetTeam()) {
		pBaseCmd->SetBits(BASE_BITS_BUTTONPB);
		pCmd->nButtons.nValue |= IN_ATTACK;
	};
}
