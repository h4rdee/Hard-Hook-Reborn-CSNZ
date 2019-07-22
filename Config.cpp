
#include "Config.h"

char* Settings::GetDirectoryFile(char *filename) //получает путь к файлу
{
	static char path[320];
	strcat_s(path, filename);
	return path;
}

void Settings::Init() { //первоначальные настройки
	cvar.drawfov = 1; cvar.noscope = 1; menu.count = 1; cvar.nofog = 1; cvar.zoomfactor = 1; cvar.nosmoke = 1; cvar.noflash = 1;
	colors.chamsRed = 255; colors.chamsGreen = 51; colors.chamsBlue = 122;
	colors.crossRed = 0; colors.crossGreen = 255; colors.crossBlue = 0; colors.crossAlpha = 255;
	colors.hudRed = 250; colors.hudGreen = 250; colors.hudBlue = 250;
	keys.aimkey = VK_MBUTTON; keys.zoomkey = VK_RBUTTON; keys.menukey = VK_INSERT;
	cvar.wallhack = 1; cvar.crosshair = 1; cvar.fov = 20;
	cvar.esp = 1; cvar.aimbot = 1; cvar.esplines = 0; cvar.esptriangles = 0;
	cvar.espbox = 0; cvar.hudhack = 1;
	cvar.duck_h = 19.0f; cvar.stand_h = 19.5f; cvar.crosshairSize = 4;
	cvar.aimdiff = 1.55f; cvar.aimheight_s = 4.5f; cvar.aimheight_d = (float)(cvar.aimheight_s * cvar.aimdiff);
	cvar.espdebug = 0; cvar.debug = 0; cvar.aimsmooth = 0; cvar.aimpreset = 1; AimPreset = "Head"; cvar.rcs = 0;
	cvar.target = 0; AimTeam = "Everyone";
	/*Developer stuff*/
	cvar.autospam = 1; cvar.fullbright = 1;
	Log("[Settings] Default vars were defined");
}

void Settings::LoadSettings() { //загрузка конфига
	Init();
	ifstream fin;
	string Word = "";

	fin.open(GetDirectoryFile("hardhook.cfg"), ifstream::in);
	fin >> Word >> cvar.wallhack;
	fin >> Word >> cvar.chams;
	fin >> Word >> cvar.esp;
	fin >> Word >> cvar.espbox;
	fin >> Word >> cvar.esplines;
	fin >> Word >> cvar.esptriangles;
	fin >> Word >> cvar.aimbot;
	fin >> Word >> cvar.target;
	fin >> Word >> cvar.aimpreset;
	fin >> Word >> cvar.fov;
	fin >> Word >> cvar.noflash;
	fin >> Word >> cvar.nosmoke;
	fin >> Word >> cvar.nofog;
	fin >> Word >> cvar.noscope;
	fin >> Word >> cvar.crosshair;
	fin >> Word >> cvar.crosshairSize;
	fin >> Word >> cvar.hudhack;
	fin >> Word >> cvar.zoomhack;
	fin >> Word >> cvar.zoomfactor;
	Log("[Settings] Config Loaded");
	fin.close();
}

void Settings::SaveSettings() { //сохранение конфига
	ofstream fout;

	fout.open(GetDirectoryFile("hardhook.cfg"), ios::trunc);
	fout << "wallhack " << cvar.wallhack << endl;
	fout << "chams " << cvar.chams << endl;
	fout << "esp " << cvar.esp << endl;
	fout << "box " << cvar.espbox << endl;
	fout << "lines " << cvar.esplines << endl;
	fout << "triangles " << cvar.esptriangles << endl;
	fout << "aimbot " << cvar.aimbot << endl;
	fout << "aimteam " << cvar.target << endl;
	fout << "aimpreset " << cvar.aimpreset << endl;
	fout << "fov " << cvar.fov << endl;
	fout << "noflash " << cvar.noflash << endl;
	fout << "nosmoke " << cvar.nosmoke << endl;
	fout << "nofog " << cvar.nofog << endl;
	fout << "noscope " << cvar.noscope << endl;
	fout << "crosshair " << cvar.crosshair << endl;
	fout << "chsize " << cvar.crosshairSize << endl;
	fout << "hudhack " << cvar.hudhack << endl;
	fout << "zoomhack " << cvar.zoomhack << endl;
	fout << "zoomfactor " << cvar.zoomfactor << endl;
	Log("[Settings] Config Saved");
	fout.close();
}