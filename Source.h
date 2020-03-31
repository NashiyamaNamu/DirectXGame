#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define D3D_DEBUG_INFO
#define DIRECTSOUND_VERSION 0x800
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define BULLET_MAX 100
#define ENEMY_MAX 30
#define EFFECT_MAX 50
#define PLAYER_BIT_MAX 6

#define RENDER_DEFAULT 0
#define RENDER_TRANS 1
#define RENDER_ADD 2
#define RENDER_HALFADD 3

#define PI 3.1415926535897932384626

#pragma comment( lib, "d3d9.lib")
#pragma comment( lib, "d3dx9.lib")
#pragma comment( lib, "dsound.lib")
#pragma comment( lib, "d3dxof.lib")
#pragma comment( lib, "dxguid.lib")
#pragma comment( lib, "winmm.lib")
#pragma comment( lib, "xinput.lib")

#include <comdef.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dsound.h>
#include <mmsystem.h>

#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <iostream>
#include <Xinput.h>
#include <cmath>
#include <vector>

#include <time.h>

#include "Sprite.h"
#include "DSound.h"
#include "DSoundEffect.h"
#include "DText.h"

using namespace std;

struct OBJECT {
	int x;
	int y;
};
struct ENEMY {
	Sprite sp;
	bool show;
	int x;
	int y;
	int hp;
};
struct BULLET {
	Sprite sp;
	bool show;
	char type;
	int x;
	int y;
};
struct EFFECT {
	Sprite sp;
	bool show;
	float x;
	float y;
	float r;
	float s;
};
struct PLAYERBIT {
	Sprite sp;
	int position;
	float x;
	float y;
};
struct CONTROLER_STATE {
	XINPUT_STATE state;
	bool bConnected;
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
HRESULT UpdateControllerState();
int getBulletIndex();
int getEnemyIndex();
float getRand();
int getEffectIndex( EFFECT* eff);
void createBomb( float x, float y, float s, float r, int uvSpeed, int size);
void createBombs( int x, int y, int num);
void InitRender(IDirect3DDevice9* pDevice3D, int type);

LPD3DXFONT g_pFont = NULL;

CONTROLER_STATE GAME_PAD;
OBJECT player;
BULLET bullet[BULLET_MAX];
ENEMY enemy[ENEMY_MAX];

EFFECT bomb[EFFECT_MAX];
PLAYERBIT playerBits[PLAYER_BIT_MAX];
