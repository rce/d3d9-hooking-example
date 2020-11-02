// Created with ReClass.NET 1.2 by KN4CK3R

#pragma once

#include "reclass.hpp"

class Vector
{
public:
	float mWestEast; //0x0000
	float mSouthNorth; //0x0004
	float mHeight; //0x0008
	float mUnknown; //0x000C
}; //Size: 0x0010
static_assert(sizeof(Vector) == 0x10);

class CPhysical
{
public:
	char pad_0000[56]; //0x0000
	class rage_phInstGta* m_phInstGta; //0x0038
	char pad_003C[84]; //0x003C
	class Vector mPhysicalPosition; //0x0090
	class Vector mVector2; //0x00A0
	class CInteriorInst* mInterior; //0x00B0
	class CPhysical* mSelf; //0x00B4
}; //Size: 0x00B8
static_assert(sizeof(CPhysical) == 0xB8);

class CPed : public CPhysical
{
public:
	char pad_00B8[160]; //0x00B8
	class CBuilding* mBuilding; //0x0158
	char pad_015C[96]; //0x015C
	class CBike* mVehicle; //0x01BC
	char pad_01C0[64]; //0x01C0
	class Vector mPosition; //0x0200
	char pad_0210[72]; //0x0210
	class CPed* mMePointerAgain; //0x0258
	char pad_025C[104]; //0x025C
	class CWeapon* mWeaponObject; //0x02C4
	char pad_02C8[40]; //0x02C8
	uint32_t mDeagleAmmo; //0x02F0
	char pad_02F4[8]; //0x02F4
	uint32_t mShotgunAmmo; //0x02FC
	char pad_0300[8]; //0x0300
	uint32_t mSmgAmmo; //0x0308
	char pad_030C[8]; //0x030C
	uint32_t mRifleAmmo; //0x0314
	char pad_0318[8]; //0x0318
	uint32_t mSniperAmmo; //0x0320
	char pad_0324[8]; //0x0324
	uint32_t mRocketAmmo; //0x032C
	char pad_0330[8]; //0x0330
	uint32_t mGrenadeAmmo; //0x0338
	char pad_033C[1084]; //0x033C
	bool mIsDrunk2; //0x0778
	bool mIsDrunk; //0x0779
	char pad_077A[10]; //0x077A
	class CPed* mYetAnotherSelfPointer; //0x0784
	char pad_0788[600]; //0x0788
	class Vector N00001580; //0x09E0
	char pad_09F0[320]; //0x09F0
	class CAutomobile* mLastVehicle; //0x0B30
	char pad_0B34[80]; //0x0B34
	float mArmor; //0x0B84
	char pad_0B88[464]; //0x0B88
	class CWeapon* N0000165D; //0x0D58
	char pad_0D5C[320]; //0x0D5C
	float mHealth; //0x0E9C
	char pad_0EA0[16]; //0x0EA0
}; //Size: 0x0EB0
static_assert(sizeof(CPed) == 0xEB0);

class CAutomobile : public CPhysical
{
public:
	char pad_00B8[312]; //0x00B8
	float mHealthMaybe; //0x01F0
	char pad_01F4[12]; //0x01F4
	class Vector N00001D21; //0x0200
	char pad_0210[64]; //0x0210
	class Vector N00001D32; //0x0250
	char pad_0260[2920]; //0x0260
	char* mVehicleName; //0x0DC8
	char pad_0DCC[2144]; //0x0DCC
	float mFrontLeftSuspensionHealth; //0x162C
	float mFrontLeftTireHealth; //0x1630
	char pad_1634[360]; //0x1634
	float mRearLeftSuspensionHealth; //0x179C
	float mRearLeftTireHealth; //0x17A0
	char pad_17A4[360]; //0x17A4
	float mFrontRightSuspensionHealth; //0x190C
	float mFrontRightTireHealth; //0x1910
	char pad_1914[360]; //0x1914
	float mRearRightSuspensionHealth; //0x1A7C
	float mRearRightTireHealth; //0x1A80
	char pad_1A84[1060]; //0x1A84
}; //Size: 0x1EA8
static_assert(sizeof(CAutomobile) == 0x1EA8);

class CBike : public CPhysical
{
public:
	char pad_00B8[1404]; //0x00B8
}; //Size: 0x0634
static_assert(sizeof(CBike) == 0x634);

class rage_phInstGta
{
public:
	class rage_phArchetypeGta* mArchetype; //0x0004
	char pad_0008[1084]; //0x0008

	virtual void Function0();
	virtual void Function1();
	virtual void Function2();
	virtual void Function3();
	virtual void Function4();
	virtual void Function5();
	virtual void Function6();
	virtual void Function7();
	virtual void Function8();
	virtual void Function9();
}; //Size: 0x0444
static_assert(sizeof(rage_phInstGta) == 0x444);

