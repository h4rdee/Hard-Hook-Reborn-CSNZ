#pragma once

#include "OpenGL32.h"
#include "Structs.h"
#include "SDK.h"

typedef struct
{
	float flMatrix[4][4];
} WorldToScreenMatrix_t;

typedef struct Player_t
{
	char *Name, *Model;
} Player_s;

typedef void(__cdecl *TpfnEventCallback)(struct event_args_s *args);
										// ^ by sk0r

struct CGameEvents
{
	CGameEvents *pNext;
	char *szName; //0x0004 
	TpfnEventCallback pFunction; //0x0008 
};//Size=0x000C

struct CUserMessageHook					//geecko
{
	DWORD unkn1;						//0x0000
	DWORD unkn2;						//0x0004
	char szName[16];					//0x0008 
	CUserMessageHook *pNext;			//0x0018 
	pfnUserMsgHook pFunction;			//0x001C 
};										//Size=0x0020


void AutoSpam();

extern GLint	  vp[4];                 //viewport инфа(2 и 3 элемент содержат ширину / высоту экрана)
extern GLdouble   mm[16], pm[16];		 //различные координаты, например мышки
extern GLint      cm;
extern GLfloat    color[4];				 //для хранения текущего цвета

extern GLuint     base;					 //для шрифта

extern team_s     team[3];
extern keys_s     keys;
extern menu_s     menu;
extern cvars_s    cvar;

extern SCREENINFO ScreenInfo;

extern cl_clientfunc_t *g_pClient;
extern cl_enginefunc_t *g_pEngine;
extern engine_studio_api_t *g_pStudio;
extern cl_clientfunc_t g_Client;
extern cl_enginefunc_t g_Engine;
extern engine_studio_api_t g_Studio;
extern PUserMsg pUserMsgBase;

class HackUtils {						 //almost everything done by me, so credits to me:D
public:

	HackUtils() {
		Log("[Utils] Initialized");
	}

	~HackUtils() {
		Log("[Utils] ~Initialized");
	}

	const char* isWeapon(long vertcount);
	GLvoid BuildFont(GLvoid);
	GLvoid KillFont(GLvoid);
	void SaveState();
	void RestoreState();
	void SetVerts();
	void UnvalidVertex();
	void HandleKey(int key);
	int ChangeCvar(int a);;
	int GetVertexMin();
	int GetVertexMax();
	int GetTeam(long vertcount);
	bool isPointVisible(GLdouble x, GLdouble y, GLdouble z);
	bool isPlayer(long vertcount);
	bool isTerrorist(long vertcount);
	bool isCounterTerrorist(long vertcount);
	bool isZombie(long vertcount);
	bool Validate(HINSTANCE hinstDll);
	float GetWidth(long dist);

	//w2s by godly/////////////////////////////////////////////////////////////////////////////////////////////
	bool GetWorldToScreenMatrix(WorldToScreenMatrix_t *pWorldToScreenMatrix, float *flOrigin, float *flOut);
	bool WorldToScreen(float *flOrigin, float *flOut);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
}; extern HackUtils Utils;

class AutoOffset //thx CSX community
{
public:

	AutoOffset() {
		Log("[Auto-Offset] Initialized");
	}

	~AutoOffset() {
		Log("[Auto-Offset] ~Initialized");
	}

	BYTE HLType;
	bool GetRendererInfo();
	unsigned Absolute(DWORD Addr);
	DWORD GetModuleSize(const DWORD Address);
	PVOID ClientFuncs();
	PVOID EngineFuncs();

	/////////////////////////////////////////////////////////// not from them, geecko made it /////
	engine_studio_api_t* AutoOffset::GetEngineStudio();
	bool FindPattern(const BYTE* pData, const BYTE* bMask, const char* szMask);
	CUserMessageHook* GetFirstUserMsg();
	CGameEvents* GetFirstEvent();
	TpfnEventCallback HookEvent(const char* pszEventName, TpfnEventCallback pfnCallbackFunction);
	///////////////////////////////////////////////////////////////////////////////////////////////

private:
	DWORD HwBase, HwSize, HwEnd;
	DWORD ClBase, ClSize, ClEnd;
	DWORD UiBase, UiSize, UiEnd;
	DWORD HlBase, HlSize, HlEnd;
	BOOL  __comparemem(const UCHAR *buff1, const UCHAR *buff2, UINT size);
	ULONG __findmemoryclone(const ULONG start, const ULONG end, const ULONG clone, UINT size);
	ULONG __findreference(const ULONG start, const ULONG end, const ULONG address);
	DWORD FarProc(const DWORD Address, DWORD LB, DWORD HB);
}; extern AutoOffset Offset;

class VHookTable //thx again:D
{
public:

	bool HookTable(DWORD dwTablePtrPtr);
	void HookIndex(DWORD dwIndex, PVOID pAddress);

	DWORD RetHookIndex(DWORD dwIndex, PVOID pAddress);

	void UnHook();
	void ReHook();

private:

	DWORD dwPtrPtrTable;
	DWORD dwPtrNewTable;
	DWORD dwPtrOldTable;
	DWORD dwSizeTable;
}; extern VHookTable VMTHook;