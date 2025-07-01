#include <windows.h>
#include <tchar.h>

#define INIT_MESSAGE "MANAGEMENT PROGRAM IS UP"
#define MANAGER_TITLE "MANAGEMENT_PROGRAM"
#define INITIAL_MUTEX_NAME "InitialMutex"
#define STARTUP_REGISTERY "Software\\Microsoft\\Windows\\CurrentVersion\\Run"

constexpr int SUCCESS_CODE = 0;
constexpr int FAIL_CODE = 1;
constexpr int HOUR_MILLISECONDS = 60 * 60 * 1000;
// minutes * seconds * milliseconds


// Wrapper for mutex with constructor and destructor.
class MyMutexA {
public:
	HANDLE hMutex;

	// Constructor
	MyMutexA(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCSTR lpName) : hMutex(CreateMutexA(lpMutexAttributes, bInitialOwner, lpName)) {}

	// Destructor
	~MyMutexA() {
		ReleaseMutex(this->hMutex);
		CloseHandle(this->hMutex);
	}
};

// Wrapper for registery key with constructor and destructor.
class MyRegisteryKey {
public:
	HKEY hRegisteryKey;

	// Constructor
	MyRegisteryKey(HKEY hKey, LPCWSTR lbSubKey, DWORD ulOptions, REGSAM samDesired) : hRegisteryKey(NULL) {
		RegOpenKeyExW(HKEY_CURRENT_USER, (LPCWSTR)STARTUP_REGISTERY, 0, KEY_SET_VALUE, &this->hRegisteryKey);
	}
	
	// Deconstructor
	~MyRegisteryKey() {
		RegCloseKey(this->hRegisteryKey);
	}
};

/*
Adds a file to the startup registery given the file's path.

@param path - the path of the file to add. 

@return SUCCESS_CODE if added successfuly.
*/
int addToStartupRegistery(WCHAR* path) {
	MyRegisteryKey startupKeyRegistery(HKEY_CURRENT_USER, (LPCWSTR)STARTUP_REGISTERY, 0, KEY_SET_VALUE);

	size_t pathLength = (_tcslen(path) + 1) * sizeof(WCHAR);
	int exitValue = RegSetValueExW(startupKeyRegistery.hRegisteryKey, (LPCWSTR)MANAGER_TITLE, 0, REG_SZ, (BYTE*)path, pathLength);
	
	if (exitValue != ERROR_SUCCESS) {
		return FAIL_CODE;
	}

	return SUCCESS_CODE;
}

int main() {
	// Create Mutex (or return immidietly if Mutex already locked)
	MyMutexA initialMutex(NULL, true, INITIAL_MUTEX_NAME);

	if (initialMutex.hMutex == NULL) {
		return FAIL_CODE;
	} else if (GetLastError() == ERROR_ALREADY_EXISTS) {
		return SUCCESS_CODE;
	}
	
	// Get currently running .exe path
	WCHAR currentExePath[MAX_PATH];
	int exitValue = GetModuleFileNameW(NULL, currentExePath, MAX_PATH);

	if (exitValue == 0) {
		return exitValue;
	}

	// Add current .exe to startup registery
	exitValue = addToStartupRegistery(currentExePath);

	if (exitValue != SUCCESS_CODE) {
		return exitValue;
	}

	// Message box
	exitValue = MessageBoxA(NULL, INIT_MESSAGE, MANAGER_TITLE, MB_OK);

	if (exitValue != IDOK) {
		return FAIL_CODE;
	}

	// Sleep 1 hour
	Sleep(HOUR_MILLISECONDS);

	return SUCCESS_CODE;
}