class rage_phArchetypeBase
{
public:

	virtual void Function0();
	virtual void Function1();
	virtual void Function2();
	virtual void Function3();
	virtual void Function4();
	virtual void Function5();
	virtual void Function6();
	virtual void Function7();
	virtual void Function8();
	virtual void Function9();
}; //Size: 0x0004
static_assert(sizeof(rage_phArchetypeBase) == 0x4);

class rage_phArchetype : public rage_phArchetypeBase
{
public:
}; //Size: 0x0004
static_assert(sizeof(rage_phArchetype) == 0x4);

class rage_phArchetypePhys : public rage_phArchetype
{
public:
}; //Size: 0x0004
static_assert(sizeof(rage_phArchetypePhys) == 0x4);

class rage_phArgehtypeDamp : public rage_phArchetypePhys
{
public:
}; //Size: 0x0004
static_assert(sizeof(rage_phArgehtypeDamp) == 0x4);

class rage_phArchetypeGta : public rage_phArgehtypeDamp
{
public:
	char pad_0004[64]; //0x0004
}; //Size: 0x0044
static_assert(sizeof(rage_phArchetypeGta) == 0x44);

class CWeapon : public CPhysical
{
public:
	char pad_00B8[3136]; //0x00B8
}; //Size: 0x0CF8
static_assert(sizeof(CWeapon) == 0xCF8);

class CVirtualBase
{
public:

	virtual void Function0();
	virtual void Function1();
	virtual void Function2();
	virtual void Function3();
	virtual void Function4();
	virtual void Function5();
	virtual void Function6();
	virtual void Function7();
	virtual void Function8();
	virtual void Function9();
}; //Size: 0x0004
static_assert(sizeof(CVirtualBase) == 0x4);

class CEntity : public CVirtualBase
{
public:
}; //Size: 0x0004
static_assert(sizeof(CEntity) == 0x4);

class CBuilding : public CEntity
{
public:
	char pad_0004[64]; //0x0004
}; //Size: 0x0044
static_assert(sizeof(CBuilding) == 0x44);

class CInteriorInst : public CBuilding
{
public:
	char pad_0044[320]; //0x0044
}; //Size: 0x0184
static_assert(sizeof(CInteriorInst) == 0x184);

class CExpensiveProcess
{
public:
	char pad_0000[24]; //0x0000
	class CPlayerPed* mArray[6]; //0x0018
	char pad_0030[44]; //0x0030
}; //Size: 0x005C
static_assert(sizeof(CExpensiveProcess) == 0x5C);

class N0000350D : public CExpensiveProcess
{
public:
}; //Size: 0x005C
static_assert(sizeof(N0000350D) == 0x5C);

class N0000350A : public N0000350D
{
public:
}; //Size: 0x005C
static_assert(sizeof(N0000350A) == 0x5C);

class CPedIntelligenceNY
{
public:
	char pad_0000[64]; //0x0000
	class CPlayerPed* mPlayerPed1; //0x0040
	char pad_0044[68]; //0x0044
	class CPlayerPed* mPlayerPed2; //0x0088
	char pad_008C[196]; //0x008C
	class N0000350A CPedScanner; //0x0150
	char pad_01AC[692]; //0x01AC
}; //Size: 0x0460
static_assert(sizeof(CPedIntelligenceNY) == 0x460);

class CPlayerPed : public CPed
{
public:
	char pad_0EB0[64]; //0x0EB0
}; //Size: 0x0EF0
static_assert(sizeof(CPlayerPed) == 0xEF0);

class CPool_CPickup
{
public:
	class CPickup(*mArray)[655]; //0x0000
	uint8_t(*mMask)[800]; //0x0004
	char pad_0008[4]; //0x0008
	uint32_t mItemSize; //0x000C
	char pad_0010[4]; //0x0010
	uint32_t mItemsInArray; //0x0014
	char pad_0018[8]; //0x0018
}; //Size: 0x0020
static_assert(sizeof(CPool_CPickup) == 0x20);

class CPool_CPedData
{
public:
	class CPedDataCop(*mArray)[120]; //0x0000
	char pad_0004[4]; //0x0004
	uint32_t mMax; //0x0008
	uint32_t mItemSize; //0x000C
	char pad_0010[16]; //0x0010
}; //Size: 0x0020
static_assert(sizeof(CPool_CPedData) == 0x20);

