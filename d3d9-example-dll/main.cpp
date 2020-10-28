#include <Windows.h>
#include <iostream>
#include <d3d9.h>
#pragma comment (lib, "d3d9.lib")

#define HOOKMETHOD_CREATEDEVICE 0x01
#define HOOKMETHOD HOOKMETHOD_CREATEDEVICE

void printError(HRESULT hr);

#if HOOKMETHOD == HOOKMETHOD_CREATEDEVICE
HWND GetDummyWindowHandle()
{
	std::cout << "Using HWNDMETHOD_CREATEWINDOW" << std::endl;

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
#endif

typedef uintptr_t* vftable_t;
struct ObjectWithVftable {
	vftable_t vftable;
};

uintptr_t* copyVftable(ObjectWithVftable* pObj)
{
	size_t len = 0;
	while (pObj->vftable[len] != 0) len++;
	std::cout << "Found " << len << " members in vftable" << std::endl;
	auto vftable = new uintptr_t[len + 1];
	memcpy(vftable, pObj->vftable, (len + 1) * sizeof(uintptr_t*));
	return vftable;
}

uintptr_t* findVftable()
{
#if HOOKMETHOD == HOOKMETHOD_CREATEDEVICE
	std::cout << "Using HOOKMETHOD_CREATEDEVICE" << std::endl;

	HWND hWnd = GetDummyWindowHandle();
	if (hWnd)
	{
		std::cout << "Found the following handle: " <<  hWnd << std::endl;
		IDirect3D9 *dummy_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
		if (dummy_pD3D)
		{
			D3DPRESENT_PARAMETERS d3dpp;
			ZeroMemory(&d3dpp, sizeof(d3dpp));
			d3dpp.Windowed = TRUE;
			d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
			d3dpp.hDeviceWindow = hWnd;

			IDirect3DDevice9 *dummy_pDevice = nullptr;
			HRESULT hrCreateDevice = dummy_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &dummy_pDevice);
			if (SUCCEEDED(hrCreateDevice))
			{
				std::cout << "pDevice == " << dummy_pDevice << std::endl;
				auto vftable = copyVftable(reinterpret_cast<ObjectWithVftable*>(dummy_pDevice));
				if (dummy_pDevice) dummy_pDevice->Release();
				return vftable;
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

	return nullptr;
#else
	std::cout << "LOL asdf" << std::endl;
	return nullptr;
#endif
}

void hookEndScene()
{
	auto vftable = findVftable();
	if (!vftable)
	{
		std::cout << "error: findVftable" << std::endl;
		return;
	}

	for (int i = 0; vftable[i]; ++i)
	{
		std::cout << std::hex << vftable[i] << std::endl;
	}
	delete vftable;
}

void initConsole();

DWORD WINAPI MainThread(LPVOID lpThreadParameter)
{
	HMODULE hModule = static_cast<HMODULE>(lpThreadParameter);

	initConsole();
	try
	{
		hookEndScene();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	FreeLibraryAndExitThread(hModule, 0);
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
		CloseHandle(CreateThread(NULL, 0, MainThread, hModule, 0, NULL));
	}
	return TRUE;
}

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
#undef ERROR_MESSAGE_BUFFER_SIZE
}