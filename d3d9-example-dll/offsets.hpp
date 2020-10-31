#pragma once

#include <Psapi.h>
#include <cstdint>
#include <vector>

#include "GTAIV.hpp"

// These offsets have to be reverse engineered by hand. These might or
// might not be the same over different applications and Direct3D9 versions.
const intptr_t offsetPresent = 0x44;
const intptr_t offsetBeginScene = 0xA4;
const intptr_t offsetEndScene = 0xA8;
const intptr_t offsetClear = 0xAC;

uintptr_t TraverseOffsets(LPCWSTR moduleName, std::vector<intptr_t> offsets)
{
	MODULEINFO moduleinfo;
	if (!GetModuleInformation(GetCurrentProcess(), GetModuleHandle(moduleName), &moduleinfo, sizeof(moduleinfo)))
	{
		std::cout << "error: GetModuleInformation" << std::endl;
		return NULL;
	}

	auto addr = reinterpret_cast<uintptr_t>(moduleinfo.lpBaseOfDll);
	for (auto& offset : offsets)
		addr = *reinterpret_cast<uintptr_t*>(addr + offset);
	return addr;
}

CPlayerPed* FindPlayer()
{
	auto addr = TraverseOffsets(L"GTAIV.exe", { 0x014b6f0c, 0x00, 0x88, 0xB4 });
	return reinterpret_cast<CPlayerPed*>(addr);
};