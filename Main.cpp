#define ATTENTION "SHIT TONE OF TRASH CODE INCLUDED, SOME PARTS OF CODE, INCLUDING PROTECTION FROM ANTI-CHEAT WERE REMOVED, FIX IT BY YOURSELF"

/*===========================================================================

  = Crusader ~ 2000-2002 / Panzer ~ ????-2005 / Hardee ~ 2015-2018          =

  ===========================================================================
  = Hard Hook Reborn - CSO / CSN:Z Hack by Hardee ===========================

  = Credits: ================================================================
  - GameDeception and UnknownCheaters tuts (too much names should be there,
	no time to find them all)
  - JodiRedlot for good working no scope code (eventho i reworked it)
  - Jusic, ot dushi bratishka <3
  - All my niggas from vk.com/hardhook, your patience is really amazing
  - Panzer and Crusader, you guys rock!
  ===========================================================================

  = Features: =
  - XQZ Wallhack
  - ESP (Box, Triangles, Lines, Aim Point)
  - Aimbot, Aim Smooth, Aim Height, Aim FOV, Aim Presets, Team Check
  - Chams (2 modes)
  - Removals (No Smoke, No Flash, No Fog, No Scope)
  - Misc (HUD Hack, Zoom Hack, Additional Crosshair)
  - Pretty dope menu, configs support, offsets-independed, hack protection, 
	MUCH improved code, a lot of my self-coded functions, logging
  - EngineFuncs, ClientFuncs(partialy), many structs from SDK were fixed
  - AutoSpam

  ===========================================================================
  = Undetected against: XignCode3 / BlackCipher2 / HackShield               =
  ===========================================================================

  =========================================================================*/

#define M_PI 3.14159265358979323846					//sorry for mixed russian~english comments, and yes, it's PI

#include "OpenGL32.h"								//опенгл функции
#include "Main.h"									//все остальные функции/переменные
#include "Structs.h"								//структуры

cvars_s					cvar;						//значения
colors_s				colors;						//цвета
keys_s					keys;						//для управления клавишами
menu_s					menu;						//для меню

team_s					team[3];					//вертексы, команды
player_s				player;						//координаты игрока, векторы, высота и прочее

SCREENINFO				g_Screen;					//инфа об экране
cl_clientfunc_t			*g_pClient = nullptr;		//поинтер на клиентские функции
cl_enginefunc_t			*g_pEngine = nullptr;		//поинтер на функции движка
cl_clientfunc_t			g_Client;					//оригинальные функции клиента
cl_enginefunc_t			g_Engine;					//оригинальные функции движка
local_state_s*			g_Local;

usercmd_s*              uCmd;
playermove_s*			pMove;

inits_s					init;

void sys_glMultiTexCoord2fARB(GLenum target, GLfloat s, GLfloat t)
{
	orig_glMultiTexCoord2fARB(target, s, t);
}

void sys_glActiveTextureARB(GLenum target)
{
	orig_glActiveTextureARB(target);
}

void sys_BindTextureEXT(GLenum target, GLuint texture)
{
	orig_BindTextureEXT(target, texture);
}

void sys_glAlphaFunc(GLenum func, GLclampf ref)
{
	(*orig_glAlphaFunc) (func, ref);
}

