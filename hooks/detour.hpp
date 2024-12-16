#pragma once

#include "../dependencies/minhook/include/MinHook.h"

template<typename T>
class CBaseHookObject {
public:
	bool Create(void* m_pFunction, void* m_pDetour) {
		m_pBaseFunction = m_pFunction;
		m_pReplaceFunction = m_pDetour;

		const MH_STATUS status = MH_CreateHook(m_pBaseFunction, m_pReplaceFunction, &m_pOriginalFunction);
		if (status != MH_OK) return false;
		if (!Replace()) return false;

		return true;
	}

	bool Replace() {
		if (m_pBaseFunction == nullptr || m_bIsHooked) return false;

		const MH_STATUS status = MH_EnableHook(m_pBaseFunction);
		if (status != MH_OK) return false;
	
		m_bIsHooked = true;
		return true;
	}

	bool Remove() {
		if (!Restore()) return false;

		const MH_STATUS status = MH_RemoveHook(m_pBaseFunction);
		if (status != MH_OK) return false;

		return true;
	}

	bool Restore() {
		if (!m_bIsHooked) return false;

		const MH_STATUS status = MH_DisableHook(m_pBaseFunction);
		
		m_bIsHooked = false;
		return true;
	}

	__forceinline T GetOriginal() {
		return reinterpret_cast<T>(m_pOriginalFunction);
	}

	__forceinline bool IsHooked() const{
		return m_bIsHooked;
	}

private:
	bool m_bIsHooked = false;
	void* m_pBaseFunction = nullptr;
	void* m_pReplaceFunction = nullptr;
	void* m_pOriginalFunction = nullptr;
};