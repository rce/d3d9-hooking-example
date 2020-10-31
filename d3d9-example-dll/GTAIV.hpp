#pragma once

// Created with ReClass.NET 1.2 by KN4CK3R

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
	char pad_0004[52]; //0x0004
	class rage_phInstGta* m_phInstGta; //0x0038
	char pad_003C[84]; //0x003C
	class Vector mVector1; //0x0090
	class Vector mVector2; //0x00A0
	class CInteriorInst* mInterior; //0x00B0
	class CPhysical* mSelf; //0x00B4

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
	char pad_0D5C[324]; //0x0D5C
	float mHealth; //0x0EA0
	char pad_0EA4[3488]; //0x0EA4
}; //Size: 0x1C44
static_assert(sizeof(CPed) == 0x1C44);

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

class N00001E9F
{
public:
	char pad_0000[4]; //0x0000
}; //Size: 0x0004
static_assert(sizeof(N00001E9F) == 0x4);

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

class CPedIKManager
{
public:
	char pad_0000[132]; //0x0000
}; //Size: 0x0084
static_assert(sizeof(CPedIKManager) == 0x84);

class CWeapon : public CPhysical
{
public:
	char pad_00B8[3136]; //0x00B8
}; //Size: 0x0CF8
static_assert(sizeof(CWeapon) == 0xCF8);

class N00002C3E
{
public:
	char pad_0000[4]; //0x0000
}; //Size: 0x0004
static_assert(sizeof(N00002C3E) == 0x4);

class N00003098
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N00003098) == 0x44);

class N000030AE
{
public:
	char pad_0000[132]; //0x0000
}; //Size: 0x0084
static_assert(sizeof(N000030AE) == 0x84);

class N000030EC
{
public:
	char pad_0000[132]; //0x0000
}; //Size: 0x0084
static_assert(sizeof(N000030EC) == 0x84);

class N0000312A
{
public:
	char pad_0000[4]; //0x0000
}; //Size: 0x0004
static_assert(sizeof(N0000312A) == 0x4);

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

class N00003214
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N00003214) == 0x44);

class N00003256
{
public:
	char pad_0000[64]; //0x0000
}; //Size: 0x0040
static_assert(sizeof(N00003256) == 0x40);

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
	char pad_01AC[2028]; //0x01AC
}; //Size: 0x0998
static_assert(sizeof(CPedIntelligenceNY) == 0x998);

class N000032BA
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N000032BA) == 0x44);

class N000032D4
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N000032D4) == 0x44);

class N000034F7
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N000034F7) == 0x44);

class CPlayerPed : public CPed
{
public:
	char pad_1C44[64]; //0x1C44
}; //Size: 0x1C84
static_assert(sizeof(CPlayerPed) == 0x1C84);

class N00003579
{
public:
	char pad_0000[4]; //0x0000
}; //Size: 0x0004
static_assert(sizeof(N00003579) == 0x4);

class N0000357D
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N0000357D) == 0x44);

class N000035B5
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N000035B5) == 0x44);

class N0000378D
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
static_assert(sizeof(N0000378D) == 0x44);