void sys_glBegin(GLenum mode)
{
	if (cvar.nofog && bDrawnWorld) {
		(*orig_glDisable)(GL_FOG);
	}
	
	if ((mode == GL_TRIANGLE_STRIP || mode == GL_TRIANGLE_FAN) && bDrawnWorld) {								//пиздатый андетект вх + чамсы
		if (cvar.chams) {
			(*orig_glBindTexture)(GL_TEXTURE_2D, 0);
		}
		if (cvar.wallhack && bWall && FoundAnEntity == FALSE) {
			if (!bDrawingEnts) {
				bDrawingEnts = true;																			//энтити отрисовываются (через стену)	
				(*orig_glClear)(GL_DEPTH_BUFFER_BIT);
				//(*orig_glClearDepth)(1.0);																	//альтернативное вх
				//(*orig_glClear)(GL_ACCUM);
			}
		}

		(*orig_glTexEnvi)(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	}

	if (cvar.nosmoke && bDrawnWorld)																			//nosmoke
	{
		if (mode == GL_QUADS)
		{
			GLfloat smokecol[4];
			(*orig_glGetFloatv)(GL_CURRENT_COLOR, smokecol);
			if ((smokecol[0] == smokecol[1]) && (smokecol[0] == smokecol[2]) && (smokecol[0] != 0.0) && (smokecol[0] != 1.0) && (smokecol[0] >= 0.6) && (smokecol[0] <= 0.7)) { //?
				(*orig_glColor4f)(smokecol[0], smokecol[1], smokecol[2], 0.01);
				bSmoke = true;
			} else {
				bSmoke = false;
			}
		}
	}

	if (mode == GL_QUADS && bDrawnWorld)
	{
		if (cvar.noflash)																						//noflash
		{
			GLfloat flashcol[4];
			(*orig_glGetFloatv)(GL_CURRENT_COLOR, flashcol);
			(flashcol[0] == 1.0f && flashcol[1] == 1.0f && flashcol[2] == 1.0f && flashcol[3] > 0.2) ? bFlash = true : bFlash = false;
		}

		if (cvar.hudhack) {																						//hudhack
			float color[4];
			(*orig_glGetFloatv)(GL_CURRENT_COLOR, color);
			bWhite = (color[0] == 1.0f) && (color[1] == 1.0f) && (color[2] == 1.0f) && (color[3] >  0.0f);
			bBlack = (color[0] == 0.0f) && (color[1] == 0.0f) && (color[2] == 0.0f) && (color[3] >= 0.0f);
			if (!(bWhite || bBlack))
				(*orig_glColor4ub)(colors.hudRed, colors.hudGreen, colors.hudBlue, 100);
		}
	}

	if ((mode == GL_POLYGON || mode == GL_QUADS || mode == GL_QUAD_STRIP) && cvar.noscope && bDrawnWorld)		//улучшенный noscope от Жени и меня
	{
		GLfloat scopecol[4];
		(*orig_glGetFloatv)(GL_CURRENT_COLOR, scopecol);
		if (scopecol[0] == 0.0 && scopecol[1] == 0.0 && scopecol[2] == 0.0)
		{
			(*orig_glEnable)(GL_BLEND);
			(*orig_glBlendFunc)(1, 2), (GL_DEPTH_TEST, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA_SATURATE);
			(*orig_glColor4f)(scopecol[0], scopecol[1], scopecol[2], 0.01f);
		}
	}

	if (cvar.chams == 2 && bDrawnWorld) {																		//прозрачные модельки
		if (mode == GL_TRIANGLE_STRIP || mode == GL_TRIANGLE_FAN) {
			(*orig_glEnable)(GL_BLEND);
			(*orig_glBlendFunc)(1, 2), (GL_DEPTH_TEST, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA_SATURATE);
			//(*orig_glBlendFunc)(GL_ONE, GL_ONE);
		}
	}

	(*orig_glBegin) (mode);
}

void sys_glBitmap(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap)
{
	(*orig_glBitmap) (width, height, xorig, yorig, xmove, ymove, bitmap);
}

void sys_glBlendFunc(GLenum sfactor, GLenum dfactor)
{
	(*orig_glBlendFunc) (sfactor, dfactor);
}

void sys_glClear(GLbitfield mask)
{
	(*orig_glClear)(mask);
}

void sys_glClearAccum(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	(*orig_glClearAccum) (red, green, blue, alpha);
}

void sys_glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	(*orig_glClearColor) (red, green, blue, alpha);
}

void sys_glColor3f(GLfloat red, GLfloat green, GLfloat blue)
{
	(*orig_glColor3f) (red, green, blue);
}

void sys_glColor3ub(GLubyte red, GLubyte green, GLubyte blue)
{
	(*orig_glColor3ub) (red, green, blue);
}

void sys_glColor3ubv(const GLubyte *v)
{
	(*orig_glColor3ubv) (v);
}

void sys_glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	(*orig_glColor4f) (red, green, blue, alpha);
}

void sys_glColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
{
	(*orig_glColor4ub) (red, green, blue, alpha);
}

void sys_glCullFace(GLenum mode)
{
	(*orig_glCullFace) (mode);
}

void sys_glDeleteTextures(GLsizei n, const GLuint *textures)
{
	(*orig_glDeleteTextures) (n, textures);
}

