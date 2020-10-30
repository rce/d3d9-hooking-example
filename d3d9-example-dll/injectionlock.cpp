#include "injectionlock.hpp"

#include <Windows.h>
#include <iostream>

const int waitSleepMillis = 10;

InjectionLock::InjectionLock()
{
	this->hMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(LockState), L"NewInjectionHelper");
	DWORD dwLastError = GetLastError();
	if (this->hMapping == NULL) throw std::exception("error: CreateFileMapping");
	this->pLock = static_cast<LockState*>(MapViewOfFile(this->hMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, sizeof(LockState)));
	if (this->pLock == nullptr) throw std::exception("error: MapViewOfFile");
	if (dwLastError != ERROR_ALREADY_EXISTS)
	{
		std::cout << "Initializing lock" << std::endl;
		this->pLock->Locked = false;
		this->pLock->LockRequested = false;
	}

	this->Acquire();
}

InjectionLock::~InjectionLock()
{
	if (this->pLock->Locked) this->Release();
	if (this->pLock) UnmapViewOfFile(this->pLock);
	if (this->hMapping) CloseHandle(this->hMapping);
}

void InjectionLock::Acquire()
{
	std::cout << "Requesting for lock" << std::endl;
	this->pLock->LockRequested = true;
	std::cout << "Waiting for lock to be released" << std::endl;
	while (this->pLock->Locked) Sleep(waitSleepMillis);
	std::cout << "Acquiring lock" << std::endl;
	this->pLock->Locked = true;
	this->pLock->LockRequested = false;
}

void InjectionLock::WaitForLockRequest()
{
	std::cout << "Waiting for lock request" << std::endl;
	while (!this->pLock->LockRequested) Sleep(waitSleepMillis);
	std::cout << "Lock request received" << std::endl;
}

void InjectionLock::Release()
{
	std::cout << "Releasing lock" << std::endl;
	this->pLock->Locked = false;
}