class CPool_CPed
{
public:
	char(*mArray)[1][4]; //0x0000
	bool(*N000023CB)[120]; //0x0004
	uint32_t mMax; //0x0008
	uint32_t mItemSize; //0x000C
	uint32_t mCount2; //0x0010
	uint32_t mCount; //0x0014
	char pad_0018[8]; //0x0018
}; //Size: 0x0020
static_assert(sizeof(CPool_CPed) == 0x20);

class GPool
{
public:
	char pad_0000[4]; //0x0000
	uint8_t(*mRefCounts)[1]; //0x0004
	char pad_0008[4]; //0x0008
	uint32_t mItemSize; //0x000C
	char pad_0010[4]; //0x0010
	uint32_t mItemsInArray; //0x0014
	char pad_0018[8]; //0x0018
}; //Size: 0x0020
static_assert(sizeof(GPool) == 0x20);

class CPool_CCamera
{
public:
	class CCamFinal(*N0000232F)[60]; //0x0000
	uint8_t(*mRefCounts)[1]; //0x0004
	uint32_t mMax; //0x0008
	uint32_t mItemSize; //0x000C
	uint32_t mCount2; //0x0010
	uint32_t mCount; //0x0014
	char pad_0018[8]; //0x0018
}; //Size: 0x0020
static_assert(sizeof(CPool_CCamera) == 0x20);

class GPoolsNotGeneric : public CPool_CPickup
{
public:
	class CPool_CPedData mPedDataPool; //0x0020
	class CPool_CPed mPedPool; //0x0040
	class GPool mUnknownPool_0x60; //0x0060
	class GPool mEntityPool; //0x0080
	class GPool N000018C2; //0x00A0
	class GPool N000018C3; //0x00C0
	class GPool N000018C4; //0x00E0
	class GPool N000018C5; //0x0100
	char pad_0120[5344]; //0x0120
	class CPool_CCamera mCamPool; //0x1600
	char pad_1620[824]; //0x1620
}; //Size: 0x1958
static_assert(sizeof(GPoolsNotGeneric) == 0x1958);

class N00001AEC
{
public:
	char pad_0000[4]; //0x0000
}; //Size: 0x0004
static_assert(sizeof(N00001AEC) == 0x4);

class N00001AEF
{
public:
	char pad_0000[4]; //0x0000
}; //Size: 0x0004
static_assert(sizeof(N00001AEF) == 0x4);

class N00001AF2
{
public:
	char pad_0000[4]; //0x0000
}; //Size: 0x0004
static_assert(sizeof(N00001AF2) == 0x4);

class N00001AF5
{
public:
	char pad_0000[4]; //0x0000
}; //Size: 0x0004
static_assert(sizeof(N00001AF5) == 0x4);

class N00001AFA
{
public:
	char pad_0000[4]; //0x0000
}; //Size: 0x0004
static_assert(sizeof(N00001AFA) == 0x4);

class N00001AFD
{
public:
	char pad_0000[4]; //0x0000
}; //Size: 0x0004
static_assert(sizeof(N00001AFD) == 0x4);

class N00001B00
{
public:
	char pad_0000[4]; //0x0000
}; //Size: 0x0004
static_assert(sizeof(N00001B00) == 0x4);

class N00001B03
{
public:
	char pad_0000[4]; //0x0000
}; //Size: 0x0004
static_assert(sizeof(N00001B03) == 0x4);

class N00001C27
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N00001C27) == 0x44);

class N00001C3B
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N00001C3B) == 0x44);

class Some_01
{
public:
	char pad_0000[40]; //0x0000
}; //Size: 0x0028
static_assert(sizeof(Some_01) == 0x28);

class N00001D72
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N00001D72) == 0x44);

class N00001DED
{
public:
	char pad_0000[4]; //0x0000
}; //Size: 0x0004
static_assert(sizeof(N00001DED) == 0x4);

class N00001E13
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N00001E13) == 0x44);

class N00001E6A
{
public:
	char pad_0000[4]; //0x0000
}; //Size: 0x0004
static_assert(sizeof(N00001E6A) == 0x4);

class N00001E90
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N00001E90) == 0x44);

class N00001ECA
{
public:
	char pad_0000[660]; //0x0000
}; //Size: 0x0294
static_assert(sizeof(N00001ECA) == 0x294);

class N00002214
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N00002214) == 0x44);

class CPedData
{
public:
	char pad_0000[4]; //0x0000
	uint32_t mType; //0x0004
}; //Size: 0x0008
static_assert(sizeof(CPedData) == 0x8);

class CPedDataNY : public CPedData
{
public:
}; //Size: 0x0008
static_assert(sizeof(CPedDataNY) == 0x8);

class CPedDataCop : public CPedDataNY
{
public:
	char pad_0008[88]; //0x0008
}; //Size: 0x0060
static_assert(sizeof(CPedDataCop) == 0x60);

