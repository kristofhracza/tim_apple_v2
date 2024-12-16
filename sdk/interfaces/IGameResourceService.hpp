#pragma once

class CGameEntitySystem;

class IGameResourceService{
public:
	char pad[0x58];
	CGameEntitySystem* m_pGameEntitySystem;
};