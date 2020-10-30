#pragma once

#include <Windows.h>


struct LockState
{
	bool Locked;
	bool LockRequested;
};

class InjectionLock
{
public:
	InjectionLock();
	~InjectionLock();
	void WaitForLockRequest();

private:
	void Acquire();
	void Release();

	HANDLE hMapping;
	LockState* pLock;
};