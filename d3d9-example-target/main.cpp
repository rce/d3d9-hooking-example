#include <Windows.h>
#include <d3d9.h>
#include <DirectXMath.h>
#include <iostream>
#include <filesystem>
#include <algorithm>

#pragma comment (lib, "d3d9.lib")

bool g_bRenderDebugLines = true;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

std::pair<int, int> gResolution = { 1280, 720 };
float aspectRatio() {
	return static_cast<float>(gResolution.first) / static_cast<float>(gResolution.second);
}

LPDIRECT3D9 g_pD3D;
LPDIRECT3DDEVICE9 g_pDevice;

#define CUSTOMFVF (D3DFVF_XYZ  | D3DFVF_NORMAL)
struct Vertex { FLOAT x, y, z; D3DVECTOR normal; };

HRESULT SetTransform(IDirect3DDevice9* pDevice, D3DTRANSFORMSTATETYPE State, DirectX::XMMATRIX matrix)
{
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMStoreFloat4x4(&mat, matrix);
	return pDevice->SetTransform(State, reinterpret_cast<D3DMATRIX*>(&mat));
}

const D3DCOLORVALUE COLOR_RED = { 1.0f, 0.0f, 0.0f, 1.0f };
const D3DCOLORVALUE COLOR_GREEN = { 0.0f, 1.0f, 0.0f, 1.0f };
const D3DCOLORVALUE COLOR_BLUE = { 0.0f, 0.0f, 1.0f, 1.0f };
const D3DCOLORVALUE COLOR_CYAN = { 0.0f, 1.0f, 1.0f, 1.0f };

void drawLine(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b, D3DCOLORVALUE color)
{
	Vertex vertices[2] =
	{
		{ a.x, a.y, a.z, 1.0f, 0.0f, 0.0f, },
		{ b.x, b.y, b.z, 1.0f, 0.0f, 0.0f, },
	};

	D3DMATERIAL9 material{};
	material.Diffuse = color;
	material.Ambient = color;
	g_pDevice->SetMaterial(&material);

	SetTransform(g_pDevice, D3DTS_WORLD, DirectX::XMMatrixIdentity());
	DWORD oldZENABLE;
	g_pDevice->GetRenderState(D3DRS_ZENABLE, &oldZENABLE);
	g_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	g_pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, vertices, sizeof(Vertex));
	g_pDevice->SetRenderState(D3DRS_ZENABLE, oldZENABLE);
}

void drawLine(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b)
{
	drawLine(a, b, COLOR_RED);
}

struct KeyState
{
	bool w, a, s, d;
	bool q, l;
};

struct MouseState
{
	bool mouse1, mouse2, mouse3;
	int posX, posY;
	int wheelX, wheelY;
};

KeyState g_previousKeyState{};
KeyState g_keyState{};

MouseState g_previousMouseState{};
MouseState g_mouseState{};

