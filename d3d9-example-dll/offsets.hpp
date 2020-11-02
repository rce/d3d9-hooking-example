#pragma once

#include "GTAIV.hpp"

#include <Windows.h>
#include <vector>
#include <iostream>

// These offsets have to be reverse engineered by hand. These might or
// might not be the same over different applications and Direct3D9 versions.
const intptr_t offsetPresent = 0x44;
const intptr_t offsetBeginScene = 0xA4;
const intptr_t offsetEndScene = 0xA8;
const intptr_t offsetDrawIndexedPrimitive = 82;
const intptr_t offsetClear = 0xAC;


const uint32_t CPedDataType_Normal = 0x01;
const uint32_t CPedDataType_NY = 0x00;
const uint32_t CPedDataType_Cop = 0x02;

template <typename T>
class CPool
{
public:
	T* mArray;
	uint8_t* mRefCounts;
	uint32_t mMax;
	uint32_t mItemSize;
	uint32_t mCount2;
	uint32_t mCount;
	uint32_t mUnknown_0x18;
	uint32_t mUnknown_0x1c;
};

class GPools
{
public:
	// Always seems to have 655 items
	CPool<CPickup> mPickups;
	
	// Around people these go to around 51/52
	// 1, 1, 1 when far at sea
	CPool<CPedData> mPedData;
	CPool<CPlayerPed> mPedPool;
	CPool<uint8_t> mUnknownPool_0x60; // this tends to be +1 from above

	// 0 far at sea
	CPool<CEntity> mEntityPool; // This pops full at first until the pedestrian pools above get full :thinkging:
	// 0 far at sea
	CPool<uint8_t> mProcessPool_0xA0; // This is often at 0
	// 3 far at sea
	CPool<uint8_t> mTaskPool_0xC0; // Near pedestrians this pops to ~200

	// Vehicles? This is <10 at Happiness Island which could indicate boats + helicopter
	// 1 far at sea when sinking a helicopter
	CPool<CEntity> mBigEntityPool_0xE0;
	// 1 far at sea
	CPool<CPedIntelligenceNY> mPedIntelligencePool;
};

uintptr_t TraverseOffsets(LPCWSTR moduleName, std::vector<intptr_t> offsets);

CPlayerPed* FindPlayer();
GPools* FindPools();