class N000023DC
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N000023DC) == 0x44);

class N00002435
{
public:
	char pad_0000[4]; //0x0000
}; //Size: 0x0004
static_assert(sizeof(N00002435) == 0x4);

class N00002479
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N00002479) == 0x44);

class N000025DA
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N000025DA) == 0x44);

class CPickup : public Vector
{
public:
	char pad_0010[16]; //0x0010
	uint16_t mObjectId; //0x0020
	char pad_0022[2]; //0x0022
	uint32_t mType; //0x0024
}; //Size: 0x0028
static_assert(sizeof(CPickup) == 0x28);

class N000026EB
{
public:
	char pad_0000[4]; //0x0000
}; //Size: 0x0004
static_assert(sizeof(N000026EB) == 0x4);

class N00002946
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N00002946) == 0x44);

class N000016ED
{
public:
	char pad_0000[8256]; //0x0000
}; //Size: 0x2040
static_assert(sizeof(N000016ED) == 0x2040);

class N00002339
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N00002339) == 0x44);

class N00002394
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N00002394) == 0x44);

class CCam
{
public:
	char pad_0000[16]; //0x0000
	Vector4 N0000266F; //0x0010
	Vector4 N00002670; //0x0020
	Vector4 N00002671; //0x0030
	Vector4 mChangesWhenAiming; //0x0040
	Vector4 mZeroWhenAiming; //0x0050
	char pad_0060[20]; //0x0060
	float mZoomSpeed; //0x0074
	char pad_0078[28]; //0x0078
}; //Size: 0x0094
static_assert(sizeof(CCam) == 0x94);

class CCamFinal : public CCam
{
public:
	char pad_0094[144]; //0x0094
	class CCamGame* mCamGame; //0x0124
	char pad_0128[132]; //0x0128
	class CViewportGame* mViewport; //0x01AC
	char pad_01B0[848]; //0x01B0
}; //Size: 0x0500
static_assert(sizeof(CCamFinal) == 0x500);

class N00002533
{
public:
	char pad_0000[4]; //0x0000
}; //Size: 0x0004
static_assert(sizeof(N00002533) == 0x4);

class N00002536
{
public:
	char pad_0000[4]; //0x0000
}; //Size: 0x0004
static_assert(sizeof(N00002536) == 0x4);

class N00002539
{
public:
	char pad_0000[4]; //0x0000
}; //Size: 0x0004
static_assert(sizeof(N00002539) == 0x4);

class CCamGame : public CCam
{
public:
	char pad_0094[72]; //0x0094
	class CCamGame* mSelf; //0x00DC
	char pad_00E0[56]; //0x00E0
	class CCamFinal* mCamFinal; //0x0118
	char pad_011C[8]; //0x011C
	class CCamFollowPed* mCamFollowPedOrVehicle; //0x0124
	char pad_0128[72]; //0x0128
	Vector4 mCameraPosition; //0x0170
	char pad_0180[908]; //0x0180
}; //Size: 0x050C
static_assert(sizeof(CCamGame) == 0x50C);

class N0000256A
{
public:
	char pad_0000[4]; //0x0000
}; //Size: 0x0004
static_assert(sizeof(N0000256A) == 0x4);

class N0000269D
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N0000269D) == 0x44);

class N000026B4
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N000026B4) == 0x44);

class CCamFollowPed : public CCam
{
public:
	char pad_0094[200]; //0x0094
	class CPlayerPed* mPedToFollow; //0x015C
	Vector4 N00002700; //0x0160
	class CAutomobile* mVehicle; //0x0170
	char pad_0174[124]; //0x0174
	Vector4 N00002724; //0x01F0
	Vector4 N00002725; //0x0200
	Vector4 N00002726; //0x0210
	Vector4 N00002727; //0x0220
	Vector4 N00002728; //0x0230
	Vector4 N00002729; //0x0240
	Vector4 N0000272A; //0x0250
	Vector4 N0000272B; //0x0260
	char pad_0270[276]; //0x0270
	class CPlayerPed* mPedToFollow2; //0x0384
	char pad_0388[376]; //0x0388
}; //Size: 0x0500
static_assert(sizeof(CCamFollowPed) == 0x500);

class N000026DB
{
public:
	char pad_0000[4]; //0x0000
}; //Size: 0x0004
static_assert(sizeof(N000026DB) == 0x4);

class N000027EF
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N000027EF) == 0x44);

class N00002803
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N00002803) == 0x44);

class CViewportGame
{
public:
	char pad_0000[1092]; //0x0000
}; //Size: 0x0444
static_assert(sizeof(CViewportGame) == 0x444);

class N0000292E
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N0000292E) == 0x44);
