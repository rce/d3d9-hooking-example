#pragma once

#include <cstdint>


struct D3DAddresses
{
	uintptr_t addrEndScene;
	uintptr_t addrDrawIndexedPrimitive;
};
D3DAddresses FindEndScene();