#include "Attributes.hpp"



CCSPlayerController* CCSPlayerController::GetLocalPlayerController() {
	int m_nIndex = Interfaces::EngineClient->GetLocalPlayer();
	return Interfaces::GameResourceService->m_pGameEntitySystem->Get<CCSPlayerController>(m_nIndex);
}


const Vector3& CCSPlayerController::GetPawnOrigin(){
	CBaseHandle m_hPawnHandle = this->GetPawnHandle();
	if (!m_hPawnHandle.IsValid()) return Vector3{ 0,0,0 };

	C_CSPlayerPawn* pPlayerPawn = Interfaces::GameResourceService->m_pGameEntitySystem->Get<C_CSPlayerPawn>(m_hPawnHandle);
	if (pPlayerPawn == nullptr) return Vector3{0,0,0};

	return pPlayerPawn->GetSceneOrigin();
}


C_BaseEntity* C_BaseEntity::GetLocalPlayer(){
	const int m_nIndex = Interfaces::EngineClient->GetLocalPlayer();
	return Interfaces::GameResourceService->m_pGameEntitySystem->Get(m_nIndex);
}


bool C_BaseEntity::IsWeapon() {
	static SchemaClassInfoData_t* pWeaponClassInfo = nullptr;
	Interfaces::SchemaSystem->FindTypeScopeForModule("client.dll")->FindDeclaredClass(&pWeaponClassInfo,"C_CSWeaponBase");
	
	SchemaClassInfoData_t* pClassInfo;
	GetSchemaClassInfo(&pClassInfo);

	if (pClassInfo == nullptr) return false;

	return (pClassInfo->InheritsFrom(pWeaponClassInfo));
}


const Vector3& C_BaseEntity::GetSceneOrigin(){
	if (this->GetGameSceneNode())
		return GetGameSceneNode()->GetAbsOrigin();

	return Vector3{0,0,0};
}


Vector3 C_BasePlayerPawn::GetEyePosition() {
	Vector3 vecEyePosition = Vector3(0.0f, 0.0f, 0.0f);
	Virtual::Call<void, 169>(this, &vecEyePosition);
	return vecEyePosition;
}


std::uint32_t C_CSPlayerPawn::GetOwnerHandleIndex(){
	std::uint32_t Result = -1;
	if (this && GetCollision() && !(GetCollision()->GetSolidFlags() & 4))
		Result = this->GetOwnerHandle().GetEntryIndex();

	return Result;
}


std::uint16_t C_CSPlayerPawn::GetCollisionMask(){
	if (this && GetCollision())
		return GetCollision()->CollisionMask(); // Collision + 0x38

	return 0;
}
