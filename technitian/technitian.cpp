#include <windows.h>

#define INIT_MESSAGE "MANAGEMENT PROGRAM IS UP"
#define MANAGER_TITLE "MANAGEMENT PROGRAM"
#define INITIAL_MUTEX_NAME "InitialMutex"

constexpr int EXIT_CODE = 0;
// 1 hour = 60 minutes * 60 seconds * 1000 miliseconds = 3600000 miliseconds
constexpr int HOUR_MILLISECONDS = 3600000;

int main() {
	// Create Mutex (or return immidietly if Mutex already locked)
	HANDLE hInitMutex = CreateMutexA(NULL, true, INITIAL_MUTEX_NAME);

	if (hInitMutex == NULL) {
		return EXIT_CODE;
	}

	// Message box
	MessageBoxA(NULL, INIT_MESSAGE, MANAGER_TITLE, MB_OK);

	// Sleep 1 hour
	Sleep(HOUR_MILLISECONDS);

	// Release Mutex
	ReleaseMutex(hInitMutex);
	CloseHandle(hInitMutex);

	return EXIT_CODE;
}