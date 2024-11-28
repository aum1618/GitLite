#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <Windows.h>
#include <wincrypt.h>
#pragma comment(lib, "Crypt32.lib")

std::string sha256(const std::string& input) {
	// Initialize variables
	HCRYPTPROV hCryptProv = NULL;
	HCRYPTHASH hHash = NULL;
	BYTE rgbHash[32];
	DWORD cbHash = 32;
	std::stringstream ss;

	// Acquire a cryptographic provider
	if (!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
		std::cerr << "CryptAcquireContext failed" << std::endl;
		return "";
	}

	// Create a hash object
	if (!CryptCreateHash(hCryptProv, CALG_SHA_256, 0, 0, &hHash)) {
		std::cerr << "CryptCreateHash failed" << std::endl;
		CryptReleaseContext(hCryptProv, 0);
		return "";
	}

	// Hash the input data
	if (!CryptHashData(hHash, reinterpret_cast<const BYTE*>(input.c_str()), input.length(), 0)) {
		std::cerr << "CryptHashData failed" << std::endl;
		CryptDestroyHash(hHash);
		CryptReleaseContext(hCryptProv, 0);
		return "";
	}

	// Get the hash value
	if (!CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0)) {
		std::cerr << "CryptGetHashParam failed" << std::endl;
		CryptDestroyHash(hHash);
		CryptReleaseContext(hCryptProv, 0);
		return "";
	}

	// Convert to hexadecimal string
	for (DWORD i = 0; i < cbHash; i++) {
		ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(rgbHash[i]);
	}

	// Clean up
	CryptDestroyHash(hHash);
	CryptReleaseContext(hCryptProv, 0);

	return ss.str();
}



int instructorHash(int number) {
	int hash = 1;
	while (number > 0) {
		hash *= number % 10;
		number /= 10;
	}
	return hash % 29;
}

int instructorHash(const std::string& str) {
	long long hash = 1;
	for (int i = 0; i < str.length(); i++) {
		hash *= str[i];
	}
	return hash % 29;
}