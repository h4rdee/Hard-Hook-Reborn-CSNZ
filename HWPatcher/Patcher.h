#pragma once

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <deque>

using namespace std;

typedef struct
{
	const char* name;
	size_t size;
} TTerm;

extern deque<TTerm> Terms;

class CPatcher
{
private:
	//ifstream iFile;
	HANDLE hFile;
	OVERLAPPED OF;
	size_t iFileSize;
	const char* Substitute;
	std::string FileContent;
	int ReplaceCounter;
	const char* ModuleName;
public:
	bool Initialization(const char* module);
	void AddTerm(const char* term);
	bool Patch();

	void SetSubstitute(const char* sub);
	const char* GetSubstitute();
};

extern CPatcher Patcher;