
#include "Utils.h"

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
	}
	else if (!GetAsyncKeyState(key)) {
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