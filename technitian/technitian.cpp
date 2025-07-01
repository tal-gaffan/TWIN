#include <windows.h>

#define INIT_MESSAGE "MANAGEMENT PROGRAM IS UP"
#define MANAGER_TITLE "MANAGEMENT_PROGRAM"
#define INITIAL_MUTEX_NAME "InitialMutex"
#define STARTUP_REGISTERY "Software\\Microsoft\\Windows\\CurrentVersion\\Run"

constexpr int SUCCESS_CODE = 0;
constexpr int FAIL_CODE = 1;
constexpr int HOUR_MILLISECONDS = 60 * 60 * 1000;
// minutes * seconds * milliseconds

/*
Adds a file to the startup registery given the file's path.

@param path - the path of the file to add. 

@return SUCCESS_CODE if added successfuly.
*/
int addToStartupRegistery(CHAR* path) {
	HKEY hRegisteryKey;
	int exitValue = RegOpenKeyExW(HKEY_CURRENT_USER, (LPCWSTR)STARTUP_REGISTERY, 0, KEY_SET_VALUE, &hRegisteryKey);

	if (exitValue != ERROR_SUCCESS) {
		return FAIL_CODE;
	}

	size_t pathLength = (strlen(path) + 1) * sizeof(TCHAR);
	RegSetValueExW(hRegisteryKey, (LPCWSTR)MANAGER_TITLE, 0, REG_SZ, (BYTE*)path, pathLength);
	RegCloseKey(hRegisteryKey);

	return SUCCESS_CODE;
}

int main() {
	// Create Mutex (or return immidietly if Mutex already locked)
	HANDLE hInitMutex = CreateMutexA(NULL, true, INITIAL_MUTEX_NAME);

	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		return SUCCESS_CODE;
	}
	
	// Get currently running .exe path
	CHAR currentExePath[MAX_PATH];
	GetModuleFileNameW(NULL, (WCHAR*)currentExePath, MAX_PATH);

	// Add current .exe to startup registery
	int exitValue = addToStartupRegistery(currentExePath);

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

	// Release Mutex
	ReleaseMutex(hInitMutex);
	CloseHandle(hInitMutex);

	return SUCCESS_CODE;
}