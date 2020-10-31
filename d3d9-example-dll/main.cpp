#include <Windows.h>
#include <iostream>

#include <d3d9.h>
#pragma comment (lib, "d3d9.lib")

#define HOOKMETHOD_DUMMYDEVICE 0x01
#define HOOKMETHOD HOOKMETHOD_DUMMYDEVICE

#include "hook.hpp"
#include "injectionlock.hpp"
#if HOOKMETHOD == HOOKMETHOD_DUMMYDEVICE
#include "dummydevice.hpp"
#endif

#include "imguioverlay.hpp"
LPCWSTR gWindowName = L"d3d9-example-target";

void RenderOverlay(IDirect3DDevice9* pDevice)
{
	imguioverlay::EndScene(pDevice, gWindowName);
	if (!imguioverlay::ReadyToRender()) return;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	ImGui::SetNextWindowPos(ImVec2(10, 50));
	ImGui::SetNextWindowSize(ImVec2(200, 200));
	ImGui::SetNextWindowBgAlpha(0.3f);

	ImGui::Begin("Overlay");
	ImGui::Text("Hello, world!");
	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

typedef HRESULT (__stdcall *EndScene_t)(IDirect3DDevice9* pDevice);
EndScene_t originalEndScene = nullptr;
HRESULT __stdcall hookEndScene(IDirect3DDevice9* pDevice)
{
	RenderOverlay(pDevice);
	return originalEndScene(pDevice);
}

void initConsole();

DWORD WINAPI MainThread(LPVOID lpThreadParameter)
{
	HMODULE hModule = static_cast<HMODULE>(lpThreadParameter);
	initConsole();

	InjectionLock lock{};

	auto addrEndScene = FindEndScene();
	if (addrEndScene)
	{
		originalEndScene = HookWithTrampoline<EndScene_t>(hookEndScene, addrEndScene, 7);
	}
	else
	{
		std::cout << "error: findEndScene" << std::endl;
	}

	lock.WaitForLockRequest();

	imguioverlay::Cleanup();
	RevertHookWithTrampoline(originalEndScene, addrEndScene, 7);
	originalEndScene = nullptr;

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