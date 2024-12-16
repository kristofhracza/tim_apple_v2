#pragma once
#undef max

#include "../../utilities/Virtual.hpp"

#include "../../sdk/interfaces/Interfaces.hpp"



enum EFlags : int
{
	FL_ONGROUND = (1 << 0), // entity is at rest / on the ground
	FL_DUCKING = (1 << 1), // player is fully crouched/uncrouched
	FL_ANIMDUCKING = (1 << 2), // player is in the process of crouching or uncrouching but could be in transition
	FL_WATERJUMP = (1 << 3), // player is jumping out of water
	FL_ONTRAIN = (1 << 4), // player is controlling a train, so movement commands should be ignored on client during prediction
	FL_INRAIN = (1 << 5), // entity is standing in rain
	FL_FROZEN = (1 << 6), // player is frozen for 3rd-person camera
	FL_ATCONTROLS = (1 << 7), // player can't move, but keeps key inputs for controlling another entity
	FL_CLIENT = (1 << 8), // entity is a client (player)
	FL_FAKECLIENT = (1 << 9), // entity is a fake client, simulated server side; don't send network messages to them
	FL_INWATER = (1 << 10), // entity is in water
	FL_FLY = (1 << 11),
	FL_SWIM = (1 << 12),
	FL_CONVEYOR = (1 << 13),
	FL_NPC = (1 << 14),
	FL_GODMODE = (1 << 15),
	FL_NOTARGET = (1 << 16),
	FL_AIMTARGET = (1 << 17),
	FL_PARTIALGROUND = (1 << 18), // entity is standing on a place where not all corners are valid
	FL_STATICPROP = (1 << 19), // entity is a static property
	FL_GRAPHED = (1 << 20),
	FL_GRENADE = (1 << 21),
	FL_STEPMOVEMENT = (1 << 22),
	FL_DONTTOUCH = (1 << 23),
	FL_BASEVELOCITY = (1 << 24), // entity have applied base velocity this frame
	FL_WORLDBRUSH = (1 << 25), // entity is not moveable/removeable brush (part of the world, but represented as an entity for transparency or something)
	FL_OBJECT = (1 << 26),
	FL_KILLME = (1 << 27), // entity is marked for death and will be freed by the game
	FL_ONFIRE = (1 << 28),
	FL_DISSOLVING = (1 << 29),
	FL_TRANSRAGDOLL = (1 << 30), // entity is turning into client-side ragdoll
	FL_UNBLOCKABLE_BY_PLAYER = (1 << 31)
};

enum ECommandButtons : std::uint64_t {
	IN_ATTACK = 1 << 0,
	IN_JUMP = 1 << 1,
	IN_DUCK = 1 << 2,
	IN_FORWARD = 1 << 3,
	IN_BACK = 1 << 4,
	IN_USE = 1 << 5,
	IN_LEFT = 1 << 7,
	IN_RIGHT = 1 << 8,
	IN_MOVELEFT = 1 << 9,
	IN_MOVERIGHT = 1 << 10,
	IN_SECOND_ATTACK = 1 << 11,
	IN_RELOAD = 1 << 13,
	IN_SPRINT = 1 << 16,
	IN_JOYAUTOSPRINT = 1 << 17,
	IN_SHOWSCORES = 1ULL << 33,
	IN_ZOOM = 1ULL << 34,
	IN_LOOKATWEAPON = 1ULL << 35
};

enum ESubtickMoveStepBits : std::uint32_t {
	MOVESTEP_BITS_BUTTON = 0x1U,
	MOVESTEP_BITS_PRESSED = 0x2U,
	MOVESTEP_BITS_WHEN = 0x4U,
	MOVESTEP_BITS_ANALOG_FORWARD_DELTA = 0x8U,
	MOVESTEP_BITS_ANALOG_LEFT_DELTA = 0x10U
};

