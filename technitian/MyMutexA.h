#include <stdexcept>
#include <windows.h>

#define MUTEX_ERROR "Error with the mutex"

using namespace std;

// Wrapper for mutex with constructor and destructor.
class MyMutexA {
public:
	MyMutexA(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCSTR lpName);

	~MyMutexA();

private:
	HANDLE m_hMutex;
};