void sys_glDepthFunc(GLenum func)
{
	(*orig_glDepthFunc) (func);
}

void sys_glDepthMask(GLboolean flag)
{
	(*orig_glDepthMask) (flag);
}

void sys_glDepthRange(GLclampd zNear, GLclampd zFar)
{
	(*orig_glDepthRange) (zNear, zFar);
}

void sys_glDisable(GLenum cap)
{
	(*orig_glDisable) (cap);
}

void sys_glEnable(GLenum cap)
{
	if (!FirstInit)											    //первый игровой кадр
	{
		FirstInit = true;

		Utils.BuildFont();									    //загружаем точечный шрифт
		Utils.UnvalidVertex();									//сбрасываем вершины
		
		if (ProneFix = 0)
			player_height_min = 0;							    //фикс присевших игроков для дода например
		else if (!ProneFix)
			player_height_min = 40;								//минимальная высота игрока, при которой он не считается "мертвым" /*!ВНИМАНИЕ!*/

		player_height_max = 120;								//максимально допустимая высота игрока

		Utils.SetVerts();										//задаём значения вертексов (альтернатива - проверять в glShadeModel)

		player_vertex_min = Utils.GetVertexMin();			    //получить минимальный вертекс
		player_vertex_max = Utils.GetVertexMax();				//получить максимальный вертекс

		Config.LoadSettings();									//грузим конфиг
		(*orig_glGetIntegerv)(GL_VIEWPORT, vp);					//берём длину/ширину экрана
	}

	if (EnableDraw)												//если viewport вызван 5ый раз и чит загружен (фикс фпс дропов)
	{
		EnableDraw = false;

		if (menu.active)										//1337 OGL меню
			Draw.Menu(40, 30);

		if (cvar.crosshair && !bCross && bDrawnWorld) {			//прицел
			Draw.Crosshair(colors.crossRed, colors.crossGreen, colors.crossBlue, colors.crossAlpha, cvar.crosshairSize, cvar.crosshair);
			bCross = true;
		}

		if (cvar.drawfov && !bFOV && bDrawnWorld) {				//фов для аима
			Draw.Circle(cvar.fov, 1.3f, 250, 250, 250, 100);
			bFOV = true;
		}

		if (cvar.debug)											//список игроков (имя (модель))
			Draw.PlayersInfo(vp[2] - 320, 30);

		/*Draw.HudStringCenter(g_pEngine->GetWindowCenterX(), 
						     ScreenInfo.iHeight - 300, 255, 255, 255, 
			                 "Velocity: %.1f u/s", g_Local->playerstate.velocity);*/
	}

	(*orig_glEnable) (cap);
}

void sys_glEnd(void)
{
	(*orig_glEnd) ();
}

void sys_glFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
	if (cvar.zoomhack && GetAsyncKeyState(keys.zoomkey)) {		//зумхак
		if (cvar.zoomfactor > 9)
			cvar.zoomfactor = 9;

		top = zNear * tan((50 - cvar.zoomfactor * 5) * M_PI / 360);
		bottom = -top;
		left = bottom * 4 / 3;
		right = top * 4 / 3;
	}

	(*orig_glFrustum) (left, right, bottom, top, zNear, zFar);
}

void sys_glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
	(*orig_glOrtho) (left, right, bottom, top, zNear, zFar);
}

void sys_glPopMatrix(void)
{
	if (cvar.wallhack)			//стена была отрисована
		bWall = false;

	if (player.get)             //игрок был отрисован
		player.get = false;

	(*orig_glPopMatrix) ();
}

void sys_glPopName(void)
{
	(*orig_glPopName) ();
}

void sys_glPrioritizeTextures(GLsizei n, const GLuint *textures, const GLclampf *priorities)
{
	(*orig_glPrioritizeTextures) (n, textures, priorities);
}

void sys_glPushAttrib(GLbitfield mask)
{
	(*orig_glPushAttrib) (mask);
}

void sys_glPushClientAttrib(GLbitfield mask)
{
	(*orig_glPushClientAttrib) (mask);
}

void sys_glPushMatrix(void)
{
	if (cvar.wallhack)
		bWall = true;	 //рисуется стена

	(*orig_glPushMatrix) ();
}

void sys_glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	(*orig_glRotatef) (angle, x, y, z);
}

