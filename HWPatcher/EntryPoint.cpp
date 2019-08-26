#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <string.h>
#include <ctime>
#include "Patcher.h"
#include "Utils.h"

void main()
{
	std::srand(time(NULL));
	SetConsoleTitleA("OpenGL hacks resurrector");

	if (Patcher.Initialization("hw.dll"))
	{
		Patcher.AddTerm("opengl32");
		Patcher.AddTerm("opengl32.dll");
		Patcher.SetSubstitute(Utils.GetRandomString(8));

		if (Patcher.Patch())
		{
			Utils.ToClipboard(Patcher.GetSubstitute());
			printf("The patching was completed successfully.\nRename your OpenGL hack to %s (the title was copied to the clipboard,\npress CTRL + V to paste)\n", Patcher.GetSubstitute());
		}
	}

	printf("\n");
	system("pause");
}