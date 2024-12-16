#pragma once

#include "../../utilities/Virtual.hpp"



class CNetworkGameClient{
public:
	bool IsConnected(){
		return Virtual::Call<bool, 12U>(this);
	}

	void FullUpdate(){
		Virtual::Call<void, 28U>(this, "unk");
	}

	int GetDeltaTick(){
		return *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(this) + 0x25C);
	}
};

class INetworkClientService {
public:
	[[nodiscard]] CNetworkGameClient* GetNetworkGameClient(){
		return Virtual::Call<CNetworkGameClient*, 23U>(this);
	}
};