void sys_glShadeModel(GLenum mode)
{
	(*orig_glShadeModel) (mode);

	GLdouble wx, wy, wz, wx2, wy2, wz2, wx3, wy3, wz3;
	GLfloat  color[4];
	(*orig_glGetFloatv)(GL_CURRENT_COLOR, color);

	(*orig_glGetDoublev)(GL_MODELVIEW_MATRIX,  mm);
	(*orig_glGetDoublev)(GL_PROJECTION_MATRIX, pm);
	(*orig_glDisable)(GL_TEXTURE_2D);

	//Здесь начинается отрисовка моделей
	if ((mode == GL_SMOOTH) && !(player.get))
	{

		player.get = true;
		player.highest_z = -99999;
		player.lowest_z = -99999;
		player.vertices = 0;

	} else {

		if (Utils.isWeapon(player.vertices) != "not a weapon") { /* есп на некоторое оружие
																    (с4, гранаты, некоторые гильзы(да, лол, и не фиксится)) */
			gluProject(player.highest_x, player.highest_y, player.highest_z + roffset, mm, pm, vp, &wx, &wy, &wz);
			gluProject(player.lowest_x, player.lowest_y, player.lowest_z, mm, pm, vp, &wx2, &wy2, &wz2);

			float px = (float)(wx / (vp[2] / 2)) - 1;
			float py = (float)(wy / (vp[3] / 2)) - 1;
			float px2 = (float)(wx2 / (vp[2] / 2)) - 1;
			float py2 = (float)(wy2 / (vp[3] / 2)) - 1;

			Draw.WeaponEsp(px, py, px2, py2, player.vertices);
		}

		if (player.get && player.vertices > player_vertex_min && player.height > player_height_min && player.height < player_height_max && player.height < 80)
		{
			if ((player.height > player_height_min) && (player.height < 55))    //игрок сидит?
				//roffset = (26.0f - cvar.duck_h)*(-1.0f);						//используем оффсет, если да
				roffset = cvar.aimheight_d * (-1.0f);
			else
				//roffset = (24.0f - cvar.stand_h)*(-1.0f);
				roffset = cvar.aimheight_s * (-1.0f);

			gluProject(player.highest_x, player.highest_y, player.highest_z + roffset, mm, pm, vp, &wx, &wy, &wz);
			gluProject(player.lowest_x, player.lowest_y, player.lowest_z, mm, pm, vp, &wx2, &wy2, &wz2);
			gluProject(player.highest_x, player.highest_y, player.highest_z, mm, pm, vp, &wx3, &wy3, &wz3);

			GLfloat rasdist[1]; GLfloat headDist[1]; GLfloat footDist[1];

			(*orig_glRasterPos3f)(player.highest_x, player.highest_y, player.highest_z + roffset);
			(*orig_glGetFloatv)(GL_CURRENT_RASTER_DISTANCE, rasdist);
			player.distance = fabs(rasdist[0]);

			(*orig_glRasterPos3f)(player.highest_x, player.highest_y, player.highest_z);
			(*orig_glGetFloatv)(GL_CURRENT_RASTER_DISTANCE, headDist);
			player.headDistance = fabs(headDist[0]);

			(*orig_glRasterPos3f)(player.lowest_x, player.lowest_y, player.lowest_z);
			(*orig_glGetFloatv)(GL_CURRENT_RASTER_DISTANCE, footDist);
			player.footDistance = fabs(footDist[0]);

			player.cDistance = fabs((player.headDistance + player.footDistance) / 2);

			if (cvar.debug) {									    //отладочное есп для поиска вершин
				if (player.vertices > vertMin && player.vertices < vertMax) {
					if (player.height < player_height_min) {
						player.dead = true;
					} else {
						player.dead = false;
						float px2 = (float)(wx2 / (vp[2] / 2)) - 1;
						float py2 = (float)(wy2 / (vp[3] / 2)) - 1;
						Draw.DebugEsp(px2, py2, player.vertices);
					}
				}
			}

			if (Utils.isPlayer(player.vertices)) {				   //если цель - игрок
				if (player.height < player_height_min) {           //когда pronefix - 1, всегда ложно
					player.dead = true;
				} else {

					player.dead = false;
					float px = (float)(wx / (vp[2] / 2)) - 1;
					float py = (float)(wy / (vp[3] / 2)) - 1;
					float pz = (float)(wz / (vp[2] / 2)) - 1;

					float px2 = (float)(wx2 / (vp[2] / 2)) - 1;
					float py2 = (float)(wy2 / (vp[3] / 2)) - 1;
					float pz2 = (float)(wz2 / (vp[2] / 2)) - 1;

					float px3 = (float)(wx3 / (vp[2] / 2)) - 1;
					float py3 = (float)(wy3 / (vp[3] / 2)) - 1;
					float pz3 = (float)(wz3 / (vp[2] / 2)) - 1;

					Draw.PlayerEsp(px3, py3, pz3, px2, py2, pz2, player.vertices, player.cDistance);
																	// ^ рисуем есп на игроке, передаём в функцию его вершины
					Draw.AimPoint(px, py);							//рисуем точку, куда будет идти наводка
				}
			}

			float delx = ((float)((vp[2] / 2) - (wx)));
			float dely = ((float)((vp[3] / 2) - (vp[3] - wy)));

			float lastx2 = ((vp[2] / 2) - ((player.vector_x*vp[2]) / 65535));
			float lasty2 = ((vp[3] / 2) - (((player.vector_y*vp[3]) / 65535) + vp[3]));

			if (AimAt == -1 && player.height > player_height_min && player.height < player_height_max)
			{
				//не включаем в поиск нашу модель
				//если нашли кого-то, проверяем его команду
				if ((player.vertices < player_vertex_max) && (player.vertices > player_vertex_min) && (player.height < player_height_max) && (wx >= vp[2] / 2 - cvar.fov) && (wx <= vp[2] / 2 + cvar.fov) && (wy >= vp[3] / 2 - cvar.fov) && (wy <= vp[3] / 2 + cvar.fov)) {
					if (Utils.isPlayer(player.vertices)) {
						if (((delx + dely) < (lastx2 + lasty2)) || (player.vector_x + player.vector_y) == 0)
						{
							//считаем и задаём векторы движения мыши
							player.vector_x = (float)((wx * 65535) / vp[2]);
							player.vector_y = (float)(((vp[3] - wy) * 65535) / vp[3]);

							TargetX = (float)wx;
							TargetY = (int)vp[3] - (float)wy;
						}

						//позиция модели для проверки на видимость
						player.origin_x = player.highest_x;
						player.origin_y = player.highest_y;
						player.origin_z = player.highest_z + roffset;

						if (Utils.isPlayer(player.vertices) && cvar.target == 0) {
							AimAt = 0; //аим на любого игрока
						} else {
							switch (Utils.GetTeam(player.vertices)) {
							case 1:
								if (cvar.target == 1) //tt
									AimAt = 1;
								break;
							case 2:
								if (cvar.target == 2) //ct
									AimAt = 2;
								break;
							case 3:
								if (cvar.target == 3) //zm
									AimAt = 3;
								break;
							}
						}
					}
				}
			}
		}
	}

	(*orig_glEnable)(GL_TEXTURE_2D);
	(*orig_glColor4f)(color[0], color[1], color[2], color[3]);
}

