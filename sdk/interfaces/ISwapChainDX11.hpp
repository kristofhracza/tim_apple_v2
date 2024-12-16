#pragma once

#include "../../utilities/Scanner.hpp"



struct IDXGISwapChain;

class ISwapChainDx11{
	MEM_PAD(0x170);
	IDXGISwapChain* pDXGISwapChain;
};