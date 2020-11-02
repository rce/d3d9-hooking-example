#include <Windows.h>
#include <iostream>

#include <d3d9.h>
#include <DirectXMath.h>
#pragma comment (lib, "d3d9.lib")

#include "hook.hpp"
#include "injectionlock.hpp"
#include "dummydevice.hpp"


#include "offsets.hpp"
#include "imguioverlay.hpp"

#define PRINT(expr) std::cout << #expr << " == " << expr << std::endl;

LPCWSTR gWindowName = L"GTAIV";

GPools* gPools = nullptr;
CPlayerPed* gPlayer = nullptr;

void DrawCrosshair(IDirect3DDevice9 *pDevice)
{
	D3DVIEWPORT9 viewport;
	pDevice->GetViewport(&viewport);
	int x = viewport.Width / 2;
	int y = viewport.Height / 2;
	D3DCOLOR color = gPlayer ? D3DCOLOR_XRGB(0, 255, 0) : D3DCOLOR_XRGB(255, 0, 0);

	D3DRECT vaaka = { x - 10, y - 1, x + 10, y + 1 };
	pDevice->Clear(1, &vaaka, D3DCLEAR_TARGET, color, 0, 0);
	D3DRECT pysty = { x - 1, y - 10, x + 1, y + 10 };
	pDevice->Clear(1, &pysty, D3DCLEAR_TARGET, color, 0, 0);
}

template <typename T>
void PoolStats(const char *name, CPool<T> &pool)
{
	size_t withRefs = 0;
	for (size_t i = 0; i < pool.mMax; ++i)
		if (pool.mRefCounts[i] & 1) ++withRefs;
	const char* ltgt = pool.mCount == pool.mCount2 ? "=" : pool.mCount < pool.mCount2 ? "<" : ">";
	ImGui::Text("CPool<%8s> %3d %s %4d %4d (%d)", name, pool.mCount, ltgt, pool.mCount2, pool.mMax, withRefs);
}


DirectX::XMFLOAT3 ConvertVector(Vector& v) { return { v.mWestEast, v.mSouthNorth, v.mHeight }; }
DirectX::XMFLOAT3 ConvertVector2(Vector& v) { return { v.mWestEast, v.mHeight, v.mSouthNorth }; }
DirectX::XMFLOAT3 ConvertVector3(Vector& v) { return { v.mSouthNorth, v.mWestEast, v.mHeight }; }
DirectX::XMFLOAT3 ConvertVector4(Vector& v) { return { v.mSouthNorth, v.mHeight, v.mWestEast }; }
DirectX::XMFLOAT3 ConvertVector5(Vector& v) { return { v.mHeight, v.mWestEast, v.mSouthNorth }; }
DirectX::XMFLOAT3 ConvertVector6(Vector& v) { return { v.mHeight, v.mSouthNorth, v.mWestEast }; }

