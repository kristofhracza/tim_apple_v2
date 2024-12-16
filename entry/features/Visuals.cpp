#include "Visuals.hpp"

#include "../Config.hpp"

#include "../../sdk/Attributes.hpp"



void VISUALS::SmokeColourMod(){
	if (!Interfaces::EngineClient->IsConnected() || !Interfaces::EngineClient->IsInGame()) return;
	for (int i = 1; i <= Interfaces::GameResourceService->m_pGameEntitySystem->GetHighestEntityIndex(); i++) {
		C_BaseEntity* pBaseEntity = Interfaces::GameResourceService->m_pGameEntitySystem->Get(i);
		if (pBaseEntity == nullptr) continue;

		// Get class info
		SchemaClassInfoData_t* pClassInfo = nullptr;
		pBaseEntity->GetSchemaClassInfo(&pClassInfo);
		if (pClassInfo == nullptr) continue;


		// C_SmokeGrenadeProjectile -- There is C_SmokeGrenade, but that's the item in the player's hand
		if (std::string(pClassInfo->m_szName) == "C_SmokeGrenadeProjectile") {
			C_SmokeGrenadeProjectile* pSmoke = reinterpret_cast<C_SmokeGrenadeProjectile*>(pBaseEntity);
			if (pSmoke == nullptr) continue;
			pSmoke->GetColour() = {Config::VISUALS.SmokeColours[0] * 255,Config::VISUALS.SmokeColours[1] * 255 ,Config::VISUALS.SmokeColours[2] * 255};
		}
	}
}