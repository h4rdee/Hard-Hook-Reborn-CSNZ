#pragma once

#ifndef MYSTRUCT_H
#define MYSTRUCT_H

struct inits_s {
	int createmove, playermove;
};

struct cvars_s {				//квары
	int wallhack;
	int nosmoke, noflash;
	int chams;
	int crosshair, crosshairSize;
	int hudhack, transp;
	int zoomhack, zoomfactor;
	int aimbot, aimsmooth, aimpreset, rcs, fov, drawfov;
	int noscope, aimthrough, autoshot, nofog, target;
	int esp, esptriangles, esplines, espbox;
	int fullbright;

	float stand_h, duck_h, aimdiff, aimheight_s, aimheight_d;

	/* Developer Stuff */
	int debug, espdebug, autospam;
};

struct colors_s {
	int hudRed, hudGreen, hudBlue;
	int crossRed, crossGreen, crossBlue, crossAlpha;
	int chamsRed, chamsGreen, chamsBlue;
};

struct menu_s {					//для управления меню
	int count;
	bool up, down, select, select_r, select_l, active;
};

struct keys_s {					//клавиши для меню
	bool up, down, left, right, active;
	int aimkey, zoomkey, speedkey, menukey;
};

struct team_s {			//хранение вертексов
	char* name[52];
	int verts[30];

	/* немного дерьма */
	/*int vert01, vert02, vert03, vert04, vert05, vert06, vert07, vert08, vert09, vert10, vert11, vert12;
	int	vert13, vert14, vert15, vert16, vert17, vert18, vert19, vert20, vert21, vert22, vert23, vert24;
	int vert25, vert26, vert27, vert28, vert29, vert30;*/
};

struct player_s {		//данные о модели игрока
	double highest_x, highest_y, highest_z;
	double lowest_x, lowest_y, lowest_z;
	double vector_x, vector_y, vector_z;
	double origin_x, origin_y, origin_z;
	double distance, cDistance, headDistance, footDistance, height;
	double x, y, z;
	long vertices;
	long headX, headY;
	bool dead, get, visible;
};

//struct offset_s {		//хранение оффсетов
//	float s, d;
//	char npart1[52];
//	char npart2[52];
//	char npart3[52];
//	char npart4[52];
//	char npart5[52];
//	char name[256];
//};

const int vertMin = 1500;
const int vertMax = 5000;

#endif