LPDIRECT3DVERTEXBUFFER9 floorVertexBuffer;
Vertex floorVertices[] =
{
	{ -50.0f, 0.0f, -50.0f, 0.0f, 1.0f, 0.0f },
	{ -50.0f, 0.0f,  50.0f, 0.0f, 1.0f, 0.0f },
	{  50.0f, 0.0f, -50.0f, 0.0f, 1.0f, 0.0f },

	{ -50.0f, 0.0f,  50.0f, 0.0f, 1.0f, 0.0f },
	{  50.0f, 0.0f,  50.0f, 0.0f, 1.0f, 0.0f },
	{  50.0f, 0.0f, -50.0f, 0.0f, 1.0f, 0.0f },
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
		D3DMATERIAL9 material{};
		material.Diffuse = D3DCOLORVALUE{ 1.0f, 0.0f, 0.0f, 1.0f };
		material.Ambient = D3DCOLORVALUE{ 1.0f, 0.0f, 0.0f, 1.0f };
		g_pDevice->SetMaterial(&material);

		g_pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(Vertex));
		g_pDevice->SetIndices(m_pIndexBuffer);
		g_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, sizeof(m_vertices) / sizeof(Vertex), 0, 12);
	}

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;

	Vertex m_vertices[24] =
	{
		{ -3.0f, -3.0f, 3.0f, 0.0f, 0.0f, 1.0f, },    // side 1
		{ 3.0f, -3.0f, 3.0f, 0.0f, 0.0f, 1.0f, },
		{ -3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 1.0f, },
		{ 3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 1.0f, },

		{ -3.0f, -3.0f, -3.0f, 0.0f, 0.0f, -1.0f, },    // side 2
		{ -3.0f, 3.0f, -3.0f, 0.0f, 0.0f, -1.0f, },
		{ 3.0f, -3.0f, -3.0f, 0.0f, 0.0f, -1.0f, },
		{ 3.0f, 3.0f, -3.0f, 0.0f, 0.0f, -1.0f, },

		{ -3.0f, 3.0f, -3.0f, 0.0f, 1.0f, 0.0f, },    // side 3
		{ -3.0f, 3.0f, 3.0f, 0.0f, 1.0f, 0.0f, },
		{ 3.0f, 3.0f, -3.0f, 0.0f, 1.0f, 0.0f, },
		{ 3.0f, 3.0f, 3.0f, 0.0f, 1.0f, 0.0f, },

		{ -3.0f, -3.0f, -3.0f, 0.0f, -1.0f, 0.0f, },    // side 4
		{ 3.0f, -3.0f, -3.0f, 0.0f, -1.0f, 0.0f, },
		{ -3.0f, -3.0f, 3.0f, 0.0f, -1.0f, 0.0f, },
		{ 3.0f, -3.0f, 3.0f, 0.0f, -1.0f, 0.0f, },

		{ 3.0f, -3.0f, -3.0f, 1.0f, 0.0f, 0.0f, },    // side 5
		{ 3.0f, 3.0f, -3.0f, 1.0f, 0.0f, 0.0f, },
		{ 3.0f, -3.0f, 3.0f, 1.0f, 0.0f, 0.0f, },
		{ 3.0f, 3.0f, 3.0f, 1.0f, 0.0f, 0.0f, },

		{ -3.0f, -3.0f, -3.0f, -1.0f, 0.0f, 0.0f, },    // side 6
		{ -3.0f, -3.0f, 3.0f, -1.0f, 0.0f, 0.0f, },
		{ -3.0f, 3.0f, -3.0f, -1.0f, 0.0f, 0.0f, },
		{ -3.0f, 3.0f, 3.0f, -1.0f, 0.0f, 0.0f, },

	};

	short m_indices[36] =
	{
		0, 1, 2,    // side 1
		2, 1, 3,
		4, 5, 6,    // side 2
		6, 5, 7,
		8, 9, 10,    // side 3
		10, 9, 11,
		12, 13, 14,    // side 4
		14, 13, 15,
		16, 17, 18,    // side 5
		18, 17, 19,
		20, 21, 22,    // side 6
		22, 21, 23,

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

void initLight()
{
	D3DLIGHT9 light{};
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse = D3DCOLORVALUE{ 0.5f, 0.5f, 0.5f, 1.0f };
	light.Direction = D3DVECTOR{ -1.0f, -0.3f, -1.0f };

	g_pDevice->SetLight(0, &light);
	g_pDevice->LightEnable(0, TRUE);
}

void renderFloor()
{
	D3DMATERIAL9 material{};
	material.Diffuse = D3DCOLORVALUE{ 0.0f, 1.0f, 0.0f, 1.0f };
	material.Ambient = D3DCOLORVALUE{ 1.0f, 1.0f, 1.0f, 1.0f };
	g_pDevice->SetMaterial(&material);

	SetTransform(g_pDevice, D3DTS_WORLD, DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f));
	g_pDevice->SetStreamSource(0, floorVertexBuffer, 0, sizeof(Vertex));
	g_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
}