void sys_glTexCoord2f(GLfloat s, GLfloat t)
{
	(*orig_glTexCoord2f) (s, t);
}

void sys_glTexEnvf(GLenum target, GLenum pname, GLfloat param)
{
	(*orig_glTexEnvf) (target, pname, param);
}

void sys_glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
	(*orig_glTexImage2D) (target, level, internalformat, width, height, border, format, type, pixels);
}

void sys_glTexParameterf(GLenum target, GLenum pname, GLfloat param)
{
	(*orig_glTexParameterf) (target, pname, param);
}

void sys_glTranslated(GLdouble x, GLdouble y, GLdouble z)
{
	(*orig_glTranslated) (x, y, z);
}

void sys_glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
	(*orig_glTranslatef) (x, y, z);
}

void sys_glVertex2f(GLfloat x, GLfloat y)
{
	if (bFlash && cvar.noflash && bDrawnWorld)
	{
		if (x == vp[2] && y == vp[3])					//как только экран заливается сплошным белым цветом (эффект от флешки)
		{
			(*orig_glColor4f)(1.0f, 1.0f, 1.0f, 0.1f);	//заменяем его на цвет с очень маленькой прозрачностью
			bFlash = false;
		}
		return (*orig_glVertex2f) (x, y);
	}
	else
		return (*orig_glVertex2f) (x, y);
}

