#include <Windows.h>
#include <iostream>

#include <d3d9.h>
#pragma comment (lib, "d3d9.lib")

#include "hook.hpp"
#include "injectionlock.hpp"
#include "dummydevice.hpp"

#include "offsets.hpp"
#include "imguioverlay.hpp"

LPCWSTR gWindowName = L"GTAIV";

CPlayerPed* gPlayer = nullptr;

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

	ImGui::Begin("Information");
	if (gPlayer)
	{
		ImGui::Text("Health: %3.1f/200.0", gPlayer->mHealth);
		ImGui::Text("Armor:  %3.1f/100.0", gPlayer->mArmor);

		if (gPlayer->mLastVehicle)
		{
			ImGui::Text("Latest vehicle: %s", gPlayer->mLastVehicle->mVehicleName);
		}
		else
		{
			ImGui::Text("Latest vehicle: N/A");
		}
	}
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
	gPlayer = FindPlayer();
	std::cout << "gPlayer == " << std::hex << gPlayer << std::endl;

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
	gPlayer = nullptr;

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