class Entity
{
public:
	Entity(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity, float rotationVelocity) : m_pModel(g_pCube), m_position(position), m_velocity(velocity), m_rotY(0.0f), m_rotationVelocity(rotationVelocity)
	{
	}

	virtual void Update()
	{
		DirectX::XMStoreFloat3(&m_position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_position), DirectX::XMLoadFloat3(&m_velocity)));
		m_rotY += m_rotationVelocity;
	}

	virtual void Render()
	{
		SetTransform(g_pDevice, D3DTS_WORLD, DirectX::XMMatrixRotationY(m_rotY) * DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z));
		m_pModel->Render();

		if (g_bRenderDebugLines)
		{
			RenderVelocityLine();
			RenderHeadingLine();
		}
	}

	~Entity()
	{
	}

	DirectX::XMFLOAT3 GetPosition() { return m_position; }

protected:
	DirectX::XMVECTOR Heading()
	{
		DirectX::XMFLOAT3 vector(0.0f, 0.0f, 1.0f);
		auto r = -m_rotY;
		return DirectX::XMVector3Normalize(DirectX::XMVectorSet(
			(cos(r) * vector.x) - (sin(r) * vector.z),
			vector.y,
			(sin(r) * vector.x) + (cos(r) * vector.z),
			0.0f
		));
	}

	DirectX::XMFLOAT3 m_velocity;
	float m_rotY, m_rotationVelocity;

private:
	void RenderVelocityLine()
	{
		DirectX::XMFLOAT3 vector;
		DirectX::XMStoreFloat3(&vector, DirectX::XMVectorAdd(
			DirectX::XMLoadFloat3(&m_position),
			DirectX::XMVectorScale(DirectX::XMLoadFloat3(&m_velocity), 15.0f)
		));
		drawLine(m_position, vector, COLOR_CYAN);
	}
	void RenderHeadingLine()
	{
		//auto heading = DirectX::XMVectorSet(0.0f, m_rotation, 0.0f, 0.0f);
		// drawLine m_position (mPosition + scale(normalize(heading), 15.0f))
		auto heading = Heading();
		DirectX::XMFLOAT3 vector;
		DirectX::XMStoreFloat3(&vector, DirectX::XMVectorAdd(
			DirectX::XMLoadFloat3(&m_position),
			DirectX::XMVectorScale(DirectX::XMVector3Normalize(heading), 15.0f)
		));
		drawLine(m_position, vector, COLOR_BLUE);
	}

	Cube* m_pModel;
	DirectX::XMFLOAT3 m_position;
};

class Player : public Entity
{
public:
	Player() : Entity({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 0.0f)
	{
	}

	void Update()
	{
		Entity::Update();

		auto diffX = g_mouseState.posX - g_previousMouseState.posX;
		m_rotY = m_rotY + static_cast<float>(diffX) / 100.0f;


		auto vel = DirectX::XMVector3Normalize(DirectX::XMVectorSet(
			0.0f + (g_keyState.a ? -1.0f : 0.0f) + (g_keyState.d ? 1.0f : 0.0f),
			0.0f,
			0.0f + (g_keyState.w ? 1.0f : 0.0f) + (g_keyState.s ? -1.0f : 0.0f),
			0.0f
		));

		float speed = 0.5f;
		DirectX::XMStoreFloat3(&m_velocity, DirectX::XMVector3Rotate(
			DirectX::XMVectorScale(vel, speed),
			DirectX::XMQuaternionRotationRollPitchYaw(0.0f, m_rotY, 0.0f)
		));
	}

	void Render()
	{
		Entity::Render();
	}
};

Player *g_pPlayer;
std::vector<Entity*> g_entities;

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

	g_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 50));

	initGraphics();
	initLight();
}

class Camera
{
public:
	Camera(Entity *pLookAtEntity) :
		m_pLookAtEntity(pLookAtEntity)
	{
		m_position = CalculateCamPosition();
	}