void sys_glVertex3f(GLfloat x, GLfloat y, GLfloat z)
{
	if (cvar.chams && player.get && bDrawnWorld) 		//новые чамсы
		(*orig_glColor4ub)(colors.chamsRed, colors.chamsGreen, colors.chamsBlue, 150);

	if (player.get)
	{
		player.vertices++;								//считаем все вершины
														//ищем самую большую точку на модели игрока
		if ((z > player.highest_z) || (player.highest_z == -99999))
		{
			player.highest_x = x;
			player.highest_y = y;
			player.highest_z = z;
		}
														//и самую маленькую..
		if ((z < player.lowest_z) || (player.lowest_z == -99999)) {
			player.lowest_x = x;
			player.lowest_y = y;
			player.lowest_z = z;
		}

		player.height = player.highest_z - player.lowest_z;

		(*orig_glEnable)(GL_TEXTURE_2D);
	}

	(*orig_glVertex3f) (x, y, z);
}

void sys_glVertex3fv(const GLfloat *v)
{
	if (!bModulesLoaded)								/* движок/клиент будут хукаться только после того, 
														как игрок зайдёт в инвентарь или на сервер */
	{
		bModulesLoaded = true;
		CreateThread(0, 0, SetupHooks, 0, 0, 0);		//хукаем в отдельном потоке
	}

	ModelViewport = true;
	bDrawnWorld = true;									//мир отрисовывается

	if (cvar.chams && player.get && bDrawnWorld) 		//новые чамсы
		(*orig_glColor4ub)(colors.chamsRed, colors.chamsGreen, colors.chamsBlue, 150);

	if (player.get)
	{
		player.vertices++;								//считаем все вершины
														//ищем самую большую точку на модели игрока
		if ((v[2] > player.highest_z) || (player.highest_z == -99999))
		{
			player.highest_x = v[0];
			player.highest_y = v[1];
			player.highest_z = v[2];
		}
														//и самую маленькую..
		if ((v[2] < player.lowest_z) || (player.lowest_z == -99999)) {
			player.lowest_x = v[0];
			player.lowest_y = v[1];
			player.lowest_z = v[2];
		}

		player.height = player.highest_z - player.lowest_z;

		(*orig_glEnable)(GL_TEXTURE_2D);
	}

	if (cvar.nosmoke && bSmoke && v[0] != player.highest_x && v[1] != player.highest_y && v[2] != player.highest_z &&
		v[0] != player.lowest_x && v[1] != player.lowest_y && v[2] != player.lowest_z) { //1337 smoke check
		return;
	}

	(*orig_glVertex3fv) (v);
}

void sys_glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	ViewportCount++;
	if (ViewportCount >= 5)
		EnableDraw = true;	//когда viewport вызван 5 раз, разрешаем отрисовку текста и прочих графических элементов
	
	if (ModelViewport)
	{	
		//если игрок во вражеской команде и какой либо из методов аима выбран..
		if (AimAt == cvar.target && cvar.aimbot && GetAsyncKeyState(VK_MBUTTON) && bDrawnWorld)
		{
			if (cvar.aimsmooth == 0) {
				mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, player.vector_x, player.vector_y, 0, 0);
			} else {
				if (TargetX && TargetY)						//не работает сук
				{
					POINT Target;
					DWORD ScrX = vp[2] / 2;
					DWORD ScrY = vp[3] / 2;

					if (TargetX > (ScrX)) {					//сука
						Target.x = TargetX - (ScrX);
						Target.x /= cvar.aimsmooth;
						Target.x = +Target.x;
					}

					if (TargetX < (ScrX)) {					//блять
						Target.x = (ScrX)-TargetX;
						Target.x /= cvar.aimsmooth;
						Target.x = -Target.x;
					}

					if (TargetX == (ScrX)) {				//работай
						Target.x = 0;
					}

					if (TargetY > (ScrY)) {					//нахуй
						Target.y = TargetY - (ScrY);
						Target.y /= cvar.aimsmooth;
						Target.y = +Target.y;
					}

					if (TargetY < (ScrY)) {					//блять
						Target.y = (ScrY)-TargetY;
						Target.y /= cvar.aimsmooth;
						Target.y = -Target.y;
					}

					if (TargetY == (ScrY)) {				//АЫЯВОПОПЫЛрп
						Target.y = 0;
					}
				}
			}
		}

		if (cvar.rcs > 0 & bDrawnWorld) {
			POINT CurPos;
			if (GetAsyncKeyState(VK_LBUTTON)) {
				GetCursorPos(&CurPos);
				SetCursorPos(CurPos.x, CurPos.y + cvar.rcs);
			}
		}

		//сброс всего для подсчета в следующем кадре
		player.highest_z = -99999;
		player.lowest_z = -99999;
		AimAt = -1;
		player.vector_x = 0;
		player.vector_y = 0;
		player.x = 0; player.y = 0;
	}

	//тут тоже сброс, по той же причине
	ModelViewport = false;
	bCross = false;
	bFOV = false;

	if (EnableDraw) //проверяем нажаты ли одни из следующих клавиш если >5ый viewport (довольно быстро)
	{
		Utils.HandleKey(keys.menukey);
		Utils.HandleKey(VK_LEFT);
		Utils.HandleKey(VK_RIGHT);
		Utils.HandleKey(VK_UP);
		Utils.HandleKey(VK_DOWN);
	}

	vp[2] = width;   //казалось бы, причём здесь Украина?
	vp[3] = height;	 //и что Титов?

	(*orig_glViewport) (x, y, width, height);
}

