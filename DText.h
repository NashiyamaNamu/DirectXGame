#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#define D3D_DEBUG_INFO	// Direct3Dデバックフラグ

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dxof.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
#include <comdef.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h>
#include <vector>

#include <Windows.h>
#include <tchar.h>


using namespace std;

class DText
{
private:
	ID3DXFont*		pFont;
	RECT			Rect;
	vector<TCHAR>	Buf;
	DWORD color;
	int x, y;

public:
	DText();
	~DText();

	bool Create(IDirect3DDevice9 * pD3DDevice, int FountHeight = 16);
	void setColor( DWORD c);
	void setPosition( int x_, int y_);
	void Draw(const TCHAR* Str, ...);
};

