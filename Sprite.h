#ifndef _INC_SPRITE
#define _INC_SPRITE
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#pragma comment( lib, "d3d9.lib")
#pragma comment( lib, "d3dx9.lib")
#pragma comment( lib, "d3dxof.lib")
#pragma comment( lib, "dxguid.lib")
#pragma comment( lib, "winmm.lib")

#include <comdef.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h>

#include <Windows.h>
#include <tchar.h>
#include <stdio.h>

class  Sprite {
private:
	IDirect3DTexture9 * pTexture = NULL;
	D3DXVECTOR2 pos;
	struct Vertex {
		float x, y, z;
		float rhw;
		float u, v;
	};
	struct UV {
		int divx;
		int divy;
		int speed;
		int count;
	};
	static const DWORD SPRITE_FVF = D3DFVF_XYZRHW | D3DFVF_TEX1;
	int width = 256;
	int height = 256;
	Vertex vtx[4] = {
		{ pos.x + width / 2, pos.y - height / 2, 0.0f, 1.0f, 1.0f, 0.0f },
		{ pos.x + width / 2, pos.y + height / 2, 0.0f, 1.0f, 1.0f, 1.0f },
		{ pos.x - width / 2, pos.y - height / 2, 0.0f, 1.0f, 0.0f, 0.0f },
		{ pos.x - width / 2, pos.y + height / 2, 0.0f, 1.0f, 0.0f, 1.0f },
	};
	UV uv;
	void updateSprite();
	void updateRotate( Vertex* v);

	float rotate = 0.0f;
public:
	Sprite();
	Sprite( LPCTSTR fname, IDirect3DDevice9* pDevice3D);
	void init(LPCTSTR fname, IDirect3DDevice9* pDevice3D);
	void draw(IDirect3DDevice9* pDevice3D);
	void draw(IDirect3DDevice9* pDevice3D, float r);
	void setSize( int width, int height);
	void setPosition( int _x, int _y);
	void setRotate( float r);

	// UV Animation
	void resetUVC();
	void setUVSpeed( int speed);
	int getUVC();
	bool isUVFinish();
	void setUVAnimation( int x, int y, int speed);
	void runUV();
};
#endif