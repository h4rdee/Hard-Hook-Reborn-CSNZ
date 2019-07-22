#pragma once

#include "Structs.h"
#include <Windows.h>
#include <string>
#include <fstream>

extern cvars_s  cvar;
extern colors_s colors;
extern keys_s   keys;
extern menu_s   menu;

extern void __cdecl Log(const char * fmt, ...);
extern char *AimPreset, *AimTeam;

using namespace std;

class Settings {
public:

	Settings() {
		Log("[Settings] Initialized");
	}

	~Settings() {
		Log("[Settings] ~Settings");
	}

	void Init();
	void LoadSettings();
	void SaveSettings();
	char* GetDirectoryFile(char *filename);

private:
	bool ConfigFound;
}; extern Settings Config;