void RenderOverlay(IDirect3DDevice9* pDevice)
{
	imguioverlay::EndScene(pDevice, gWindowName);
	if (!imguioverlay::ReadyToRender()) return;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	ImGui::SetNextWindowPos(ImVec2(10, 50));
	ImGui::SetNextWindowSize(ImVec2(300, 500));
	ImGui::SetNextWindowBgAlpha(0.3f);


	ImGui::Begin("Information");
	
	if (gPools)
	{
		//ImGui::TextWrapped("Information about game's internal object pools");
		if (!ImGui::CollapsingHeader("Pools"))
		{	
			ImGui::Text("CPool<T>          ?    ?  Max");

			PoolStats("CPickup", gPools->mPickups);
			PoolStats("CPedData", gPools->mPedData);
			PoolStats("CPed2?", gPools->mPedPool);
			PoolStats("CPed3?", gPools->mUnknownPool_0x60);
			PoolStats("prepare?", gPools->mEntityPool);
			PoolStats("CProcess", gPools->mProcessPool_0xA0);
			PoolStats("CTask?", gPools->mTaskPool_0xC0);
			PoolStats("CVehicle", gPools->mBigEntityPool_0xE0);
			PoolStats("CPedInt", gPools->mPedIntelligencePool);
		}


		size_t pigeons = 0, type0 = 0, type1 = 0, type2 = 0, type3 = 0, typeOther = 0, withRefs = 0;
		for (size_t i = 0; i < gPools->mPickups.mCount; ++i)
		{
			if (!gPools->mPickups.mRefCounts[i]) continue;
			++withRefs;
			CPickup* pPickup = &gPools->mPickups.mArray[i];
			const uint16_t PigeonObjectId = 0x08DC;
			if (pPickup->mType == 0x00) ++type0;
			else if (pPickup->mType == 0x01) ++type1;
			else if (pPickup->mType == 0x02) ++type2;
			else if (pPickup->mType == 0x03) ++type3;
			else ++typeOther;

			if (pPickup->mObjectId == PigeonObjectId)
				++pigeons;
		}

		if (!ImGui::CollapsingHeader("CPickups"))
		{
			ImGui::TextWrapped("withRefs:   %d", withRefs);
			ImGui::TextWrapped("mType 0x00: %d", type0);
			ImGui::TextWrapped("mType 0x01: %d", type1);
			ImGui::TextWrapped("mType 0x02: %d", type2);
			ImGui::TextWrapped("mType 0x03: %d", type3);
			ImGui::TextWrapped("other:      %d", typeOther);
			ImGui::TextWrapped("Pigeons nearby:    %d", pigeons);
		}
	}

	if (gPlayer && !ImGui::CollapsingHeader("CPlayerPed"))
	{
		ImGui::Text("Health: %3.1f/200.0", gPlayer->mHealth);
		ImGui::Text("Armor:  %3.1f/100.0", gPlayer->mArmor);

		if (gPlayer->mLastVehicle)
		{
			ImGui::Text("Latest vehicle: %s", gPlayer->mLastVehicle->mVehicleName);
			auto a = ConvertVector(gPlayer->mPhysicalPosition), b = ConvertVector(gPlayer->mLastVehicle->mPhysicalPosition);
			auto aVec = DirectX::XMVectorSet(a.x, a.y, a.z, 0.0f);
			auto bVec = DirectX::XMVectorSet(b.x, b.y, b.z, 0.0f);
			float distance;
			DirectX::XMStoreFloat(&distance, DirectX::XMVector3Length(DirectX::XMVectorSubtract(aVec, bVec)));
			ImGui::Text("Distance: %.1f", distance);
			//std::cout << "Distance to mLastVehicle == " << distance << std::endl;

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

#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)
struct CUSTOMVERTEX { FLOAT x, y, z; DWORD color; };	
IDirect3DDevice9* g_pDevice = nullptr;

HRESULT SetTransform(IDirect3DDevice9* pDevice, D3DTRANSFORMSTATETYPE State, DirectX::XMMATRIX matrix)
{
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMStoreFloat4x4(&mat, matrix);
	return pDevice->SetTransform(State, reinterpret_cast<D3DMATRIX*>(&mat));
}

typedef HRESULT(WINAPI* DrawIndexedPrimitive_t)(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
DrawIndexedPrimitive_t originalDrawIndexedPrimitive = nullptr;
HRESULT WINAPI hookDrawIndexedPrimitive(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	if (primCount > 10)
	{
		pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		auto result = originalDrawIndexedPrimitive(pDevice, PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
		pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	}
	auto result = originalDrawIndexedPrimitive(pDevice, PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	return result;
}

DirectX::XMVECTOR WorldToScreen(IDirect3DDevice9* pDevice, DirectX::XMVECTOR pos)
{
	D3DVIEWPORT9 viewport;
	D3DMATRIX view, projection;
	pDevice->GetViewport(&viewport);
	pDevice->GetTransform(D3DTS_VIEW, &view);
	pDevice->GetTransform(D3DTS_PROJECTION, &projection);

	auto viewMatrix = DirectX::XMLoadFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&view));
	auto projectionMatrix = DirectX::XMLoadFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&projection));
	return DirectX::XMVector3Project(pos, viewport.X, viewport.Y, viewport.Width, viewport.Height, viewport.MinZ, viewport.MaxZ, projectionMatrix, viewMatrix, DirectX::XMMatrixIdentity());
}

void DrawLine(IDirect3DDevice9 *pDevice, DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b)
{
	//std::cout << rand() << std::endl;
	auto g_pd3dDevice = pDevice;
	CUSTOMVERTEX vertices[] =
	{
		{ a.x, a.y, a.z, D3DCOLOR_XRGB(255, 0, 0) },
		{ b.x, b.y, b.z, D3DCOLOR_XRGB(255, 0, 0) },
	};


	auto screenpos = WorldToScreen(pDevice, DirectX::XMLoadFloat3(&b));
	std::cout << "WorldToScreen(...) -> "
		<< DirectX::XMVectorGetX(screenpos) << ", "
		<< DirectX::XMVectorGetY(screenpos) << ", "
		<< DirectX::XMVectorGetZ(screenpos) << std::endl;
	// Backup the DX9 state
	//IDirect3DStateBlock9* d3d9_state_block = NULL;
	//pDevice->CreateStateBlock(D3DSBT_ALL, &d3d9_state_block);
	//D3DMATRIX last_world, last_view, last_projection;
	//pDevice->GetTransform(D3DTS_WORLD, &last_world);
	//pDevice->GetTransform(D3DTS_VIEW, &last_view);
	//pDevice->GetTransform(D3DTS_PROJECTION, &last_projection);
	DWORD oldFVF;
	pDevice->GetFVF(&oldFVF);

	pDevice->SetVertexShader(NULL);
	pDevice->SetPixelShader(NULL);

	pDevice->SetFVF(CUSTOMFVF);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//SetTransform(g_pDevice, D3DTS_WORLD, DirectX::XMMatrixIdentity());
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertices, sizeof(CUSTOMVERTEX));

	pDevice->SetFVF(oldFVF);


	//// Restore the DX9 state
	//pDevice->SetTransform(D3DTS_WORLD, &last_world);
	//pDevice->SetTransform(D3DTS_VIEW, &last_view);
	//pDevice->SetTransform(D3DTS_PROJECTION, &last_projection);
	//d3d9_state_block->Apply();
	//d3d9_state_block->Release();
}


typedef HRESULT (__stdcall *EndScene_t)(IDirect3DDevice9* pDevice);
EndScene_t originalEndScene = nullptr;
HRESULT __stdcall hookEndScene(IDirect3DDevice9* pDevice)
{
	g_pDevice = pDevice;

	if (gPlayer && !gPlayer->mVehicle)
		DrawCrosshair(pDevice);

	if (gPlayer && gPlayer->mLastVehicle)
	{
		std::cout << gPlayer->mPhysicalPosition.mWestEast << std::endl;
		std::cout << gPlayer->mLastVehicle->mPhysicalPosition.mWestEast << std::endl;
		DrawLine(pDevice, ConvertVector(gPlayer->mPhysicalPosition), ConvertVector(gPlayer->mLastVehicle->mPhysicalPosition));
		//DrawLine(pDevice, ConvertVector2(gPlayer->mPhysicalPosition), ConvertVector2(gPlayer->mLastVehicle->mPhysicalPosition));
		//DrawLine(pDevice, ConvertVector3(gPlayer->mPhysicalPosition), ConvertVector3(gPlayer->mLastVehicle->mPhysicalPosition));
		//DrawLine(pDevice, ConvertVector4(gPlayer->mPhysicalPosition), ConvertVector4(gPlayer->mLastVehicle->mPhysicalPosition));
		//DrawLine(pDevice, ConvertVector5(gPlayer->mPhysicalPosition), ConvertVector5(gPlayer->mLastVehicle->mPhysicalPosition));
		//DrawLine(pDevice, ConvertVector6(gPlayer->mPhysicalPosition), ConvertVector6(gPlayer->mLastVehicle->mPhysicalPosition));
	}
	RenderOverlay(pDevice);

	if (gPlayer)
	{
		if (gPlayer->mHealth < 120.0f)
			gPlayer->mHealth = 200.0f;
		if (gPlayer->mArmor < 10.0f)
			gPlayer->mArmor = 100.0f;
	}

	return originalEndScene(pDevice);
}

void initConsole();

DWORD WINAPI MainThread(LPVOID lpThreadParameter)
{
	HMODULE hModule = static_cast<HMODULE>(lpThreadParameter);
	initConsole();

	InjectionLock lock{};

	gPools = FindPools();
	std::cout << "gPools == " << std::hex << gPools << std::endl;

	gPlayer = FindPlayer();
	std::cout << "gPlayer == " << std::hex << gPlayer << std::endl;

	auto addrs = FindEndScene();
	if (addrs.addrEndScene && addrs.addrDrawIndexedPrimitive)
	{
		originalEndScene = HookWithTrampoline<EndScene_t>(hookEndScene, addrs.addrEndScene, 7);
		//originalDrawIndexedPrimitive = HookWithTrampoline<DrawIndexedPrimitive_t>(hookDrawIndexedPrimitive, addrs.addrDrawIndexedPrimitive, 5);
	}
	else
	{
		std::cout << "error: findEndScene" << std::endl;
	}

	lock.WaitForLockRequest();


	imguioverlay::Cleanup();
	if (originalEndScene)
	{
		RevertHookWithTrampoline(originalEndScene, addrs.addrEndScene, 7);
		originalEndScene = nullptr;
	}
	if (originalDrawIndexedPrimitive)
	{
		RevertHookWithTrampoline(originalDrawIndexedPrimitive, addrs.addrDrawIndexedPrimitive, 5);
		originalDrawIndexedPrimitive = nullptr;
	}
	gPlayer = nullptr;
	g_pDevice = nullptr;

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