PROC sys_wglGetProcAddress(LPCSTR ProcName) //хуя, лол, тут GetProcAddress хук валялся под ногами два года, рофл
{											//спасибо тебе крусадер. от души.
	if (!strcmp(ProcName, "glMultiTexCoord2fARB"))
	{
		orig_glMultiTexCoord2fARB = (func_glMultiTexCoord2fARB_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glMultiTexCoord2fARB;
	}
	else if (!strcmp(ProcName, "glBindTextureEXT"))
	{
		orig_BindTextureEXT = (func_BindTextureEXT_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_BindTextureEXT;
	}
	else if (!strcmp(ProcName, "glActiveTextureARB"))
	{
		orig_glActiveTextureARB = (func_glActiveTextureARB_t)orig_wglGetProcAddress(ProcName);
		return (FARPROC)sys_glActiveTextureARB;
	}
	return orig_wglGetProcAddress(ProcName);
}

void sys_wglSwapBuffers(HDC hDC)
{
	ViewportCount = 0;							//сбрасывает кол-во вызовов viewport, так как это последняя функция в конце каждого кадра
	bDrawnWorld = false;					    //мир не отрисовывается
	bDrawingEnts = false;						//энтити не отрисовываются

	(*orig_wglSwapBuffers) (hDC);
}

int HUD_Redraw()
{
	if (!bFirstFrame)							//первый кадр
	{
		bFirstFrame = true;
		ScreenInfo.iSize = sizeof(SCREENINFO);  //берём инфу
		g_Engine.pfnGetScreenInfo(&ScreenInfo); //об экране

		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)AutoSpam, 0, 0, 0); 
												// ^ Автоспам в чат, отдельным потоком
	}

	vp[2] = ScreenInfo.iWidth; vp[3] = ScreenInfo.iHeight;

	if (cvar.debug)
	{
		hud_player_info_s pinfo;
		cl_entity_s* ent;
		char *name, *model;

		for (int i = 1; i < 33; i++)			//gets playerlist (name/model) into array
		{
			g_Engine.pfnGetPlayerInfo(i, &pinfo);
			ent = g_Engine.GetEntityByIndex(i);
			name = pinfo.name; model = pinfo.model;
			Player[i].Name = name;
			Player[i].Model = model;
		}
	}

	/*if (uCmd)
	{
		if (uCmd->buttons & IN_JUMP)
			Draw.Text(ScreenInfo.iWidth / 2, ScreenInfo.iHeight - 200, 255, 255, 255, "Jump");
	}*/

	return g_Engine.HUD_Redraw();
}

void SetPlayerMove(playermove_s* ppmove)
{
	pMove = ppmove;
	//g_Local->playerstate.velocity = ppmove->velocity;
}

void SetUserCmd(usercmd_s* cmd)
{
	uCmd = cmd;
}