enum EInputHistoryBits : std::uint32_t {
	INPUT_HISTORY_BITS_VIEWANGLES = 0x1U,
	INPUT_HISTORY_BITS_SHOOTPOSITION = 0x2U,
	INPUT_HISTORY_BITS_TARGETHEADPOSITIONCHECK = 0x4U,
	INPUT_HISTORY_BITS_TARGETABSPOSITIONCHECK = 0x8U,
	INPUT_HISTORY_BITS_TARGETANGCHECK = 0x10U,
	INPUT_HISTORY_BITS_CL_INTERP = 0x20U,
	INPUT_HISTORY_BITS_SV_INTERP0 = 0x40U,
	INPUT_HISTORY_BITS_SV_INTERP1 = 0x80U,
	INPUT_HISTORY_BITS_PLAYER_INTERP = 0x100U,
	INPUT_HISTORY_BITS_RENDERTICKCOUNT = 0x200U,
	INPUT_HISTORY_BITS_RENDERTICKFRACTION = 0x400U,
	INPUT_HISTORY_BITS_PLAYERTICKCOUNT = 0x800U,
	INPUT_HISTORY_BITS_PLAYERTICKFRACTION = 0x1000U,
	INPUT_HISTORY_BITS_FRAMENUMBER = 0x2000U,
	INPUT_HISTORY_BITS_TARGETENTINDEX = 0x4000U
};

enum EButtonStatePBBits : uint32_t {
	BUTTON_STATE_PB_BITS_BUTTONSTATE1 = 0x1U,
	BUTTON_STATE_PB_BITS_BUTTONSTATE2 = 0x2U,
	BUTTON_STATE_PB_BITS_BUTTONSTATE3 = 0x4U
};

enum EBaseCmdBits : std::uint32_t {
	BASE_BITS_MOVE_CRC = 0x1U,
	BASE_BITS_BUTTONPB = 0x2U,
	BASE_BITS_VIEWANGLES = 0x4U,
	BASE_BITS_COMMAND_NUMBER = 0x8U,
	BASE_BITS_CLIENT_TICK = 0x10U,
	BASE_BITS_FORWARDMOVE = 0x20U,
	BASE_BITS_LEFTMOVE = 0x40U,
	BASE_BITS_UPMOVE = 0x80U,
	BASE_BITS_IMPULSE = 0x100U,
	BASE_BITS_WEAPON_SELECT = 0x200U,
	BASE_BITS_RANDOM_SEED = 0x400U,
	BASE_BITS_MOUSEDX = 0x800U,
	BASE_BITS_MOUSEDY = 0x1000U,
	BASE_BITS_CONSUMED_SERVER_ANGLE = 0x2000U,
	BASE_BITS_CMD_FLAGS = 0x4000U,
	BASE_BITS_ENTITY_HANDLE = 0x8000U
};

enum ECSGOUserCmdBits : std::uint32_t {
	CSGOUSERCMD_BITS_BASECMD = 0x1U,
	CSGOUSERCMD_BITS_LEFTHAND = 0x2U,
	CSGOUSERCMD_BITS_ATTACK3START = 0x4U,
	CSGOUSERCMD_BITS_ATTACK1START = 0x8U,
	CSGOUSERCMD_BITS_ATTACK2START = 0x10U
};



template <typename T>
struct RepeatedPtrField_t {
	struct Rep_t {
		int nAllocatedSize;
		T* tElements[(std::numeric_limits<int>::max() - 2 * sizeof(int)) / sizeof(void*)];
	};

	void* pArena;
	int nCurrentSize;
	int nTotalSize;
	Rep_t* pRep;
};


class CBasePB {
public:
	MEM_PAD(0x8)
	std::uint32_t nHasBits;
	std::uint64_t nCachedBits;

	void SetBits(std::uint64_t nBits) {
		nCachedBits |= nBits;
	}
};


class CMsgQAngle : public CBasePB {
public:
	Vector3 angValue;
};


class CMsgVector : public CBasePB {
public:
	Vector4D_t vecValue;
};


class CCSGOInterpolationInfoPB : public CBasePB {
public:
	float flFraction;
	int nSrcTick;
	int nDstTick;
};


