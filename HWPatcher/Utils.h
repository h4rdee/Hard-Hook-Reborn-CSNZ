#pragma once

#include <Windows.h>
#include <string>

class CUtils
{
public:
	void ToClipboard(const std::string& s);
	const char* GetRandomString(int length);
};

extern CUtils Utils;