	DirectX::XMFLOAT3 CalculateCamPosition()
	{
		auto camOffset = DirectX::XMVectorScale(DirectX::XMVector3Rotate(
			DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
			DirectX::XMQuaternionRotationRollPitchYaw(m_rotX, m_rotY, 0.0f)
		), m_distance);

		auto lookAt = DirectX::XMLoadFloat3(&m_pLookAtEntity->GetPosition());

		DirectX::XMFLOAT3 result;
		DirectX::XMStoreFloat3(&result, DirectX::XMVectorAdd(lookAt, camOffset));
		return result;
	}
	void Update()
	{
		auto diffX = g_mouseState.posX - g_previousMouseState.posX;
		auto diffY = g_mouseState.posY - g_previousMouseState.posY;

		m_rotX = std::clamp(m_rotX - static_cast<float>(diffY) / 100.0f, -1.5f, 1.5f);
		m_rotY = m_rotY + static_cast<float>(diffX) / 100.0f;

		auto diffWheel = g_mouseState.wheelY - g_previousMouseState.wheelY;
		m_distance = std::clamp(m_distance + (-diffWheel / 100.0f), 1.0f, 100.0f);

		m_position = CalculateCamPosition();
	}

	void Render()
	{
		SetTransform(g_pDevice, D3DTS_VIEW, DirectX::XMMatrixLookAtLH(
			DirectX::XMLoadFloat3(&m_position),
			DirectX::XMLoadFloat3(&m_pLookAtEntity->GetPosition()),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f)
		));

		SetTransform(g_pDevice, D3DTS_PROJECTION, DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(75), aspectRatio(), 1.0f, 100.0f));
	}

private:
	DirectX::XMFLOAT3 m_position;
	Entity* m_pLookAtEntity;
	float m_distance = 20.0;
	float m_rotX = -1.0f;
	float m_rotY = DirectX::XMConvertToRadians(180);
};

Camera* g_pCamera;

void update()
{
	if (g_keyState.l && !g_previousKeyState.l)
		g_bRenderDebugLines = !g_bRenderDebugLines;

	for (auto& e : g_entities)
		e->Update();

	g_pCamera->Update();

	g_previousKeyState = g_keyState;
	g_previousMouseState = g_mouseState;
}

void render()
{
	g_pDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
	g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
	g_pDevice->BeginScene();

	g_pDevice->SetFVF(CUSTOMFVF);

	renderFloor();

	for (auto& e : g_entities)
		e->Render();

	if (g_bRenderDebugLines)
		for (auto& e : g_entities)
			if (e != g_pPlayer)
				drawLine(g_pPlayer->GetPosition(), e->GetPosition());

	g_pCamera->Render();

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

bool registerRawInput()
{
	RAWINPUTDEVICE Rid[] =
	{
		{0x01, 0x02, RIDEV_NOLEGACY, 0},
		{0x01, 0x06, RIDEV_NOLEGACY, 0}
	};

	return RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) != FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	AllocConsole();
	FILE *f;
	freopen_s(&f, "CONIN$", "r", stdin);
	freopen_s(&f, "CONOUT$", "w", stdout);
	freopen_s(&f, "CONOUT$", "w", stderr);

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

	g_pPlayer = new Player();
	g_entities.push_back(g_pPlayer);
	g_entities.push_back(new Entity({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.005f, 0.0f }, -0.02f));
	g_entities.push_back(new Entity({ 2.0f, 0.0f, -5.0f }, { 0.0f, 0.04f, 0.0f }, 0.05f));
	g_pCamera = new Camera(g_pPlayer);

	auto path = std::string("../Debug/d3d9-example-dll.dll");
	if (std::filesystem::exists(path))
	{
		auto dll = std::filesystem::canonical(path).wstring();
		std::wcout << dll << std::endl;
		LoadLibrary(dll.c_str());
	}

	MSG msg;

	bool quit = false;

	if (!registerRawInput())
	{
		std::cout << "Failed to register for raw input devices" << std::endl;
		quit = true;
	}

	while (!quit)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT || g_keyState.q)
				quit = true;
		}

		update();
		render();
	}

	g_pPlayer = nullptr;
	g_entities.erase(std::remove_if(g_entities.begin(), g_entities.end(),
		[](Entity* pEntity) { delete pEntity; return true; }));
	delete g_pCamera;

	cleanupD3D();
	return msg.wParam;
}

