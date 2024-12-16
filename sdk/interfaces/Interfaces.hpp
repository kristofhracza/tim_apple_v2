#pragma once

#include <d3d11.h>

#include "SchemaSystem.hpp"
#include "CGameEntitySystem.hpp"
#include "IGameResourceService.hpp"
#include "IEngineClient.hpp"
#include "IResourceSystem.hpp"
#include "IGlobalVars.hpp"
#include "ISwapChainDX11.hpp"
#include "INetworkClientService.hpp"
#include "CGameTraceManager.hpp"
#include "IInputSystem.hpp"
#include "IPVS.hpp"
#include "CCSGOInput.hpp"

#include "../../utilities/Scanner.hpp"


#define GAME_RESOURCE_SERVICE_CLIENT "GameResourceServiceClientV001"
#define SOURCE2_CLIENT "Source2Client002"
#define SCHEMA_SYSTEM "SchemaSystem_001"
#define INPUT_SYSTEM_VERSION "InputSystemVersion001"
#define SOURCE2_ENGINE_TO_CLIENT "Source2EngineToClient001"
#define ENGINE_CVAR "VEngineCvar007"
#define LOCALIZE "Localize_001"
#define NETWORK_CLIENT_SERVICE "NetworkClientService_001"
#define MATERIAL_SYSTEM2 "VMaterialSystem2_001"
#define RESOURCE_SYSTEM "ResourceSystem013"
#define RESOURCE_HANDLE_UTILS "ResourceHandleUtils001"


namespace InterfaceFunctions {
	typedef void* (__cdecl* tCreateInterface)(const char* name, int* retCode);
	template <typename T>
	T* GetInterface(const char* interfaceName, const char* dll);
	void SetInterfaces();
}


class ISource2Client;
class IEngineCVar;
class IMaterialSystem2;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;


namespace Interfaces {
	void CreateRenderTarget(IDXGISwapChain* pSwapChain);
	void DestroyRenderTarget();
	void DestroyDeviceContext();
	void DestroyDevice();

	inline ISwapChainDx11* SwapChain = nullptr;
	inline ID3D11Device* Device = nullptr;
	inline ID3D11DeviceContext* DeviceContext = nullptr;
	inline ID3D11RenderTargetView* RenderTargetView = nullptr;

	inline CCSGOInput* Input = nullptr;
	inline IInputSystem* InputSystem = nullptr;
	inline IGlobalVars* GlobalVars = nullptr;
	inline CSchemaSystem* SchemaSystem = nullptr;
	inline IGameResourceService* GameResourceService = nullptr;
	inline ISource2Client* Source2Client = nullptr;
	inline IEngineClient* EngineClient = nullptr;
	inline IEngineCVar* EngineCVar = nullptr;
	inline INetworkClientService* NetworkClientService = nullptr;
	inline IMaterialSystem2* MaterialSystem2 = nullptr;
	inline IResourceSystem* ResourceSystem = nullptr;
	inline CResourceHandleUtils* ResourceHandleUtils = nullptr;
	inline CPVS* PVS = nullptr;
	inline CGameTraceManager* GameTraceManager = nullptr;
};