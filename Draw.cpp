#pragma once
#pragma comment(lib, "winmm.lib")

#include "Draw.h"
bool bClose;

void DrawGL::AimPoint(float x, float y) { //рисует точки, куда происходит наводка
	Utils.SaveState();
	(*orig_glColor4ub)(250, 250, 250, 250);
	(*orig_glPointSize)(3.0f);
	(*orig_glEnable)(GL_POINT_SMOOTH);
	(*orig_glBegin)(GL_POINTS);
	(*orig_glVertex2f)(x, y);
	(*orig_glEnd)();
	(*orig_glDisable)(GL_POINT_SMOOTH);
	Utils.RestoreState();
}

void DrawGL::Circle(int r, float lw, int red, int green, int blue, int alpha) { //рисует круг
	float PI = 3.1415926535897932384626433832795;
	float color[4];
	(*orig_glGetFloatv)(GL_CURRENT_COLOR, color);
	GLint vp[4];
	(*orig_glGetIntegerv)(GL_VIEWPORT, vp);
	(*orig_glPushMatrix)();
	(*orig_glLoadIdentity)();
	(*orig_glDisable)(GL_TEXTURE_2D);
	(*orig_glEnable)(GL_BLEND);
	(*orig_glBlendFunc)(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	(*orig_glColor4ub)(red, green, blue, alpha);
	(*orig_glLineWidth)(lw);
	(*orig_glBegin)(GL_LINE_LOOP);
	for (double ang = 0; ang <= 2 * PI; ang = ang + 0.1)
		(*orig_glVertex2d)(r * cos(ang) + vp[2] / 2, r * sin(ang) + vp[3] / 2);
	(*orig_glEnd)();
	(*orig_glDisable)(GL_BLEND);
	(*orig_glEnable)(GL_TEXTURE_2D);
	(*orig_glPopMatrix)();
	(*orig_glColor4f)(color[0], color[1], color[2], color[3]);
}

void DrawGL::Crosshair(int red, int green, int blue, int alpha, int size, int type) { //рисует прицел (type 1 - точка, 2 - крестик)
	GLint vp[4];
	float color[4];
	(*orig_glGetFloatv)(GL_CURRENT_COLOR, color);
	(*orig_glDisable)(GL_TEXTURE_2D);
	(*orig_glGetIntegerv)(GL_VIEWPORT, vp);
	(*orig_glColor4ub)(red, green, blue, alpha);
	switch (type) {
	case 1:
		(*orig_glEnable)(GL_POINT_SMOOTH);
		(*orig_glPointSize)((float)size);
		(*orig_glBegin)(GL_POINTS);
		(*orig_glVertex2i)(vp[2] / 2, vp[3] / 2);
		(*orig_glEnd)();
		(*orig_glDisable)(GL_POINT_SMOOTH);
		break;
	case 2:
		(*orig_glLineWidth)(1.5f);
		(*orig_glBegin)(GL_LINES);
		(*orig_glVertex2i)(vp[2] / 2, (vp[3] / 2) - (size * 2));
		(*orig_glVertex2i)(vp[2] / 2, (vp[3] / 2) - 2);
		(*orig_glVertex2i)(vp[2] / 2, (vp[3] / 2) + 2);
		(*orig_glVertex2i)(vp[2] / 2, (vp[3] / 2) + (size * 2));
		(*orig_glVertex2i)((vp[2] / 2) - (size * 2), vp[3] / 2);
		(*orig_glVertex2i)((vp[2] / 2) - 2, vp[3] / 2);
		(*orig_glVertex2i)((vp[2] / 2) + 2, vp[3] / 2);
		(*orig_glVertex2i)((vp[2] / 2) + (size * 2), vp[3] / 2);
		(*orig_glEnd)();
		break;
	}
	(*orig_glEnable)(GL_TEXTURE_2D);
	(*orig_glColor4f)(color[0], color[1], color[2], color[3]);
}

void DrawGL::DebugEsp(float x, float y, long vertcount) { //рисует отладочное есп
	Utils.SaveState();
	(*orig_glColor4ub)(250, 250, 250, 250);
	if (cvar.espdebug) {
		char str[25];
		_itoa(vertcount, str, 10);
		Text(x, y, 255, 255, 255, "Vertices: %s", str);
	}
	Utils.RestoreState();
}

void DrawGL::Line(int x1, int y1, int x2, int y2, int lw, int red, int green, int blue, int alpha) { //рисует линию
	(*orig_glDisable)(GL_TEXTURE_2D);
	(*orig_glEnable)(GL_BLEND);
	(*orig_glBlendFunc)(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	(*orig_glColor4ub)(red, green, blue, alpha);
	(*orig_glLineWidth)((float)lw);
	(*orig_glBegin)(GL_LINES);
	(*orig_glVertex2i)(x1, y1);
	(*orig_glVertex2i)(x2, y2);
	(*orig_glEnd)();
	(*orig_glColor3f)(1.0f, 1.0f, 1.0f);
	(*orig_glEnable)(GL_TEXTURE_2D);
	(*orig_glDisable)(GL_BLEND);
}

void DrawGL::Menu(int x, int y) { //рисует меню
	MenuForm(x, y, 145, 300, 2, 55, 275, 105, 105, 105, 160); //x,y,w,h,linew,lineoffs1,lineoffs2,rgba
	Text(x + 10, y + 20, 255, 255, 255, "Hard Hook Reborn");
	Text(x + 10, y + 30, 255, 255, 255, "CSN:Z Hack by Hardee");
	Text(x + 10, y + 40, 255, 255, 255, "vk.com/hardhook");
	MenuItems(x + 10, y + 73, 40, 185);
	Time(x + 10, y + 290, 255, 255, 255);
}

void DrawGL::MenuForm(int x, int y, int w, int h, int lw, int lineoffset, int lineoffset2, int red, int green, int blue, int alpha) { //рисует каркас меню
	Rect(x, y, w, h, lw, red, green, blue, alpha);
	Line(x, y + lineoffset, x + w, y + lineoffset, lw, 255, 255, 255, 255);
	Line(x, y + lineoffset2, x + w, y + lineoffset2, lw, 255, 255, 255, 255);
}

void DrawGL::PlayersInfo(int x, int y) { //рисует отладочную инфу об игроках
	Draw.Rect(x, y, 240, 380, 2, 105, 105, 105, 160);
	Draw.Line(x, y + 35, x + 240, y + 35, 2, 255, 255, 255, 255);
	Draw.Text(x + 10, y + 20, 255, 255, 255, "Current players:");
	for (int i = 1; i < 33; i++)
		Draw.Text(x + 10, y + 45 + 10 * i, 255, 255, 255, "%s (%s)", Player[i].Name, Player[i].Model);
}

void DrawGL::MenuItems(int x, int y, int menuX1, int menuX2) { //рисует итемы меню
	if (menu.count == 1)
	{
		if (menu.select)
		{
			menu.select = false;
			cvar.wallhack = Utils.ChangeCvar(cvar.wallhack);
		}
		if (cvar.wallhack) { Text(x, y, 0, 255, 0, "Wallhack: On"); }
		else if (!cvar.wallhack) { Text(x, y, 0, 255, 0, "Wallhack: Off"); }
	}
	else if (menu.count != 1)
	{
		if (cvar.wallhack) { Text(x, y, 255, 255, 255, "Wallhack: On"); }
		else if (!cvar.wallhack) { Text(x, y, 255, 255, 255, "Wallhack: Off"); }
	}

	if (menu.count == 2)
	{
		if (menu.select)
		{
			menu.select = false;
			if (menu.select_r)
			{
				cvar.chams++;
				menu.select_r = false;
			}
			else if (menu.select_l)
			{
				cvar.chams--;
				menu.select_l = false;
			}
			if (cvar.chams > 2) { cvar.chams = 0; }
			if (cvar.chams < 0) { cvar.chams = 2; }
		}
		Text(x, y + 10, 0, 255, 0, "Chams: type %i", cvar.chams);
	}
	else if (menu.count != 2)
		Text(x, y + 10, 255, 255, 255, "Chams: type %i", cvar.chams);

	if (menu.count == 3)
	{
		if (menu.select)
		{
			menu.select = false;
			cvar.hudhack = Utils.ChangeCvar(cvar.hudhack);
		}
		if (cvar.hudhack) { Text(x, y + 20, 0, 255, 0, "HUD Hack: On"); }
		else if (!cvar.hudhack) { Text(x, y + 20, 0, 255, 0, "HUD Hack: Off"); }
	}
	else if (menu.count != 3)
	{
		if (cvar.hudhack) { Text(x, y + 20, 255, 255, 255, "HUD Hack: On"); }
		else if (!cvar.hudhack) { Text(x, y + 20, 255, 255, 255, "HUD Hack: Off"); }
	}

	if (menu.count == 4)
	{
		if (menu.select)
		{
			menu.select = false;
			if (menu.select_r)
			{
				cvar.crosshair++;
				menu.select_r = false;
			}
			else if (menu.select_l)
			{
				cvar.crosshair--;
				menu.select_l = false;
			}
			if (cvar.crosshair > 2) { cvar.crosshair = 0; }
			if (cvar.crosshair < 0) { cvar.crosshair = 2; }
		}
		Text(x, y + 30, 0, 255, 0, "Crosshair: type %i", cvar.crosshair);
	}
	else if (menu.count != 4)
		Text(x, y + 30, 255, 255, 255, "Crosshair: type %i", cvar.crosshair);

	if (menu.count == 5)
	{
		if (menu.select)
		{
			menu.select = false;
			cvar.nofog = Utils.ChangeCvar(cvar.nofog);
		}
		if (cvar.nofog) { Text(x, y + 40, 0, 255, 0, "No Fog: On"); }
		else if (!cvar.nofog) { Text(x, y + 40, 0, 255, 0, "No Fog: Off"); }
	}
	else if (menu.count != 5)
	{
		if (cvar.nofog) { Text(x, y + 40, 255, 255, 255, "No Fog: On"); }
		else if (!cvar.nofog) { Text(x, y + 40, 255, 255, 255, "No Fog: Off"); }
	}

	if (menu.count == 6)
	{
		if (menu.select)
		{
			menu.select = false;
			cvar.noscope = Utils.ChangeCvar(cvar.noscope);
		}
		if (cvar.noscope) { Text(x, y + 50, 0, 255, 0, "No Scope: On"); }
		else if (!cvar.noscope) { Text(x, y + 50, 0, 255, 0, "No Scope: Off"); }
	}
	else if (menu.count != 6)
	{
		if (cvar.noscope) { Text(x, y + 50, 255, 255, 255, "No Scope: On"); }
		else if (!cvar.noscope) { Text(x, y + 50, 255, 255, 255, "No Scope: Off"); }
	}

	if (menu.count == 7)
	{
		if (menu.select)
		{
			menu.select = false;
			cvar.nosmoke = Utils.ChangeCvar(cvar.nosmoke);
		}
		if (cvar.nosmoke) { Text(x, y + 60, 0, 255, 0, "No Smoke: On"); }
		else if (!cvar.nosmoke) { Text(x, y + 60, 0, 255, 0, "No Smoke: Off"); }
	}
	else if (menu.count != 7)
	{
		if (cvar.nosmoke) { Text(x, y + 60, 255, 255, 255, "No Smoke: On"); }
		else if (!cvar.nosmoke) { Text(x, y + 60, 255, 255, 255, "No Smoke: Off"); }
	}

	if (menu.count == 8)
	{
		if (menu.select)
		{
			menu.select = false;
			cvar.noflash = Utils.ChangeCvar(cvar.noflash);
		}
		if (cvar.noflash) { Text(x, y + 70, 0, 255, 0, "No Flash: On"); }
		else if (!cvar.noflash) { Text(x, y + 70, 0, 255, 0, "No Flash: Off"); }
	}
	else if (menu.count != 8)
	{
		if (cvar.noflash) { Text(x, y + 70, 255, 255, 255, "No Flash: On"); }
		else if (!cvar.noflash) { Text(x, y + 70, 255, 255, 255, "No Flash: Off"); }
	}

	if (menu.count == 9)
	{
		if (menu.select)
		{
			menu.select = false;
			cvar.aimbot = Utils.ChangeCvar(cvar.aimbot);
		}
		if (cvar.aimbot) { Text(x, y + 80, 0, 255, 0, "Aimbot: On"); }
		else if (!cvar.aimbot) { Text(x, y + 80, 0, 255, 0, "Aimbot: Off"); }
	}
	else if (menu.count != 9)
	{
		if (cvar.aimbot) { Text(x, y + 80, 255, 255, 255, "Aimbot: On"); }
		else if (!cvar.aimbot) { Text(x, y + 80, 255, 255, 255, "Aimbot: Off"); }
	}

	if (menu.count == 10)
	{
		if (menu.select)
		{
			menu.select = false;
			if (menu.select_r)
			{
				cvar.aimsmooth++;
				menu.select_r = false;
			}
			else if (menu.select_l)
			{
				cvar.aimsmooth--;
				menu.select_l = false;
			}
			if (cvar.aimsmooth > 5) { cvar.aimsmooth = 0; }
			if (cvar.aimsmooth < 0) { cvar.aimsmooth = 5; }
		}
		Text(x, y + 90, 0, 255, 0, "Aim Smooth: %i", cvar.aimsmooth);
	}
	else if (menu.count != 10)
		Text(x, y + 90, 255, 255, 255, "Aim Smooth: %i", cvar.aimsmooth);

	if (menu.count == 11)
	{
		if (menu.select)
		{
			menu.select = false;
			if (menu.select_r)
			{
				cvar.aimpreset++;
				menu.select_r = false;
			}
			else if (menu.select_l)
			{
				cvar.aimpreset--;
				menu.select_l = false;
			}
			if (cvar.aimpreset > 3) { cvar.aimpreset = 1; }
			if (cvar.aimpreset < 1) { cvar.aimpreset = 3; }
		}

		switch (cvar.aimpreset) {
		case 1:
			AimPreset = "Head";
			cvar.aimheight_s = 6.0f;
			cvar.aimheight_d = 4.2f;
			break;
		case 2:
			AimPreset = "Chest";
			cvar.aimheight_s = 13.5f;
			cvar.aimheight_d = 14.7f;
			break;
		case 3:
			AimPreset = "Stomach";
			cvar.aimheight_s = 18.5f;
			cvar.aimheight_d = 20.9f;
			break;
		}

		Text(x, y + 100, 0, 255, 0, "Aim Preset: %s", AimPreset);
	}
	else if (menu.count != 11)
		Text(x, y + 100, 255, 255, 255, "Aim Preset: %s", AimPreset);

	if (menu.count == 12)
	{
		if (menu.select)
		{
			menu.select = false;
			if (menu.select_r)
			{
				cvar.target++;
				menu.select_r = false;
			}
			else if (menu.select_l)
			{
				cvar.target--;
				menu.select_l = false;
			}
			if (cvar.target > 3) { cvar.target = 0; }
			if (cvar.target < 0) { cvar.target = 3; }
		}

		switch (cvar.target) {
		case 0:
			AimTeam = "Everyone";
			break;
		case 1:
			AimTeam = "TT";
			break;
		case 2:
			AimTeam = "CT";
			break;
		case 3:
			AimTeam = "ZM";
			break;
		}

		Text(x, y + 110, 0, 255, 0, "Aim Team: %s", AimTeam);
	}
	else if (menu.count != 12)
		Text(x, y + 110, 255, 255, 255, "Aim Team: %s", AimTeam);

	if (menu.count == 13)
	{
		if (menu.select)
		{
			menu.select = false;
			if (menu.select_r)
			{
				cvar.fov += 10;
				menu.select_r = false;
			}
			else if (menu.select_l)
			{
				cvar.fov -= 10;
				menu.select_l = false;
			}
			if (cvar.fov > 650) { cvar.fov = 10; }
			if (cvar.fov < 10) { cvar.fov = 650; }
		}
		Text(x, y + 120, 0, 255, 0, "Aim FOV: %i", cvar.fov);
	}
	else if (menu.count != 13)
		Text(x, y + 120, 255, 255, 255, "Aim FOV: %i", cvar.fov);

	if (menu.count == 14)
	{
		if (menu.select)
		{
			menu.select = false;
			cvar.esptriangles = Utils.ChangeCvar(cvar.esptriangles);
		}
		if (cvar.esptriangles) { Text(x, y + 130, 0, 255, 0, "ESP Triangles: On"); }
		else if (!cvar.esptriangles) { Text(x, y + 130, 0, 255, 0, "ESP Triangles: Off"); }
	}
	else if (menu.count != 14)
	{
		if (cvar.esptriangles) { Text(x, y + 130, 255, 255, 255, "ESP Triangles: On"); }
		else if (!cvar.esptriangles) { Text(x, y + 130, 255, 255, 255, "ESP Triangles: Off"); }
	}

	if (menu.count == 15)
	{
		if (menu.select)
		{
			menu.select = false;
			cvar.esplines = Utils.ChangeCvar(cvar.esplines);
		}
		if (cvar.esplines) { Text(x, y + 140, 0, 255, 0, "ESP Lines: On"); }
		else if (!cvar.esplines) { Text(x, y + 140, 0, 255, 0, "ESP Lines: Off"); }
	}
	else if (menu.count != 15)
	{
		if (cvar.esplines) { Text(x, y + 140, 255, 255, 255, "ESP Lines: On"); }
		else if (!cvar.esplines) { Text(x, y + 140, 255, 255, 255, "ESP Lines: Off"); }
	}

	if (menu.count == 16)
	{
		if (menu.select)
		{
			menu.select = false;
			cvar.espbox = Utils.ChangeCvar(cvar.espbox);
		}
		if (cvar.espbox) { Text(x, y + 150, 0, 255, 0, "ESP Box: On"); }
		else if (!cvar.espbox) { Text(x, y + 150, 0, 255, 0, "ESP Box: Off"); }
	}
	else if (menu.count != 16)
	{
		if (cvar.espbox) { Text(x, y + 150, 255, 255, 255, "ESP Box: On"); }
		else if (!cvar.espbox) { Text(x, y + 150, 255, 255, 255, "ESP Box: Off"); }
	}

	if (menu.count == 17)
	{
		if (menu.select)
		{
			menu.select = false;
			if (menu.select_r)
			{
				cvar.rcs++;
				menu.select_r = false;
			}
			else if (menu.select_l)
			{
				cvar.rcs--;
				menu.select_l = false;
			}
			if (cvar.rcs > 4) { cvar.rcs = 0; }
			if (cvar.rcs < 0) { cvar.rcs = 4; }
		}
		Text(x, y + 160, 0, 255, 0, "RCS: %i", cvar.rcs);
	}
	else if (menu.count != 17)
		Text(x, y + 160, 255, 255, 255, "RCS: %i", cvar.rcs);

	if (menu.count == 18)
	{
		if (menu.select)
		{
			menu.select = false;
			cvar.zoomhack = Utils.ChangeCvar(cvar.zoomhack);
		}
		if (cvar.zoomhack) { Text(x, y + 170, 0, 255, 0, "Zoom Hack: On"); }
		else if (!cvar.zoomhack) { Text(x, y + 170, 0, 255, 0, "Zoom Hack: Off"); }
	}
	else if (menu.count != 18)
	{
		if (cvar.zoomhack) { Text(x, y + 170, 255, 255, 255, "Zoom Hack: On"); }
		else if (!cvar.zoomhack) { Text(x, y + 170, 255, 255, 255, "Zoom Hack: Off"); }
	}

	if (menu.count == 19)
	{
		if (menu.select)
		{
			menu.select = false;
			if (menu.select_r)
			{
				cvar.zoomfactor++;
				menu.select_r = false;
			}
			else if (menu.select_l)
			{
				cvar.zoomfactor--;
				menu.select_l = false;
			}
			if (cvar.zoomfactor > 9) { cvar.zoomfactor = 1; }
			if (cvar.zoomfactor < 1) { cvar.zoomfactor = 9; }
		}
		Text(x, y + 180, 0, 255, 0, "Zoom Factor: %i", cvar.zoomfactor * 5);
	}
	else if (menu.count != 19)
		Text(x, y + 180, 255, 255, 255, "Zoom Factor: %i", cvar.zoomfactor * 5);


	if (menu.count > 19) { menu.count = 1; }
	else if (menu.count < 1) { menu.count = 20; }
}

bool DrawGL::PlayerEsp(float x, float y, float z, float x2, float y2, float z2, long vertcount, double distance) { //рисует есп (боксы, треугольники, линии)
	Utils.SaveState();
	GLint teamColor[4];
	if (cvar.esp) {
		switch (Utils.GetTeam(vertcount)) {
		case 0:
			teamColor[0] = 255; teamColor[1] = 255; teamColor[2] = 255; teamColor[3] = 180;
			break;
		case 1:
			teamColor[0] = 255; teamColor[1] = 0; teamColor[2] = 50; teamColor[3] = 180;
			break;
		case 2:
			teamColor[0] = 66; teamColor[1] = 170; teamColor[2] = 255; teamColor[3] = 180;
			break;
		case 3:
			teamColor[0] = 255; teamColor[1] = 255; teamColor[2] = 0; teamColor[3] = 180;
			break;
		case 4:
			Utils.RestoreState();
			return false;
		}
		if (cvar.esptriangles) {
			(*orig_glColor4ub)(teamColor[0], teamColor[1], teamColor[2], teamColor[3]);
			(*orig_glBegin)(GL_TRIANGLES);         //треугольник
			(*orig_glVertex2f)(x, y + 0.06);
			(*orig_glVertex2f)(x - 0.02, y + 0.1);
			(*orig_glVertex2f)(x + 0.02, y + 0.1);
			(*orig_glEnd)();

			(*orig_glColor4ub)(0, 0, 0, 255);
			(*orig_glLineWidth)(1.5f);
			(*orig_glBegin)(GL_LINE_STRIP);		   //обводка
			(*orig_glVertex2f)(x, y + 0.06);
			(*orig_glVertex2f)(x - 0.02, y + 0.1);
			(*orig_glVertex2f)(x + 0.02, y + 0.1);
			(*orig_glEnd)();
		}
		if (cvar.espbox) {
			float width;
			if (distance >= 0 && distance <= 70)
				bClose = true;
			else
				bClose = false;

			if (distance < 700)
				width = Utils.GetWidth(distance);
			else
				width = 0.015f;

			if (!bClose) {
				(*orig_glColor4ub)(teamColor[0], teamColor[1], teamColor[2], teamColor[3]);
				(*orig_glLineWidth)(1.4f);
				(*orig_glBegin)(GL_LINE_STRIP);
				(*orig_glVertex2f)(x - width, y);
				(*orig_glVertex2f)(x + width, y);
				(*orig_glVertex2f)(x + width, y2);
				(*orig_glVertex2f)(x - width, y2);
				(*orig_glVertex2f)(x - width, y);
				(*orig_glEnd)();
				bClose = true;
			}
		}
		if (cvar.esplines) {
			(*orig_glLineWidth)(1.2);
			(*orig_glHint)(GL_LINE_SMOOTH_HINT, GL_NICEST);
			(*orig_glColor4ub)(0, 255, 0, 150);
			(*orig_glBegin)(GL_LINES);
			(*orig_glEnable)(GL_BLEND);
			(*orig_glBlendFunc)(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			(*orig_glVertex2f)(0.0f, -1.0f);					//1 число - половина высоты
			(*orig_glVertex2f)((x + x2) / 2, (y + y2) / 2);		//среднее арифметическое между головой и пяткой
			(*orig_glEnd)();
		}
	}
	Utils.RestoreState();
	return true;
}

void DrawGL::Rect(int x, int y, int w, int h, int lw, int red, int green, int blue, int alpha) { //рисует прямоугольник
	(*orig_glDisable)(GL_TEXTURE_2D);
	(*orig_glEnable)(GL_BLEND);
	(*orig_glBlendFunc)(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	(*orig_glColor4ub)(red, green, blue, alpha);
	(*orig_glBegin)(GL_QUADS);
	(*orig_glVertex2i)(x, y);
	(*orig_glVertex2i)(x + w, y);
	(*orig_glVertex2i)(x + w, y + h);
	(*orig_glVertex2i)(x, y + h);
	(*orig_glEnd)();
	(*orig_glColor4ub)(255, 255, 255, 255);
	(*orig_glLineWidth)(lw);
	(*orig_glBegin)(GL_LINES);
	(*orig_glVertex2d)(x, y);
	(*orig_glVertex2d)(x + w, y);
	(*orig_glVertex2d)(x + w, y + h);
	(*orig_glVertex2d)(x, y + h);
	(*orig_glVertex2d)(x, y);
	(*orig_glEnd)();
	(*orig_glEnable)(GL_TEXTURE_2D);
	(*orig_glDisable)(GL_BLEND);
}

void DrawGL::Text(int x, int y, int red, int green, int blue, const char* fmt, ...) { //рисует форматированный текст
	char		text[256];
	va_list		ap;
	GLfloat     curcolor[4], position[4];

	if (fmt == NULL)
		return;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);

	(*orig_glPushAttrib)(GL_ALL_ATTRIB_BITS);
	(*orig_glGetFloatv)(GL_CURRENT_COLOR, curcolor);
	(*orig_glGetFloatv)(GL_CURRENT_RASTER_POSITION, position);
	(*orig_glDisable)(GL_TEXTURE_2D);
	(*orig_glColor4f)(0.0f, 0.0f, 0.0f, 1.0f);
	(*orig_glRasterPos2f)(x + 1, y + 1);

	(*orig_glPushAttrib)(GL_LIST_BIT);
	(*orig_glListBase)(base - 32);
	(*orig_glCallLists)(strlen(text), GL_UNSIGNED_BYTE, text);
	(*orig_glPopAttrib)();

	(*orig_glEnable)(GL_TEXTURE_2D);
	(*orig_glDisable)(GL_TEXTURE_2D);
	(*orig_glColor4ub)(red, green, blue, 1.0f);
	(*orig_glRasterPos2f)(x, y);
	(*orig_glColor4ub)(red, green, blue, 1.0f);
	(*orig_glPushAttrib)(GL_LIST_BIT);
	(*orig_glListBase)(base - 32);
	(*orig_glCallLists)(strlen(text), GL_UNSIGNED_BYTE, text);
	(*orig_glPopAttrib)();

	(*orig_glEnable)(GL_TEXTURE_2D);
	(*orig_glPopAttrib)();

	(*orig_glColor4fv)(curcolor);
	(*orig_glRasterPos2f)(position[0], position[1]);
}

void DrawGL::Time(int x, int y, int red, int green, int blue) { //рисует строку со временем
	SYSTEMTIME SysTime;
	GetLocalTime(&SysTime);
	const char* timestring = "%02d:%02d:%02d";
	Text(x, y, red, green, blue, timestring, SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
}

void DrawGL::WeaponEsp(float x, float y, float x2, float y2, long vertcount) { //рисует есп на гранатах и бомбе (забейте, иногда на гильзах)
	Utils.SaveState();
	if (Utils.isWeapon(vertcount) == "c4")
		(*orig_glColor4ub)(0, 255, 0, 200);
	else if (Utils.isWeapon(vertcount) == "grenade")
		(*orig_glColor4ub)(0, 255, 255, 200);

	(*orig_glPointSize)(5.0f);
	(*orig_glEnable)(GL_POINT_SMOOTH);
	(*orig_glBegin)(GL_POINTS);
	(*orig_glVertex2f)((x + x2) / 2, (y + y2) / 2);
	(*orig_glEnd)();
	(*orig_glDisable)(GL_POINT_SMOOTH);
	Utils.RestoreState();
}
