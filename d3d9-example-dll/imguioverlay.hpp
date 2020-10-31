#pragma once

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include <d3d9.h>

namespace imguioverlay {
	HWND g_hWnd;
	bool g_ImGui_Initialized = false;
	bool g_ImGui_Closing = false;

	void EndScene(IDirect3DDevice9* pDevice, LPCWSTR windowName)
	{
		if (!g_ImGui_Initialized)
		{
			g_hWnd = FindWindow(NULL, windowName);

			ImGui::CreateContext();

			ImGui_ImplWin32_Init(g_hWnd);
			ImGui_ImplDX9_Init(pDevice);
			g_ImGui_Initialized = true;
			g_ImGui_Closing = false;
		}
	}

	bool ReadyToRender()
	{
		return g_ImGui_Initialized && !g_ImGui_Closing;
	}


	void Cleanup()
	{
		if (g_ImGui_Initialized)
		{
			g_ImGui_Closing = true;
			Sleep(100);

			ImGui_ImplDX9_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();
			g_ImGui_Initialized = false;
			g_ImGui_Closing = false;
		}
	}
}