void CL_CreateMove(float frametime, struct usercmd_s *cmd, int active)
{
	g_pClient->CL_CreateMove = g_Client.CL_CreateMove;
	SetUserCmd(cmd);
	//g_Client.CL_CreateMove(frametime, cmd, active);
}

void HUD_PlayerMove(struct playermove_s *ppmove, int server)
{
	g_pClient->HUD_PlayerMove = g_Client.HUD_PlayerMove;
	SetPlayerMove(ppmove);
	//g_Client.HUD_PlayerMove(ppmove, server);
}

int Engine_PlayerMove()
{
	if (!init.playermove) {
		init.playermove = true;
		g_pClient->HUD_PlayerMove = HUD_PlayerMove;
	}

	if (pMove)
	{
		//Log("[Info] my ppmove: 0x%X", &pMove);
	}

	//return 1;
	return g_Engine.HUD_PlayerMove();
}

int Engine_CreateMove()
{
	if (!init.createmove) {
		init.createmove = true;
		g_pClient->CL_CreateMove = CL_CreateMove;
	}

	if (uCmd)
	{
		//Log("[Info] my UserCmd: 0x%X", &uCmd);
	}

	//return 1;
	return g_Engine.CL_CreateMove();
}

void HookClient()
{
	g_pClient->HUD_PlayerMove = HUD_PlayerMove;
	g_pClient->CL_CreateMove = CL_CreateMove;
}

DWORD WINAPI SetupHooks(__in LPVOID lpParameter)
{
	AutoOffset Offset;
	
	while (!Offset.GetRendererInfo())
		Sleep(100);

	g_pClient = (cl_clientfunc_t*)Offset.ClientFuncs();
	g_pEngine = (cl_enginefunc_t*)Offset.EngineFuncs();

Hook:
	memcpy(&g_Client, g_pClient, sizeof(cl_clientfunc_t));
	memcpy(&g_Engine, g_pEngine, sizeof(cl_enginefunc_t));

	if (!g_Client.V_CalcRefdef || !g_Engine.V_CalcShake)
		goto Hook;

	init.createmove = false; init.playermove = false;

	g_pEngine->HUD_Redraw = HUD_Redraw;								   //хук клиента через движок, как тебе такое, Илон Маск?

	/*g_pEngine->CL_CreateMove = Engine_CreateMove;
	g_pEngine->HUD_PlayerMove = Engine_PlayerMove*/;

	//HookClient();

	g_pEngine->pfnClientCmd("gl_vsync 0; cl_minmodels 1; developer 2; fps_max 101; cl_showfps 1;");
	g_pEngine->Con_Printf("Hard Hook Hack by Hardee loaded.\n");
	g_pEngine->Con_Printf("vk.com/hardhook\n");

	return 1;
}

#pragma warning(disable:4100)
BOOL __stdcall DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hOriginalDll);

		//if (Utils.Validate(hinstDll)) {
			hInst = hinstDll;
			//CreateThread(0, 0, ProtectionThread, 0, 0, 0); //1337 Hack Protection Unit (HPU)
			CreateThread(0, 0, SetupHooks, 0, 0, 0);  
			return Init();
		//} else {
		//	Log("=");
		//}
	case DLL_PROCESS_DETACH:
		if (hOriginalDll != NULL)
		{
			Config.SaveSettings();						   //сохраняем настройки чита
			Log("[Hooking] Hard Hook Detached. Exit code: %d\r\n", GetLastError());
			FreeLibrary(hOriginalDll);
			hOriginalDll = NULL;
		}
		break;
	}
	return TRUE;
}
#pragma warning(default:4100)


void __cdecl Log(const char * fmt, ...)					  //spasibo
{
	va_list va_alist;
	char logbuf[256] = "";
	FILE * fp;
	SYSTEMTIME SysTime;
	GetLocalTime(&SysTime);

	va_start(va_alist, fmt);
	_vsnprintf(logbuf + strlen(logbuf), sizeof(logbuf) - strlen(logbuf), fmt, va_alist);
	va_end(va_alist);

	if ((fp = fopen("hardhook.log", "ab")) != NULL)		  //открывает лог файл, пишет туда строку формата [Время] Сообщение
	{
		fprintf(fp, "[%02d:%02d:%02d] %s\r\n", SysTime.wHour, SysTime.wMinute, SysTime.wSecond, logbuf);
		fclose(fp);
	}
}