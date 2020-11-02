#include <Windows.h>
#include <d3d9.h>
#include <DirectXMath.h>
#include <iostream>
#include <filesystem>

#pragma comment (lib, "d3d9.lib")

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

std::pair<int, int> gResolution = { 1280, 720 };
float aspectRatio() {
	return static_cast<float>(gResolution.first) / static_cast<float>(gResolution.second);
}

LPDIRECT3D9 g_pD3D;
LPDIRECT3DDEVICE9 g_pDevice;

#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)
struct Vertex { FLOAT x, y, z; DWORD color; };

LPDIRECT3DVERTEXBUFFER9 floorVertexBuffer;
Vertex floorVertices[] =
{
	{ -50.0f, 0.0f, -50.0f, D3DCOLOR_XRGB(55, 55, 55) },
	{ -50.0f, 0.0f,  50.0f, D3DCOLOR_XRGB(55, 55, 55) },
	{  50.0f, 0.0f, -50.0f, D3DCOLOR_XRGB(55, 55, 55) },

	{ -50.0f, 0.0f,  50.0f, D3DCOLOR_XRGB(55, 55, 55) },
	{  50.0f, 0.0f,  50.0f, D3DCOLOR_XRGB(55, 55, 55) },
	{  50.0f, 0.0f, -50.0f, D3DCOLOR_XRGB(55, 55, 55) },
};

class Cube
{
public:
	Cube()
	{
		VOID* pVoid;

		g_pDevice->CreateVertexBuffer(sizeof(m_vertices), 0, CUSTOMFVF, D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);
		m_pVertexBuffer->Lock(0, 0, &pVoid, 0);
		memcpy(pVoid, m_vertices, sizeof(m_vertices));
		m_pVertexBuffer->Unlock();

		g_pDevice->CreateIndexBuffer(sizeof(m_indices), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndexBuffer, NULL);
		m_pIndexBuffer->Lock(0, 0, &pVoid, 0);
		memcpy(pVoid, m_indices, sizeof(m_indices));
		m_pIndexBuffer->Unlock();
	}

	~Cube()
	{
		m_pVertexBuffer->Release();
		m_pIndexBuffer->Release();
	}

	void Render()
	{
		g_pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(Vertex));
		g_pDevice->SetIndices(m_pIndexBuffer);
		g_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, sizeof(m_vertices) / sizeof(Vertex), 0, 12);
	}

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;

	Vertex m_vertices[12] =
	{
		{ -3.0f, 3.0f, -3.0f, D3DCOLOR_XRGB(111, 111, 255) },
		{ 3.0f, 3.0f, -3.0f, D3DCOLOR_XRGB(111, 255, 111) },
		{ -3.0f, -3.0f, -3.0f, D3DCOLOR_XRGB(255, 111, 111) },
		{ 3.0f, -3.0f, -3.0f, D3DCOLOR_XRGB(111, 255, 255) },
		{ -3.0f, 3.0f, 3.0f, D3DCOLOR_XRGB(111, 111, 255) },
		{ 3.0f, 3.0f, 3.0f, D3DCOLOR_XRGB(255, 111, 111) },
		{ -3.0f, -3.0f, 3.0f, D3DCOLOR_XRGB(111, 255, 111) },
		{ 3.0f, -3.0f, 3.0f, D3DCOLOR_XRGB(111, 255, 255) },
	};

	short m_indices[36] =
	{
		0, 1, 2,    // side 1
		2, 1, 3,
		4, 0, 6,    // side 2
		6, 0, 2,
		7, 5, 6,    // side 3
		6, 5, 4,
		3, 1, 7,    // side 4
		7, 1, 5,
		4, 5, 0,    // side 5
		0, 5, 1,
		3, 7, 2,    // side 6
		2, 7, 6,
	};
};

Cube* g_pCube = nullptr;

void initGraphics()
{
	VOID* pVoid;

	g_pDevice->CreateVertexBuffer(sizeof(floorVertices), 0, CUSTOMFVF, D3DPOOL_MANAGED, &floorVertexBuffer, NULL);
	floorVertexBuffer->Lock(0, 0, &pVoid, 0);
	memcpy(pVoid, floorVertices, sizeof(floorVertices));
	floorVertexBuffer->Unlock();

	g_pCube = new Cube();
}

HRESULT SetTransform(IDirect3DDevice9* pDevice, D3DTRANSFORMSTATETYPE State, DirectX::XMMATRIX matrix)
{
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMStoreFloat4x4(&mat, matrix);
	return pDevice->SetTransform(State, reinterpret_cast<D3DMATRIX*>(&mat));
}

void renderFloor()
{
	SetTransform(g_pDevice, D3DTS_WORLD, DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f));
	g_pDevice->SetStreamSource(0, floorVertexBuffer, 0, sizeof(Vertex));
	g_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
}


void renderObject(DirectX::XMVECTOR position)
{
	auto pos = DirectX::XMMatrixTranslation(
		DirectX::XMVectorGetX(position),
		DirectX::XMVectorGetY(position),
		DirectX::XMVectorGetZ(position));

	static float rotation = 0.0f;
	rotation += 0.05f;

	auto t = DirectX::XMMatrixRotationY(rotation) * pos;
	SetTransform(g_pDevice, D3DTS_WORLD, t);

	g_pCube->Render();
}

void initD3D(HWND hWnd)
{
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferWidth = gResolution.first;
	d3dpp.BackBufferHeight = gResolution.second;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pDevice);

	g_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	initGraphics();
}

void setCamera(DirectX::XMVECTOR lookAt)
{
	SetTransform(g_pDevice, D3DTS_VIEW, DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(0.0f, 5.0f, 10.0f, 1.0f),
		lookAt,
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f)
	));

	SetTransform(g_pDevice, D3DTS_PROJECTION, DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(110), aspectRatio(), 1.0f, 100.0f));
}

void render()
{
	g_pDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
	g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
	g_pDevice->BeginScene();

	g_pDevice->SetFVF(CUSTOMFVF);

	static auto objectPosition = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	static auto objectPosition2 = DirectX::XMVectorSet(2.0f, 0.0f, -5.0f, 0.0f);

	renderFloor();

	objectPosition = DirectX::XMVectorSet(
		DirectX::XMVectorGetX(objectPosition),
		DirectX::XMVectorGetY(objectPosition) + 0.01f,
		DirectX::XMVectorGetZ(objectPosition),
		DirectX::XMVectorGetW(objectPosition));
	renderObject(objectPosition);

	objectPosition2 = DirectX::XMVectorSet(
		DirectX::XMVectorGetX(objectPosition2),
		DirectX::XMVectorGetY(objectPosition2) + 0.03f,
		DirectX::XMVectorGetZ(objectPosition2),
		DirectX::XMVectorGetW(objectPosition2));
	renderObject(objectPosition2);

	setCamera(objectPosition);

	g_pDevice->EndScene();
	g_pDevice->Present(NULL, NULL, NULL, NULL);
}

void cleanupD3D()
{
	delete g_pCube;
	floorVertexBuffer->Release();
	g_pDevice->Release();
	g_pD3D->Release();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass1";
	RegisterClassEx(&wc);

	HWND hWnd = CreateWindowEx(NULL, wc.lpszClassName, L"d3d9-example-target", WS_OVERLAPPEDWINDOW, 1200, 300, gResolution.first, gResolution.second, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	initD3D(hWnd);

	MSG msg;
	while (true)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT) break;

		render();
	}

	cleanupD3D();
	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}