#pragma once

#include "../../utilities/Virtual.hpp"



struct ResourceBinding_t;

class IResourceSystem{
public:
	void* QueryInterface(const char* szInterfaceName){
		return Virtual::Call<void*, 2U>(this, szInterfaceName);
	}
};

class CResourceHandleUtils {
public:
	void DeleteResource(const ResourceBinding_t* pBinding) {
		Virtual::Call<void, 2U>(this, pBinding);
	}
};