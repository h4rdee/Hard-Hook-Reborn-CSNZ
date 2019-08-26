#include "Utils.h"

void CUtils::ToClipboard(const std::string& s)
{
	OpenClipboard(0);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
	if (!hg) {
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}

const char* CUtils::GetRandomString(int length)
{
	char* res = new char[length];

	for (int i = 0; i < length; i++)
	{
		char j = 30 + rand() % 92;

		if ((j >= 48 && j <= 57) || (j >= 65 && j <= 90) || (j >= 97 && j <= 122))
			res[i] = (char)j;
		else
			if (i >= 0) i--;
	}
	res[length] = '\0';

	return res;
}

CUtils Utils;