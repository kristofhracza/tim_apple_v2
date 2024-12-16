#pragma once

#include "../../utilities/Virtual.hpp"



class CPVS{
public:
	void Set(bool bState){
		Virtual::Call<void*, 7U>(this, bState);
	}
};