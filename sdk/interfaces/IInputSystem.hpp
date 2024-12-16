#pragma once

#include <cstdint>



class IInputSystem{
public:
	bool IsRelativeMouseMode(){
		return *reinterpret_cast<bool*>(reinterpret_cast<std::uintptr_t>(this) + 0x4D);
	}

	void* GetSDLWindow(){
		return *reinterpret_cast<void**>(reinterpret_cast<std::uintptr_t>(this) + 0x26A8);
	}
};