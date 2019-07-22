#pragma once

/*
	Error Codes:
	0x01 - )
	0x02 - hardhook.exe unloaded
	0x03 - OllyDBG found
	0x04 - CE found
*/

#define NULL 0

#include "Draw.h"
#include "Protection.h"
#include "Utils.h"
#include "Config.h"
#include "SDK.h"

extern SCREENINFO g_Screen;
extern cl_clientfunc_t *g_pClient;
extern cl_enginefunc_t *g_pEngine;
extern engine_studio_api_t *g_pStudio;
extern cl_clientfunc_t g_Client;
extern cl_enginefunc_t g_Engine;
extern engine_studio_api_t g_Studio;
extern PUserMsg pUserMsgBase;

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
int ViewportCount = 0,  ProneFix, AimAt = 0;	
										//0, 1, 2, 3 � 4 ��������� �������� �� ������ ������ �����

DrawGL Draw;
HackProtection Protection;
HackUtils Utils;
Settings Config;
AutoOffset Offset;
VHookTable VMT;

void __cdecl Log(const char * fmt, ...);
DWORD WINAPI ProtectionThread(__in LPVOID lpParameter);
DWORD WINAPI SetupHooks(__in LPVOID lpParameter);
DWORD pID, TargetX, TargetY; HANDLE processHandle;
HINSTANCE hInst;
char *AimPreset, *AimTeam;
GLuint base;							//��� ������
float roffset;							//������� ������

GLint	 vp[4];							//viewport ����(2 � 3 ������� �������� ������ / ������ ������)
GLdouble mm[16], pm[16];				//��������� ����������, �������� �����
GLint    cm;
GLfloat  color[4];

//bool SetupInterfaces();

IVGuiModuleLoader* g_pIVGuiModuleLoader = nullptr;
IRunGameEngine* g_pIRunGameEngine = nullptr;
IGameUI* g_pGameUI = nullptr;
IGameConsole* g_pConsole = nullptr;

vgui::IPanel* g_pIPanel = nullptr;
vgui::ISurface* g_pISurface = nullptr;
vgui::IEngineVGui* g_pIEngineVGui = nullptr;

vgui::IPanel* RootPanel;
vgui::IPanel* ClientPanel;
vgui::IPanel* GameUIPanel;

SCREENINFO ScreenInfo;
Player_s Player[33];
int Flags;
Vector Velocity;

CreateInterfaceFn CaptureFactory(char* FactoryModule)
{
	CreateInterfaceFn Interface = 0;
	while (!Interface)
	{
		HMODULE hFactoryModule = GetModuleHandleA(FactoryModule);
		if (hFactoryModule)
		{
			Interface = (CreateInterfaceFn)(GetProcAddress(hFactoryModule, CREATEINTERFACE_PROCNAME));
		}
		Sleep(100);
	}
	return Interface;
}

PVOID CaptureInterface(CreateInterfaceFn Interface, char* InterfaceName)
{
	PVOID dwPointer = nullptr;
	while (!dwPointer)
	{
		dwPointer = (PVOID)(Interface(InterfaceName, 0));
		Sleep(100);
	}
	return dwPointer;
}
