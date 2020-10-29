#include <Windows.h>
#include <iostream>
#include <d3d9.h>
#pragma comment (lib, "d3d9.lib")

#define HOOKMETHOD_DUMMYDEVICE 0x01
#define HOOKMETHOD HOOKMETHOD_DUMMYDEVICE

#include "hook.hpp"
#if HOOKMETHOD == HOOKMETHOD_DUMMYDEVICE
#include "dummydevice.hpp"
#endif

typedef HRESULT (__stdcall *EndScene_t)(IDirect3DDevice9* pDevice);
EndScene_t originalEndScene = nullptr;
HRESULT __stdcall hookEndScene(IDirect3DDevice9* pDevice)
{
	std::cout << "hookEndScene(" << std::hex << pDevice << ")" << std::endl;
	// Clear the whole window to bright green so it's obvious we are hooked and can draw
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 255, 0), 1.0f, 0);
	return originalEndScene(pDevice);
}

void initConsole();

DWORD WINAPI MainThread(LPVOID lpThreadParameter)
{
	HMODULE hModule = static_cast<HMODULE>(lpThreadParameter);

	initConsole();
	auto addrEndScene = FindEndScene();
	if (addrEndScene)
	{
		originalEndScene = HookWithTrampoline<EndScene_t>(hookEndScene, addrEndScene, 7);
	}
	else
	{
		std::cout << "error: findEndScene" << std::endl;
	}

	// TODO Remove hook

	return 0;
}

void initConsole()
{
	if (GetConsoleWindow() == NULL)
	{
		AllocConsole();
		FILE* f;
		freopen_s(&f, "CONOUT$", "w", stdout);
		freopen_s(&f, "CONOUT$", "w", stderr);
		std::cout.clear();
		std::cin.clear();
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		HANDLE hThread = CreateThread(NULL, 0, MainThread, hModule, 0, NULL);
		if (hThread) CloseHandle(hThread);
	}
	return TRUE;
}