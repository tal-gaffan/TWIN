#include "MyRegisteryKey.h"

// Implementation of MyRegisteryKey class
MyRegisteryKey::MyRegisteryKey(HKEY hKey, LPCWSTR lbSubKey, DWORD ulOptions, REGSAM samDesired) {
	int exitValue = RegOpenKeyExW(HKEY_CURRENT_USER, lbSubKey, 0, KEY_SET_VALUE, &this->m_hRegisteryKey);

	if (exitValue != ERROR_SUCCESS) {
		throw runtime_error(REG_KEY_ERROR);
	}
}

// SetValue
void MyRegisteryKey::SetValue(LPCWSTR lpValueName, DWORD Reserved, DWORD dwType, const BYTE* lpData, DWORD cbData) {
	int exitValue = RegSetValueExW(this->m_hRegisteryKey, lpValueName, Reserved, dwType, lpData, cbData);

	if (exitValue != ERROR_SUCCESS) {
		throw runtime_error(REG_KEY_ERROR);
	}
}

// Deconstructor
MyRegisteryKey::~MyRegisteryKey() {
	RegCloseKey(this->m_hRegisteryKey);
}