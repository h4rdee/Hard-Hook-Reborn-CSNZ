
#include "Utils.h"
#include "SDK.h"

extern SCREENINFO g_Screen;
extern cl_clientfunc_t *g_pClient;
extern cl_enginefunc_t *g_pEngine;
extern cl_clientfunc_t g_Client;
extern cl_enginefunc_t g_Engine;

#define CompareMemory(Buff1, Buff2, Size) __comparemem((const UCHAR *)Buff1, (const UCHAR *)Buff2, (UINT)Size)
#define FindMemoryClone(Start, End, Clone, Size) __findmemoryclone((const ULONG)Start, (const ULONG)End, (const ULONG)Clone, (UINT)Size)
#define FindReference(Start, End, Address) __findreference((const ULONG)Start, (const ULONG)End, (const ULONG)Address)

//DWORD cl, eg;
typedef unsigned Cardinal;
typedef unsigned* PCardinal;
typedef unsigned** PPCardinal;

HDC hDC;
long T[13] = { 1978, 2065, 2152, 2074, 2070, 2092, 2069, 2039, 2156, 1982, 2096, 2133, 2137 };
long CT[15] = { 1978, 2011, 2088, 1984, 1988, 2007, 2313, 2317, 2033, 1780, 2037, 2002, 1878, 2111, 2115 };
long UC[7] = { 1982, 1988, 2056, 2037, 2429, 2242 };
long ZM[27] = { 2448, 2853, 4830, 2591, 2064, 3614, 2411, 4746, 2238, 4172, 4346, 3656, 2452, 2415, 2068, 2350, 2354, 1994, 1996, 2035, 2037, 1998, 2242, 2429, 2425, 5790, 4716 };
long TRASH[46] = { 1962, 2059, 2024, 2357, 2361, 2055, 2030, 2200, 2192, 2182, 2180, 2172, 2124, 2112, 2108, 2184, 2164, 2160, 1966, 2489, 1792, 1776, 1784, 1788,
2148, 2144, 2342, 2326, 1968, 1964, 1960, 1956, 1952, 1948, 1944, 1940, 1936, 1932, 1928, 1924, 1920, 1916, 10300 };

//DWORD GameUI, HlBase, HwBase, vgui, vgui2;

