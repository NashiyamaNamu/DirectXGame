#ifndef _INC_DSound
#define _INC_DSound
#define _CRT_SECURE_NO_WARNINGS
#define D3D_DEBUG_INFO
#define DIRECTSOUND_VERSION 0x800

#pragma comment( lib, "dsound.lib")
#pragma comment( lib, "d3dxof.lib")
#pragma comment( lib, "dxguid.lib")

#include <comdef.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dsound.h>

#include <tchar.h>
#include <vector>

#include <iostream>

class DSound
{
private:
	IDirectSoundBuffer8 * pSecondaryBuffer;
	byte*			WaveData;
public:
	DSound();
	DSound(const TCHAR* FileName, IDirectSound8* pDirectSound8);
	int load(const TCHAR* FileName, IDirectSound8* pDirectSound8);
	void play();
	void exit();
};
#endif