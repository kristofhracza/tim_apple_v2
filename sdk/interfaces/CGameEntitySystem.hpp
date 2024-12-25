#pragma once

#include "../../utilities/Scanner.hpp"

#include "EntityHandle.h"



#define MAX_ENTITIES_IN_LIST 512
#define MAX_ENTITY_LISTS 64 // 0x3F
#define MAX_TOTAL_ENTITIES MAX_ENTITIES_IN_LIST* MAX_ENTITY_LISTS

class C_BaseEntity;

class CGameEntitySystem {
public:
	template <typename T = C_BaseEntity>
	T* Get(int m_nIndex){
		return reinterpret_cast<T*>(this->GetEntityByIndex(m_nIndex));
	}

	template <typename T = C_BaseEntity>
	T* Get(const CBaseHandle m_hHandle){
		if (!m_hHandle.IsValid())
			return nullptr;

		return reinterpret_cast<T*>(this->GetEntityByIndex(m_hHandle.GetEntryIndex()));
	}

	int GetHighestEntityIndex(){
		return *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(this) + 0x20F0); // dwGameEntitySystem_highestEntityIndex 
	}
private:
	// IDA:  #STR: "(missing),", "(missing)", "Ent %3d: %s class %s name %s\n" | or find "cl_showents" cvar -> look for callback
	void* GetEntityByIndex(int m_nIndex){
		using fnGetBaseEntity = void* (__thiscall*)(void*, int);
		static auto GetBaseEntity = reinterpret_cast<fnGetBaseEntity>(Scanner::PatternScan("client.dll", "81 FA ? ? ? ? 77 ? 8B C2 C1 F8 ? 83 F8 ? 77 ? 48 98 48 8B 4C C1 ? 48 85 C9 74 ? 8B C2 25 ? ? ? ? 48 6B C0 ? 48 03 C8 74 ? 8B 41 ? 25 ? ? ? ? 3B C2 75 ? 48 8B 01"));
		return GetBaseEntity(this, m_nIndex);
	}
};