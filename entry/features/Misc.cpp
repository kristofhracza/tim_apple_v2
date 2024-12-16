#include "Misc.hpp"

void MISC::AntiFlash() {
	if (Interfaces::EngineClient->IsConnected() && Interfaces::EngineClient->IsInGame()) {
		CCSPlayerController* pLocalPlayerController = CCSPlayerController::GetLocalPlayerController();
		C_CSPlayerPawn* pLocalPlayerPawn = Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>(pLocalPlayerController->GetPawnHandle());

		if (Config::MISC.IsFlashed)
			pLocalPlayerPawn->GetFlashDuration() = 0.f;
		else
			pLocalPlayerPawn->GetFlashDuration() = 255.f;
	}
};