#pragma once

#include "../../utilities/Virtual.hpp"
#include "../../utilities/Vectors.h"



enum EClientFrameStage : int{
	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,
	FRAME_RENDER_START,
	FRAME_RENDER_END,
	FRAME_NET_FULL_FRAME_UPDATE_ON_REMOVE
};


class IEngineClient{
public:
	int GetMaxClients(){
		return Virtual::Call<int, 34U>(this);
	}

	bool IsInGame(){
		return Virtual::Call<bool, 35U>(this);
	}

	bool IsConnected(){
		return Virtual::Call<bool, 36U>(this);
	}

	int GetLocalPlayer(){
		int nIndex = -1;
		Virtual::Call<void, 49U>(this, std::ref(nIndex), 0);
		return nIndex + 1;
	}

	[[nodiscard]] const char* GetLevelName(){
		return Virtual::Call<const char*, 56U>(this);
	}

	[[nodiscard]] const char* GetLevelNameShort(){
		return Virtual::Call<const char*, 57U>(this);
	}

	[[nodiscard]] const char* GetProductVersionString(){
		return Virtual::Call<const char*, 82U>(this);
	}
};