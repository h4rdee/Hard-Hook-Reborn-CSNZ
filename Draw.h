#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "OpenGL32.h"
#include "Utils.h"
#include "Structs.h"

extern const char* AimPreset;
extern const char* AimTeam;
extern GLuint base;

extern cvars_s cvar;
extern keys_s keys;
extern menu_s menu;

extern Player_t Player[33];

class DrawGL {
public:

	DrawGL() {
		Log("[Drawing] Initialized");
	}

	~DrawGL() {
		Log("[Drawing] ~Drawing");
	}

	void Circle(int r, float lw, int red, int green, int blue, int alpha);
	void Line(int x1, int y1, int x2, int y2, int lw, int red, int green, int blue, int alpha);
	void Rect(int x, int y, int w, int h, int lw, int red, int green, int blue, int alpha);
	void MenuForm(int x, int y, int w, int h, int lw, int lineoffset, int lineoffset2, int red, int green, int blue, int alpha);
	void Text(int x, int y, int red, int green, int blue, const char* fmt, ...);
	void Time(int x, int y, int red, int green, int blue);
	void Crosshair(int red, int green, int blue, int alpha, int size, int type);
	void AimPoint(float x, float y);
	void Menu(int x, int y);
	void MenuItems(int x, int y, int menuX1, int menuX2);
	void PlayersInfo(int x, int y);

	void WeaponEsp(float x, float y, float x2, float y2, long vertcount);
	void DebugEsp(float x, float y, long vertcount);
	bool PlayerEsp(float x, float y, float z, float x2, float y2, float z2, long vertcount, double distance);
}; extern DrawGL Draw;