// https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
void updateKeyState(WPARAM keyCode, bool state)
{
		switch (keyCode)
		{
		case 'W': g_keyState.w = state; break;
		case 'A': g_keyState.a = state; break;
		case 'S': g_keyState.s = state; break;
		case 'D': g_keyState.d = state; break;
		case 'Q': g_keyState.q = state; break;
		case 'L': g_keyState.l = state; break;
		}

}

#define HasFlag(value, flag) ((value & flag) == flag)

void HandleRawInput(HRAWINPUT lParam)
{
	UINT dwSize;
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
	auto lpb = std::make_unique<BYTE[]>(dwSize);

	if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb.get(), &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
		std::cout << "GetRAwInputData does not return correct size!" << std::endl;

	RAWINPUT* raw = (RAWINPUT*)lpb.get();
	if (raw->header.dwType == RIM_TYPEKEYBOARD)
	{
		if (raw->data.keyboard.Message == WM_KEYDOWN)
		{
			updateKeyState(raw->data.keyboard.VKey, true);
		}
		else if (raw->data.keyboard.Message == WM_KEYUP)
		{
			updateKeyState(raw->data.keyboard.VKey, false);
		}
	}
	else if (raw->header.dwType == RIM_TYPEMOUSE)
	{
		if (HasFlag(raw->data.mouse.usFlags, MOUSE_MOVE_RELATIVE))
		{
			g_mouseState.posX += raw->data.mouse.lLastX;
			g_mouseState.posY += raw->data.mouse.lLastY;
		}
		else if (HasFlag(raw->data.mouse.usFlags, MOUSE_MOVE_ABSOLUTE))
		{
			// MOUSE_MOVE_ABSOLUTE not handled
		}
		else if (raw->data.mouse.ulButtons == RI_MOUSE_BUTTON_1_DOWN)
		{
			g_mouseState.mouse1 = true;
		}
		else if (raw->data.mouse.ulButtons == RI_MOUSE_BUTTON_1_UP)
		{
			g_mouseState.mouse1 = false;
		}
		else if (raw->data.mouse.ulButtons == RI_MOUSE_BUTTON_2_DOWN)
		{
			g_mouseState.mouse2 = true;
		}
		else if (raw->data.mouse.ulButtons == RI_MOUSE_BUTTON_2_UP)
		{
			g_mouseState.mouse2 = false;
		}
		else if (raw->data.mouse.ulButtons == RI_MOUSE_BUTTON_3_DOWN)
		{
			g_mouseState.mouse3 = true;
		}
		else if (raw->data.mouse.ulButtons == RI_MOUSE_BUTTON_3_UP)
		{
			g_mouseState.mouse3 = false;
		}

		if (HasFlag(raw->data.mouse.usButtonFlags, RI_MOUSE_WHEEL))
		{
			auto delta = static_cast<float>(static_cast<short>(raw->data.mouse.usButtonData));
			g_mouseState.wheelY += delta;
		}
		if (HasFlag(raw->data.mouse.usButtonFlags, RI_MOUSE_HWHEEL))
		{
			auto delta = static_cast<float>(static_cast<short>(raw->data.mouse.usButtonData));
			g_mouseState.wheelX = raw->data.mouse.usButtonData;
		}
	}
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INPUT:
		if (GET_RAWINPUT_CODE_WPARAM(wParam) == RIM_INPUT)
		{
			HandleRawInput((HRAWINPUT)lParam);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}