#pragma once

#ifndef MYSTRUCT_H
#define MYSTRUCT_H

struct inits_s {
	int createmove, playermove;
};

struct cvars_s {				//�����
	int wallhack;
	int nosmoke, noflash;
	int chams;
	int crosshair, crosshairSize;
	int hudhack, transp;
	int zoomhack, zoomfactor;
	int aimbot, aimsmooth, aimpreset, rcs, fov, drawfov;
	int noscope, aimthrough, autoshot, nofog, target;
	int esp, esptriangles, esplines, espbox;

	float stand_h, duck_h, aimdiff, aimheight_s, aimheight_d;

	/* Developer Stuff */
	int debug, espdebug;
};

struct colors_s {
	int hudRed, hudGreen, hudBlue;
	int crossRed, crossGreen, crossBlue, crossAlpha;
	int chamsRed, chamsGreen, chamsBlue;
};

struct menu_s {					//��� ���������� ����
	int count;
	bool up, down, select, select_r, select_l, active;
};

struct keys_s {					//������� ��� ����
	bool up, down, left, right, active;
	int aimkey, zoomkey, speedkey, menukey;
};

struct team_s {			//�������� ���������
	char* name[52];
	int verts[30];
};

struct player_s {		//������ � ������ ������
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

const int vertMin = 1500;
const int vertMax = 5000;

#endif