#pragma once

/*
	Error Codes:
	0x01 - )
	0x02 - hardhook.exe unloaded
	0x03 - OllyDBG found
	0x04 - CE found
*/

#define NULL 0
#define _CRT_SECURE_NO_WARNINGS

#include "Draw.h"
#include "Utils.h"
#include "Config.h"

bool FoundAnEntity = false;
bool bDrawnWorld = false;			    //отрисовывается ли мир?
bool bDrawingEnts = false;			    //отрисовываются ли энтити?
bool bFlash = false;					//ослеплен/нет
bool bSmoke = false;					//лежит дым/нет
bool bScope = false;					//снайперский прицел/нет
bool bWall = false;
bool bSky = false;
bool bCross = false;
bool bFOV = false;
bool FirstInit = false;
bool ModelViewport = false;
bool EnableDraw = false;				//true когда viewport вызван 5 раз (фикс дропа фпс)
bool bGotFlashed = false;
bool bConfigFound = false;				//true если конфиг не найден
bool bModelsFound = false;				//true если файл с вертексами не найден, частично используется
bool bWhite = false;
bool bBlack = false;
bool bFirstFrame = false;
bool bModulesLoaded = false;
bool hooksApplied = false;

int player_height_min = 0, player_height_max = 0;
int player_vertex_min = 0, player_vertex_max = 0;
int ViewportCount = 0, ProneFix, AimAt = 0;
//0, 1, 2, 3 и 4 проверяет является ли модель игрока целью

DrawGL Draw;
HackUtils Utils;
Settings Config;
AutoOffset Offset;
VHookTable VMT;

void __cdecl Log(const char* fmt, ...);
DWORD pID, TargetX, TargetY; HANDLE processHandle;
HINSTANCE hInst;
const char* AimPreset;
const char* AimTeam;
GLuint base;							//для шрифта
float roffset;							//текущий оффсет

GLint	 vp[4];							//viewport инфа(2 и 3 элемент содержат ширину / высоту экрана)
GLdouble mm[16], pm[16];				//различные координаты, например мышки
GLint    cm;
GLfloat  color[4];

Player_s Player[33];
int Flags;
