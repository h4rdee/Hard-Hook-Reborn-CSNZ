#pragma once
#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;

extern void __cdecl Log(const char* fmt, ...);

extern HANDLE processHandle;

class Memory
{
public:
	template <typename Type, typename Base, typename Offset>
	static inline Type Ptr(Base base, Offset offset)
	{
		static_assert(std::is_pointer<Type>::value || std::is_integral<Type>::value, "Type must be a pointer or address");
		static_assert(std::is_pointer<Base>::value || std::is_integral<Base>::value, "Base must be a pointer or address");
		static_assert(std::is_pointer<Offset>::value || std::is_integral<Offset>::value, "Offset must be a pointer or address");

		return base ? reinterpret_cast<Type>((reinterpret_cast<uint64_t>(base) + static_cast<uint64_t>(offset))) : nullptr;
	}

	template <typename Type>
	static bool IsValidPtr(Type * ptr)
	{
		return (ptr && sizeof(ptr)) ? true : false;
	}

	static bool IsValidPtr(void* ptr)
	{
		return (ptr && sizeof(ptr)) ? true : false;
	}

	template<typename T>
	static T ReadMemory(SIZE_T address)
	{
		T buffer;
		if (!ReadProcessMemory(processHandle, (LPCVOID)address, &buffer, sizeof(T), NULL))
		{
			return 0;
			Log("[Memory] Reading memory failed. Address: 0x%X", address);
		}
		return buffer;
	}

	template<typename T>
	static bool ReadMemoryC(SIZE_T address)
	{
		T buffer;
		if (!ReadProcessMemory(processHandle, (LPCVOID)address, &buffer, sizeof(T), NULL))
		{
			Log("[Memory] Reading memory failed. Address: 0x%X", address);
			return false;
		}
		return true;
	}

	static string ReadString(SIZE_T address, int Size)
	{
		string buffer = "";
		if (!ReadProcessMemory(processHandle, (LPCVOID)address, &buffer, Size, NULL))
			Log("[Memory] Reading string failed. Address: 0x%X", address);
		return buffer;
	}

	static wstring ReadUnicodeString(SIZE_T address, int Size)
	{
		wstring buffer = L"";
		if (!ReadProcessMemory(processHandle, (LPCVOID)address, &buffer, Size, NULL))
			Log("[Memory] Reading string failed. Address: 0x%X", address);
		return buffer;
	}

	template<typename T>
	static bool WriteMemory(DWORD address, T value)
	{
		if (!WriteProcessMemory(processHandle, (LPVOID)address, &value, sizeof(T), NULL)) {
			Log("[Memory] Writing memory failed. Address: 0x%X", address);
			return false;
		}
		return true;
	}

	static bool WriteString(SIZE_T address, string str)
	{
		//if (!WriteProcessMemory(processHandle, (LPVOID)address, &str, Size, NULL)) {
		if (!WriteProcessMemory(processHandle, (LPVOID)address, str.c_str(), str.length(), 0)) {
			Log("[Memory] Writing string failed. Address: 0x%X", address);
			return false;
		}
		return true;
	}

	static bool WriteUnicodeString(SIZE_T address, wstring str)
	{
		//if (!WriteProcessMemory(processHandle, (LPVOID)address, &str, Size, NULL)) {
		if (!WriteProcessMemory(processHandle, (LPVOID)address, str.c_str(), str.length(), 0)) {
			Log("[Memory] Writing unicode string failed. Address: 0x%X", address);
			return false;
		}
		return true;
	}
};

void* DetourFunction(BYTE* src, const BYTE* dst, const int len)
{
	BYTE* jmp = (BYTE*)malloc(len + 5);
	DWORD dwBack;

	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &dwBack);
	memcpy(jmp, src, len);
	jmp += len;
	jmp[0] = 0xE9;
	*(DWORD*)(jmp + 1) = (DWORD)(src + len - jmp) - 5;
	src[0] = 0xE9;
	*(DWORD*)(src + 1) = (DWORD)(dst - src) - 5;
	for (int i = 5; i < len; i++)
		src[i] = 0x90;
	VirtualProtect(src, len, dwBack, &dwBack);
	return (jmp - len);
}