GLvoid HackUtils::BuildFont(GLvoid) {
	hDC = wglGetCurrentDC();
	HFONT	font;
	HFONT	oldfont;
	base = (*orig_glGenLists)(96);
	font = CreateFontA(-10, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Verdana");
	oldfont = (HFONT)SelectObject(hDC, font);
	wglUseFontBitmaps(hDC, 32, 96, base);
	SelectObject(hDC, oldfont);
	DeleteObject(font);
}

void HackUtils::KillFont(GLvoid) {
	(*orig_glDeleteLists)(base, 96);
}

int HackUtils::ChangeCvar(int a) {
	int b = 0;
	if (a == 0) { b = 1; }
	else if (a == 1) { b = 0; }
	return b;
}

int HackUtils::GetTeam(long vertcount) {
	for (int i = 0; i < sizeof(T) / sizeof(*T); i++)		 //terrorist
		if (vertcount == T[i] || vertcount - 68 == T[i])	 //68 - bomb
			return 1;

	for (int i = 0; i < sizeof(CT) / sizeof(*CT); i++)		 //counter-terrorist
		if (vertcount == CT[i])
			return 2;

	for (int i = 0; i < sizeof(ZM) / sizeof(*ZM); i++)		 //zombie
		if (vertcount == ZM[i])
			return 3;

	for (int i = 0; i < sizeof(TRASH) / sizeof(*TRASH); i++) //trash
		if (vertcount == TRASH[i])
			return 4;

	return 0;												 //undefined
}

int HackUtils::GetVertexMax() {
	int max = 0;

	for (int fx = 0; fx < 3; fx++) {
		for (int fn = 0; fn < 30; fn++) {
			if (team[fx].verts[fn] > max) {
				max = team[fx].verts[fn];
			}
		}
	}

	max = max + 5;
	return max;
}

int HackUtils::GetVertexMin() {
	int min = 26666;

	for (int gx = 0; gx < 3; gx++) {
		for (int gn = 0; gn < 30; gn++) {
			if ((team[gx].verts[gn] > 0) && (team[gx].verts[gn] < min)) {
				min = team[gx].verts[gn];
			}
		}
	}

	min = min - 5;
	return min;
}

float HackUtils::GetWidth(long dist) {
	return (float)(0.075 - dist / 100 * 0.01);
}

void HackUtils::HandleKey(int key) {
	if (GetAsyncKeyState(keys.menukey)) {
		if (!keys.active)
		{
			keys.active = true;
			menu.active = !menu.active;
		}
	}
	else if (!GetAsyncKeyState(keys.menukey)) {
		keys.active = false;
	}
	if (GetAsyncKeyState(key))
	{
		switch (key) {
		case VK_UP:
			if (menu.active && !keys.up)
			{
				keys.up = true;
				menu.count -= 1;
			}
			break;
		case VK_DOWN:
			if (menu.active && !keys.down)
			{
				keys.down = true;
				menu.count += 1;
			}
			break;
		case VK_RIGHT:
			if (menu.active && !keys.right)
			{
				keys.right = true;
				menu.select = true;
				menu.select_r = true;
			}
			break;
		case VK_LEFT:
			if (menu.active && !keys.left)
			{
				keys.left = true;
				menu.select = true;
				menu.select_l = true;
			}
			break;
		}
	} else if (!GetAsyncKeyState(key)) {
		switch (key) {
			case VK_UP: keys.up = false; break;
			case VK_DOWN: keys.down = false; break;
			case VK_RIGHT: keys.right = false; break;
			case VK_LEFT: keys.left = false; break;
		}
	}
}

bool HackUtils::isTerrorist(long vertcount) {
	for (int vertnum = 0; vertnum < 30; vertnum++) {
		if (vertcount == team[0].verts[vertnum] || vertcount - 68 == team[0].verts[vertnum]) { //68 - bomb
			return true;
		}
	}
	return false;
}

bool HackUtils::isCounterTerrorist(long vertcount) {
	for (int vertnum = 0; vertnum < 30; vertnum++) {
		if (vertcount == team[1].verts[vertnum]) {
			return true;
		}
	}
	return false;
}

bool HackUtils::isZombie(long vertcount) {
	for (int vertnum = 0; vertnum < 30; vertnum++) {
		if (vertcount == team[2].verts[vertnum]) {
			return true;
		}
	}
	return false;
}

const char* HackUtils::isWeapon(long vertcount) {
	switch (vertcount) {
	case 145:
		return "c4";
		break;
	case 60:
		return "grenade";
		break;
	default:
		return "not a weapon";
		break;
	}
}

bool HackUtils::isPlayer(long vertcount) {
	if (cvar.debug) {
		if (vertcount > vertMin && vertcount < vertMax) {
			return true;
		}
		else {
			return false;
		}
	}

	for (int t = 0; t < 3; t++) {
		for (int vertnum = 0; vertnum < 30; vertnum++) {
			if (vertcount == team[t].verts[vertnum]) {
				return true;
			}
		}
	}

	return false;
}

bool HackUtils::isPointVisible(GLdouble x, GLdouble y, GLdouble z) {
	GLdouble	winX, winY, winZ;
	GLfloat		pix;

	(*orig_glGetDoublev)(GL_MODELVIEW_MATRIX, mm);
	(*orig_glGetDoublev)(GL_PROJECTION_MATRIX, pm);

	if (gluProject(x, y, z, mm, pm, vp, &winX, &winY, &winZ) == GL_TRUE)
	{
		(*orig_glReadPixels)((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &pix);
			return pix > winZ ? true : false;
	}
	else
		return false;
}

void HackUtils::SaveState() {
	(*orig_glGetIntegerv)(GL_MATRIX_MODE, &cm);
	(*orig_glGetDoublev)(GL_MODELVIEW_MATRIX, mm);
	(*orig_glGetDoublev)(GL_PROJECTION_MATRIX, pm);
	(*orig_glDisable)(GL_TEXTURE_2D);
	(*orig_glMatrixMode)(GL_PROJECTION);
	(*orig_glPushMatrix)();
	(*orig_glLoadIdentity)();
	(*orig_glMatrixMode)(GL_MODELVIEW);
	(*orig_glPushMatrix)();
	(*orig_glLoadIdentity)();
	(*orig_glGetFloatv)(GL_CURRENT_COLOR, color);
}

void HackUtils::RestoreState() {
	(*orig_glMatrixMode)(GL_PROJECTION);
	(*orig_glPopMatrix)();
	(*orig_glMatrixMode)(GL_MODELVIEW);
	(*orig_glPopMatrix)();
	(*orig_glMatrixMode)(cm);
	(*orig_glEnable)(GL_TEXTURE_2D);
	(*orig_glColor4f)(color[0], color[1], color[2], color[3]);
}

void HackUtils::SetVerts() {
	scanf("Counter-Terrorists", &team[0].name);
	team[0].verts[0] = 2313;
	team[0].verts[1] = 2033;
	team[0].verts[2] = 2007;
	team[0].verts[3] = 1984;
	team[0].verts[4] = 1780;
	team[0].verts[5] = 2088;
	team[0].verts[6] = 2011;
	team[0].verts[7] = 1978;
	team[0].verts[8] = 2317;
	team[0].verts[9] = 2037;
	team[0].verts[10] = 2002;
	team[0].verts[11] = 1988;
	team[0].verts[12] = 1878;
	team[0].verts[13] = 2111;
	team[0].verts[14] = 2115;

	scanf("Terrorists", &team[1].name);
	team[1].verts[0] = 2092;
	team[1].verts[1] = 2065;
	team[1].verts[2] = 2070;
	team[1].verts[3] = 1978;
	team[1].verts[4] = 2039;
	team[1].verts[5] = 2152;
	team[1].verts[6] = 2074;
	team[1].verts[7] = 2069;
	team[1].verts[8] = 2156;
	team[1].verts[9] = 1982;
	team[1].verts[10] = 2096;
	team[1].verts[11] = 2133;
	team[1].verts[12] = 2137;

	scanf("Zombies", &team[2].name);
	team[2].verts[0] = 2448;
	team[2].verts[1] = 2853;
	team[2].verts[2] = 2591;
	team[2].verts[3] = 2064;
	team[2].verts[4] = 2411;
	team[2].verts[5] = 2238;
	team[2].verts[6] = 3614;
	team[2].verts[7] = 3656;
	team[2].verts[8] = 4172;
	team[2].verts[9] = 4346;
	team[2].verts[10] = 4746;
	team[2].verts[11] = 4830;
	team[2].verts[12] = 2452;
	team[2].verts[13] = 2415;
	team[2].verts[14] = 2068;
	team[2].verts[15] = 2350;
	team[2].verts[16] = 2354;
	team[2].verts[17] = 2035;
	team[2].verts[18] = 2037;
	team[2].verts[19] = 1994;
	team[2].verts[20] = 1996;
	team[2].verts[21] = 1998;
	team[2].verts[22] = 2242;
	team[2].verts[23] = 2429;
	team[2].verts[24] = 2425;
	team[2].verts[25] = 5790; //Chaser
	team[2].verts[26] = 4716; //Blotter
}

void HackUtils::UnvalidVertex() {
	for (int vx = 0; vx < 3; vx++) {
		for (int vn = 0; vn < 30; vn++) {
			team[vx].verts[vn] = -1;
		}
	}
}

bool HackUtils::Validate(HINSTANCE hinstDll) {
	CHAR szPath[MAX_PATH];
	GetModuleFileName(hinstDll, szPath, MAX_PATH);
	return (strstr(szPath, "opengl")) ? false : true;
}

bool HackUtils::GetWorldToScreenMatrix(WorldToScreenMatrix_t *pWorldToScreenMatrix, float *flOrigin, float *flOut)
{
	flOut[0] = pWorldToScreenMatrix->flMatrix[0][0] * flOrigin[0] + pWorldToScreenMatrix->flMatrix[1][0] * flOrigin[1] + pWorldToScreenMatrix->flMatrix[2][0] * flOrigin[2] + pWorldToScreenMatrix->flMatrix[3][0];
	flOut[1] = pWorldToScreenMatrix->flMatrix[0][1] * flOrigin[0] + pWorldToScreenMatrix->flMatrix[1][1] * flOrigin[1] + pWorldToScreenMatrix->flMatrix[2][1] * flOrigin[2] + pWorldToScreenMatrix->flMatrix[3][1];
	float flZ = pWorldToScreenMatrix->flMatrix[0][3] * flOrigin[0] + pWorldToScreenMatrix->flMatrix[1][3] * flOrigin[1] + pWorldToScreenMatrix->flMatrix[2][3] * flOrigin[2] + pWorldToScreenMatrix->flMatrix[3][3];

	if (flZ != 0.0f)
	{
		float flTmp = 1.0f / flZ;
		flOut[0] *= flTmp;
		flOut[1] *= flTmp;
	}

	return (flZ <= 0.0f);
}

bool HackUtils::WorldToScreen(float *flOrigin, float *flOut)
{
	WorldToScreenMatrix_t *pWorldToScreenMatrix = (WorldToScreenMatrix_t*)((DWORD)GetModuleHandle("hw.dll") + 0xE956A0);
	if (!GetWorldToScreenMatrix(pWorldToScreenMatrix, flOrigin, flOut))
	{
		flOut[0] = (flOut[0] * (ScreenInfo.iWidth / 2)) + ScreenInfo.iWidth / 2;
		flOut[1] = (-flOut[1] * (ScreenInfo.iHeight / 2)) + ScreenInfo.iHeight / 2;
		return true;
	}
	return false;
}

bool VHookTable::HookTable(DWORD dwTablePtrPtr)
{
	DWORD dwIndexFunction = 0;

	dwPtrPtrTable = dwTablePtrPtr;
	dwPtrOldTable = *(PDWORD)dwPtrPtrTable;

	for (dwIndexFunction = 0; ((PDWORD)*(PDWORD)dwTablePtrPtr)[dwIndexFunction]; dwIndexFunction++)
		if (IsBadCodePtr((FARPROC)((PDWORD)*(PDWORD)dwTablePtrPtr)[dwIndexFunction]))
			break;

	dwSizeTable = sizeof(DWORD) * dwIndexFunction;

	if (dwIndexFunction && dwSizeTable)
	{
		dwPtrNewTable = (DWORD)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwSizeTable);
		memcpy((PVOID)dwPtrNewTable, (PVOID)*(PDWORD)dwTablePtrPtr, dwSizeTable);

		*(PDWORD)dwTablePtrPtr = dwPtrNewTable;

		return true;
	}

	return false;
}

void VHookTable::HookIndex(DWORD dwIndex, PVOID pAddress)
{
	((PDWORD)dwPtrNewTable)[dwIndex] = (DWORD)pAddress;
}

DWORD VHookTable::RetHookIndex(DWORD dwIndex, PVOID pAddress)
{
	DWORD old = ((PDWORD)dwPtrNewTable)[dwIndex];
	((PDWORD)dwPtrNewTable)[dwIndex] = (DWORD)pAddress;
	return old;
}

void VHookTable::UnHook()
{
	if (dwPtrPtrTable)
		*(PDWORD)dwPtrPtrTable = dwPtrOldTable;
}

void VHookTable::ReHook()
{
	if (dwPtrPtrTable)
		*(PDWORD)dwPtrPtrTable = dwPtrNewTable;
}

bool AutoOffset::GetRendererInfo()
{
	/*while (!(HwBase && ClBase && HlBase))
	{
		HwBase = (DWORD)GetModuleHandleA("hw.dll");
		ClBase = (DWORD)GetModuleHandleA("client.dll");
		HlBase = (DWORD)GetModuleHandleA(NULL);

		Sleep(100);
	}*/

	HwBase = (DWORD)GetModuleHandleA("hw.dll");
	ClBase = (DWORD)GetModuleHandleA("client.dll");
	HlBase = (DWORD)GetModuleHandleA(NULL);

	HwSize = (DWORD)GetModuleSize(HwBase);
	HwEnd = HwBase + HwSize - 1;

	ClSize = (DWORD)GetModuleSize(ClBase);
	ClEnd = ClBase + ClSize - 1;

	HlSize = (DWORD)GetModuleSize(HlBase);
	HlEnd = HlBase + HlSize - 1;

	char buf[256];
	sprintf(buf, "\r\n\r\nModules Info:\r\n\r\nHWBase: 0x%X\r\nHWSize: 0x%X\r\nCLBase: 0x%X\r\nCLSize: 0x%X\r\nHLBase: 0x%X\r\nHLSize: 0x%X\r\n", HwBase, HwSize, ClBase, ClSize, HlBase, HlSize);
	Log(buf);
	//MessageBox(0, buf, "HardHook", 0);

	bool bFound = HwBase && ClBase && HlBase;
	bFound ? Log("[Hooking] Modules found") : Log("[Hooking] Modules not found");
	return bFound;
}

unsigned AutoOffset::Absolute(DWORD Addr)
{
	return Cardinal(Addr) + *(PCardinal)(Addr)+4;
}

DWORD AutoOffset::GetModuleSize(const DWORD Address)
{
	return PIMAGE_NT_HEADERS(Address + (DWORD)PIMAGE_DOS_HEADER(Address)->e_lfanew)->OptionalHeader.SizeOfImage;
}

DWORD AutoOffset::FarProc(const DWORD Address, DWORD LB, DWORD HB)
{
	return ((Address < LB) || (Address > HB));
}

BOOL AutoOffset::__comparemem(const UCHAR *buff1, const UCHAR *buff2, UINT size)
{
	for (UINT i = 0; i < size; i++, buff1++, buff2++)
	{
		if ((*buff1 != *buff2) && (*buff2 != 0xFF))
			return FALSE;
	}
	return TRUE;
}

ULONG AutoOffset::__findmemoryclone(const ULONG start, const ULONG end, const ULONG clone, UINT size)
{
	for (ULONG ul = start; (ul + size) < end; ul++)
	{
		if (CompareMemory(ul, clone, size))
			return ul;
	}
	return NULL;
}

ULONG AutoOffset::__findreference(const ULONG start, const ULONG end, const ULONG address)
{
	UCHAR Pattern[5];
	Pattern[0] = 0x68;
	*(ULONG*)&Pattern[1] = address;
	return FindMemoryClone(start, end, Pattern, sizeof(Pattern) - 1);
}

PVOID AutoOffset::ClientFuncs()
{
	DWORD Address = (DWORD)FindMemoryClone(HwBase, HwEnd, "ScreenFade", strlen("ScreenFade"));
	PVOID ClientPtr = (PVOID)*(PDWORD)(FindReference(HwBase, HwEnd, Address) + 0x13); // all patch

	if (FarProc((DWORD)ClientPtr, HwBase, HwEnd))
		Log("[Hooking] Failed find Client ptr.");

	Log("[Hooking] Found Client ptr. at: 0x%X", (DWORD)ClientPtr);
	return ClientPtr;
}

PVOID AutoOffset::EngineFuncs()
{
	DWORD Address = (DWORD)FindMemoryClone(HwBase, HwEnd, "ScreenFade", strlen("ScreenFade"));
	PVOID EnginePtr = (PVOID)*(PDWORD)(FindReference(HwBase, HwEnd, Address) + 0x0D); // 0x0D CSNZ Engine ptr. by Jusic

	if (FarProc((DWORD)EnginePtr, HwBase, HwEnd))
		Log("[Hooking] Failed find Engine ptr.");

	Log("[Hooking] Found Engine ptr. at: 0x%X", (DWORD)EnginePtr);
	return EnginePtr;
}

void AutoSpam()
{
	while (1)
	{
		if (cvar.autospam) {
			g_pEngine->pfnClientCmd("say ""vk.com/hardhook ~ CSN:Z hack :3""");
			Sleep(1000);
			g_pEngine->pfnClientCmd("say ""Hard Hook - Perfect Anti-Turk system!"""); //Can Varol can suck my peepee btw
			Sleep(1000);
		}
	}
}

bool AutoOffset::FindPattern(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask != 'x' && *pData != *bMask)
			return false;

	return (*szMask) == NULL;
}

