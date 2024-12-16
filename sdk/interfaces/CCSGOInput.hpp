#pragma once

#include "../../utilities/Virtual.hpp"
#include "../tools/CUserCMD.hpp"

#define MULTIPLAYER_BACKUP 150



class CTinyMoveStepData
{
public:
	float flWhen; 
	MEM_PAD(0x4); 
	std::uint64_t nButton; 
	bool bPressed; 
	MEM_PAD(0x7); 
}; 

class CMoveStepButtons
{
public:
	std::uint64_t nKeyboardPressed; 
	std::uint64_t nMouseWheelheelPressed; 
	std::uint64_t nUnPressed; 
	std::uint64_t nKeyboardCopy; 
}; 

class CExtendedMoveData : public CMoveStepButtons
{
public:
	float flForwardMove; 
	float flSideMove; 
	float flUpMove; 
	std::int32_t nMouseDeltaX; 
	std::int32_t nMouseDeltaY; 
	std::int32_t nAdditionalStepMovesCount; 
	CTinyMoveStepData tinyMoveStepData[12]; 
	Vector3 vecViewAngle; 
	std::int32_t nTargetHandle; 
};

class CCSGOInput{
public:
	MEM_PAD(0x1)
	bool bInThirdPerson;
	MEM_PAD(0x6);
	Vector3 angThirdPersonAngles;
	MEM_PAD(0x10);
	std::int32_t nSequenceNumber;
	double dbSomeTimer;
	CExtendedMoveData currentMoveData;
	std::int32_t nWeaponSwitchTick;
	MEM_PAD(0x1C4);
	CExtendedMoveData* pExtendedMoveData;
	MEM_PAD(0x48);
	int32_t nAttackStartHistoryIndex1;
	int32_t nAttackStartHistoryIndex2;
	int32_t nAttackStartHistoryIndex3;


	void SetViewAngle(Vector3& angView){
		using fnSetViewAngle = std::int64_t(__fastcall*)(void*, std::int32_t, Vector3&);
		static auto oSetViewAngle = reinterpret_cast<fnSetViewAngle>(Scanner::PatternScan("client.dll", "85 D2 75 3F 48"));

		oSetViewAngle(this, 0, std::ref(angView));
	}

	Vector3 GetViewAngles(){
		using fnGetViewAngles = std::int64_t(__fastcall*)(CCSGOInput*, std::int32_t);
		static auto oGetViewAngles = reinterpret_cast<fnGetViewAngles>(Scanner::PatternScan("client.dll", "4C 8B C1 85 D2 74 08 48 8D 05 ? ? ? ? C3"));

		return *reinterpret_cast<Vector3*>(oGetViewAngles(this, 0));
	}
};