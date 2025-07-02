#include <windows.h>
#include <stdexcept>

#define REG_KEY_ERROR "Error with the registery key"

using namespace std;

// Wrapper for registery key with constructor and destructor.
class MyRegisteryKey {
public:
	// Constructor
	MyRegisteryKey(HKEY hKey, LPCWSTR lbSubKey, DWORD ulOptions, REGSAM samDesired);

	// SetValue
	void SetValue(LPCWSTR lpValueName, DWORD Reserved, DWORD dwType, const BYTE* lpData, DWORD cbData);

	// Deconstructor
	~MyRegisteryKey();

private:
	HKEY m_hRegisteryKey;
};
