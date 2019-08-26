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
bool bDrawnWorld = false;			    //�������������� �� ���?
bool bDrawingEnts = false;			    //�������������� �� ������?
bool bFlash = false;					//��������/���
bool bSmoke = false;					//����� ���/���
bool bScope = false;					//����������� ������/���
bool bWall = false;
bool bSky = false;
bool bCross = false;
bool bFOV = false;
bool FirstInit = false;
bool ModelViewport = false;
bool EnableDraw = false;				//true ����� viewport ������ 5 ��� (���� ����� ���)
bool bGotFlashed = false;
bool bConfigFound = false;				//true ���� ������ �� ������
bool bModelsFound = false;				//true ���� ���� � ���������� �� ������, �������� ������������
bool bWhite = false;
bool bBlack = false;
bool bFirstFrame = false;
bool bModulesLoaded = false;
bool hooksApplied = false;

int player_height_min = 0, player_height_max = 0;
int player_vertex_min = 0, player_vertex_max = 0;
int ViewportCount = 0, ProneFix, AimAt = 0;
//0, 1, 2, 3 � 4 ��������� �������� �� ������ ������ �����

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
GLuint base;							//��� ������
float roffset;							//������� ������

GLint	 vp[4];							//viewport ����(2 � 3 ������� �������� ������ / ������ ������)
GLdouble mm[16], pm[16];				//��������� ����������, �������� �����
GLint    cm;
GLfloat  color[4];

Player_s Player[33];
int Flags;
