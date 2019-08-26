#include "Patcher.h"

deque<TTerm> Terms;

bool CPatcher::Initialization(const char* module)
{
	ModuleName = module;

	ifstream iFile(module, std::ios::binary);

	iFile.seekg(0, std::ios::end);
	iFileSize = iFile.tellg();

	if (GetLastError())
	{
		printf("%s wasn't loaded\n", module);
		return false;
	}

	char* Buffer = new char[iFileSize + 1];
	iFile.seekg(0, std::ios::beg);

	FileContent.reserve(iFileSize);
	std::streamsize CharsRead;

	while (iFile.read(Buffer, sizeof Buffer), CharsRead = iFile.gcount())
		FileContent.append(Buffer, CharsRead);

	hFile = CreateFileA(module,
		GENERIC_WRITE, FILE_SHARE_READ,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	OF.OffsetHigh = 0;
	OF.Offset = 0;
	OF.hEvent = 0;

	bool res = hFile ? true : false;
	if(!res)
		printf("%s wasn't loaded\n", module);

	if(res && !iFile.eof())
		printf("End of file %s wasn't reached\n", module);

	return res;
}

void CPatcher::AddTerm(const char* termName)
{
	TTerm term;
	term.name = termName;
	term.size = strlen(termName);
	Terms.push_back(term);
}

bool CPatcher::Patch()
{
	for (TTerm term : Terms)
	{
		std::string::size_type offset = NULL, foundAt = NULL;
		DWORD WriteSize;

		for (;
			iFileSize > offset &&
			(foundAt = FileContent.find(term.name, offset)) !=
			std::string::npos;
			offset = foundAt + term.size)
		{
			ReplaceCounter++;
			OF.Offset = foundAt;
			WriteFile(hFile, Substitute, strlen(Substitute), &WriteSize, &OF);
		}

		Terms.pop_back();
	}

	if (ReplaceCounter > 0)
		return true;
	else
	{
		printf("%s was already patched or corrupted\n", ModuleName);
		return false;
	}
}

void CPatcher::SetSubstitute(const char* sub)
{
	Substitute = sub;
}

const char* CPatcher::GetSubstitute()
{
	return Substitute;
}

CPatcher Patcher;