engine_studio_api_t* AutoOffset::GetEngineStudio()
{
	static DWORD dwEngineStudio;

	if (!dwEngineStudio)
	{
		dwEngineStudio = FindPattern((BYTE*)HwBase, (BYTE*)"\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00\x6A\x01\xFF\xD0\x83\xC4\x0C", "x????x????xxxxxxx");
		dwEngineStudio = *(DWORD*)(dwEngineStudio + 0x1);
	}

	return (engine_studio_api_t*)dwEngineStudio;
}

CGameEvents* AutoOffset::GetFirstEvent()
{

	static DWORD dwFirstEvent;

	if (!dwFirstEvent)
	{
		dwFirstEvent = FindPattern((BYTE*)HwBase, (BYTE*)"\x8B\x15\x00\x00\x00\x00\x89\x46\x04\x8B\x4C\x24\x14", "xx????xxxxxxx");
		dwFirstEvent = *(DWORD*)(*(DWORD*)(dwFirstEvent + 0x2));
	}

	return (CGameEvents*)dwFirstEvent;
}

CUserMessageHook* AutoOffset::GetFirstUserMsg()
{
	static DWORD dwFirstUserMsg;

	if (!dwFirstUserMsg)
	{
		dwFirstUserMsg = FindPattern((BYTE*)HwBase, (BYTE*)"\x8B\x35\x00\x00\x00\x00\x85\xF6\x74\x0B", "xx????xxxx");
		dwFirstUserMsg = *(DWORD*)(*(DWORD*)(dwFirstUserMsg + 0x2));
	}

	return (CUserMessageHook*)dwFirstUserMsg;
}

TpfnEventCallback AutoOffset::HookEvent(const char* pszEventName, TpfnEventCallback pfnCallbackFunction) //by sk0r
																										 //but a bit modifyed by me
{
	if ((!pszEventName) || (!pfnCallbackFunction))														 //Hook an event handler
		return NULL;

	CGameEvents* pElement = GetFirstEvent();															 //Get first element
	if (!pElement)
		return NULL;

	while (pElement) {																					 //While pointer not NULL
		if (strcmp(pElement->szName, pszEventName) == 0) {												 //If event name has been found
			TpfnEventCallback pOrig = pElement->pFunction;												 //Backup original function
			pElement->pFunction = pfnCallbackFunction;													 //Replace function address
			return pOrig;
		}

		pElement = pElement->pNext;																		 //Go to next element
	}

	return NULL;
}