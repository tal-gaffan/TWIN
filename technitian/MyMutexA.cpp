#include "MyMutexA.h"

// Implementation of MyMutexA class
// Constructor
MyMutexA::MyMutexA(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCSTR lpName) : m_hMutex(CreateMutexA(lpMutexAttributes, bInitialOwner, lpName)) {
	if (this->m_hMutex == NULL || GetLastError() == ERROR_ALREADY_EXISTS) {
		throw runtime_error(MUTEX_ERROR);
	}
}

// Destructor
MyMutexA::~MyMutexA() {
	ReleaseMutex(this->m_hMutex);
	CloseHandle(this->m_hMutex);
}