class CCSGOInputHistoryEntryPB : public CBasePB {
public:
	CMsgQAngle* pViewAngles;
	CMsgVector* pShootPosition;
	CMsgVector* pTargetHeadPositionCheck;
	CMsgVector* pTargetAbsPositionCheck;
	CMsgQAngle* pTargetAngPositionCheck;
	CCSGOInterpolationInfoPB* cl_interp;
	CCSGOInterpolationInfoPB* sv_interp0;
	CCSGOInterpolationInfoPB* sv_interp1;
	CCSGOInterpolationInfoPB* player_interp;
	int nRenderTickCount;
	float flRenderTickFraction;
	int nPlayerTickCount;
	float flPlayerTickFraction;
	int nFrameNumber;
	int nTargetEntIndex;
};


struct CInButtonStatePB : CBasePB {
	std::uint64_t nValue;
	std::uint64_t nValueChanged;
	std::uint64_t nValueScroll;
};


struct CSubtickMoveStep : CBasePB {
public:
	std::uint64_t nButton;
	bool bPressed;
	float flWhen;
	float flAnalogForwardDelta;
	float flAnalogLeftDelta;
};


class CBaseUserCmdPB : public CBasePB {
public:
	RepeatedPtrField_t<CSubtickMoveStep> subtickMovesField;
	std::string* strMoveCrc;
	CInButtonStatePB* pInButtonState;
	CMsgQAngle* pViewAngles;
	std::int32_t nLegacyCommandNumber;
	std::int32_t nClientTick;
	float flForwardMove;
	float flSideMove;
	float flUpMove;
	std::int32_t nImpulse;
	std::int32_t nWeaponSelect;
	std::int32_t nRandomSeed;
	std::int32_t nMousedX;
	std::int32_t nMousedY;
	std::uint32_t nConsumedServerAngleChanges;
	std::int32_t nCmdFlags;
	std::uint32_t nPawnEntityHandle;

	int CalculateCmdCRCSize() {
		return Virtual::Call<int, 7>(this);
	}
};


class CCSGOUserCmdPB {
public:
	std::uint32_t nHasBits;
	std::uint64_t nCachedSize;
	RepeatedPtrField_t<CCSGOInputHistoryEntryPB> inputHistoryField;
	CBaseUserCmdPB* pBaseCmd;
	bool bLeftHandDesired;
	std::int32_t nAttack3StartHistoryIndex;
	std::int32_t nAttack1StartHistoryIndex;
	std::int32_t nAttack2StartHistoryIndex;

	void CheckAndSetBits(std::uint32_t nBits) {
		if (!(nHasBits & nBits))
			nHasBits |= nBits;
	}
};

struct CInButtonState {
public:
	MEM_PAD(0x8);
	std::uint64_t nValue;
	std::uint64_t nValueChanged;
	std::uint64_t nValueScroll;
};


class CUserCmd {
public:
	MEM_PAD(0x8);
	MEM_PAD(0x10);
	CCSGOUserCmdPB csgoUserCmd;
	CInButtonState nButtons;
	MEM_PAD(0x20);

	CCSGOInputHistoryEntryPB* GetInputHistoryEntry(int nIndex) {
		if (nIndex >= csgoUserCmd.inputHistoryField.pRep->nAllocatedSize || nIndex >= csgoUserCmd.inputHistoryField.nCurrentSize)
			return nullptr;

		return csgoUserCmd.inputHistoryField.pRep->tElements[nIndex];
	}

	void SetSubTickAngle(const Vector3& angView) {
		for (int i = 0; i < this->csgoUserCmd.inputHistoryField.pRep->nAllocatedSize; i++) {
			CCSGOInputHistoryEntryPB* pInputEntry = this->GetInputHistoryEntry(i);
			if (!pInputEntry || !pInputEntry->pViewAngles)
				continue;

			pInputEntry->pViewAngles->angValue = angView;
			pInputEntry->SetBits(EInputHistoryBits::INPUT_HISTORY_BITS_VIEWANGLES);
		}
	}
};
