#pragma once

#include <Windows.h>
#include <cstdint>

const size_t jmpSize = 5;

void write_jmp(uintptr_t dest, uintptr_t src)
{
	*reinterpret_cast<uint8_t*>(src) = 0xE9;
	*reinterpret_cast<intptr_t*>(src + 1) = dest - src - 5;
}

template <typename Function>
Function HookWithTrampoline(Function hook, uintptr_t target, size_t trampolineSize)
{
	DWORD oldProtect;

	auto trampoline = new uint8_t[trampolineSize + jmpSize];
	memcpy(trampoline, reinterpret_cast<void*>(target), trampolineSize);
	write_jmp(target + trampolineSize, reinterpret_cast<uintptr_t>(trampoline) + trampolineSize);
	VirtualProtect(trampoline, trampolineSize + jmpSize, PAGE_EXECUTE_READWRITE, &oldProtect);


	VirtualProtect(reinterpret_cast<void*>(target), trampolineSize, PAGE_EXECUTE_READWRITE, &oldProtect);
	write_jmp(reinterpret_cast<uintptr_t>(hook), target);
	VirtualProtect(reinterpret_cast<void*>(target), trampolineSize, oldProtect, &oldProtect);

	return reinterpret_cast<Function>(trampoline);
}