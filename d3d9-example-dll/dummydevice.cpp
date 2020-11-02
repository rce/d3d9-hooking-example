#include "dummydevice.hpp"

#include "offsets.hpp"

#include <Windows.h>
#include <d3d9.h>
#include <iostream>


void printError(HRESULT hr)
{
	const size_t ERROR_MESSAGE_BUFFER_SIZE = 2048;
	CHAR buf[ERROR_MESSAGE_BUFFER_SIZE];

	if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, hr, 0, (LPTSTR)&buf, ERROR_MESSAGE_BUFFER_SIZE, NULL))
	{
		std::cout << "error: " << buf << " (" << hr << ")" << std::endl;
	}
	else
	{
		std::cout << "error: Error code did not exist in system errors" << std::endl;
	}
}

HWND GetDummyWindowHandle()
{
	HINSTANCE hInstance = NULL;
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = DefWindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"Dummy";
	RegisterClassEx(&wc);

	HWND hWnd = CreateWindowEx(NULL, wc.lpszClassName, L"Dummy", WS_OVERLAPPEDWINDOW, 300, 300, 500, 400, NULL, NULL, hInstance, NULL);
	return hWnd;
}

D3DAddresses FindEndScene()
{
	HWND hWnd = GetDummyWindowHandle();
	if (hWnd)
	{
		std::cout << "Found the following handle: " << hWnd << std::endl;
		IDirect3D9* dummy_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
		if (dummy_pD3D)
		{
			D3DPRESENT_PARAMETERS d3dpp;
			ZeroMemory(&d3dpp, sizeof(d3dpp));
			d3dpp.Windowed = TRUE;
			d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
			d3dpp.hDeviceWindow = hWnd;

			IDirect3DDevice9* dummy_pDevice = nullptr;
			HRESULT hrCreateDevice = dummy_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &dummy_pDevice);
			if (SUCCEEDED(hrCreateDevice))
			{
				std::cout << "pDevice == " << dummy_pDevice << std::endl;
				auto vmt = *reinterpret_cast<uintptr_t**>(dummy_pDevice);
				std::cout << "vmt == " << vmt << std::endl;
				D3DAddresses result{
					vmt[offsetEndScene / sizeof(uintptr_t)],
					vmt[offsetDrawIndexedPrimitive / sizeof(uintptr_t)]
				};
				std::cout << "EndScene == " << std::hex << result.addrEndScene << std::endl;
				std::cout << "DrawIndexedPrimitive == " << std::hex << result.addrDrawIndexedPrimitive << std::endl;
				if (dummy_pDevice) dummy_pDevice->Release();
				return result;
			}
			else
			{
				std::cout << "error: IDirect3DDevice9::CreateDevice(...)" << std::endl;
				printError(hrCreateDevice);
			}

			if (dummy_pD3D) dummy_pD3D->Release();
		}
		else
		{
			std::cout << "error: Direct3DCreate" << std::endl;
		}

		if (hWnd) CloseWindow(hWnd);
	}

	return { 0, 0 };
}