#include "offsets.hpp"

#include <Psapi.h>

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

GPools* FindPools()
{
	auto addr = TraverseOffsets(L"GTAIV.exe", { 0x014b6f0c });
	return reinterpret_cast<GPools*>(addr - 0x100);
};