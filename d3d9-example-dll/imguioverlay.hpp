#pragma once

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include <d3d9.h>

//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace imguioverlay {
	HWND g_hWnd;
	bool g_ImGui_Initialized = false;
	bool g_ImGui_Closing = false;

	//WNDPROC oWndProc = nullptr;
	//LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	//	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	//		return true;
	//	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
	//}

	void EndScene(IDirect3DDevice9* pDevice, LPCWSTR windowName)
	{
		if (!g_ImGui_Initialized)
		{
			g_hWnd = FindWindow(NULL, windowName);
			//oWndProc = (WNDPROC)SetWindowLongPtr(g_hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);

			ImGui::CreateContext();

			ImGui_ImplWin32_Init(g_hWnd);
			ImGui_ImplDX9_Init(pDevice);
			g_ImGui_Initialized = true;
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
			//SetWindowLongPtr(g_hWnd, GWLP_WNDPROC, (LONG_PTR)oWndProc);

			ImGui_ImplDX9_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();
			g_ImGui_Initialized = false;
			g_ImGui_Closing = false;
		}
	}
}