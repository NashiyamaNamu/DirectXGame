#include "Source.h"

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
	int nCmdShow) {
	const TCHAR* WC_BASIC = _T("APPLICATION");

	WNDCLASSEX wcex = { sizeof(WNDCLASSEX),CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, WndProc, 0, 0, hInstance,
				(HICON)LoadImage(NULL, MAKEINTRESOURCE(IDI_APPLICATION),IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED),
				(HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED),
				(HBRUSH)GetStockObject(WHITE_BRUSH), NULL, WC_BASIC, NULL };
	if (!RegisterClassEx(&wcex)) return false;

	HWND hwnd = CreateWindowEx(0, WC_BASIC,
		_T("APPLICATION"), (WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX) | WS_CLIPCHILDREN | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);
	IDirect3D9*			pD3D9 = NULL;
	IDirect3DDevice9*	pDevice3D = NULL;

	pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	D3DDISPLAYMODE Display;
	pD3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display);
	
	D3DPRESENT_PARAMETERS D3DParam = {
		WINDOW_WIDTH, WINDOW_HEIGHT, Display.Format, 1, D3DMULTISAMPLE_NONE, 0,
		D3DSWAPEFFECT_DISCARD, hwnd, TRUE, TRUE, D3DFMT_D24S8, 0, 0, D3DPRESENT_INTERVAL_DEFAULT
	};
	if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DParam.hDeviceWindow,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &pDevice3D)))
		if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DParam.hDeviceWindow,
			D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &pDevice3D)))
			if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, D3DParam.hDeviceWindow,
				D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &pDevice3D)))
				if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, D3DParam.hDeviceWindow,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &pDevice3D))) {
					pD3D9->Release();
					return -1;
				}
	// define sprite
	Sprite sprite1(_T("1.bmp"), pDevice3D);
	sprite1.setSize(48, 48);
	sprite1.setPosition(320, 240);
	player.x = 320;
	player.y = 320;

	DText fpsText;
	fpsText.Create( pDevice3D, 20);
	fpsText.setPosition( 0, 0);
	fpsText.setColor( 0xff000000);

	for (int i = 0; i < BULLET_MAX; i++) {
		bullet[i].sp.init(_T("2.bmp"), pDevice3D);
		bullet[i].sp.setSize(9, 20);
		bullet[i].show = false;
	}
	for (int i = 0; i < ENEMY_MAX; i++) {
		enemy[i].sp.init(_T("5.bmp"), pDevice3D);
		enemy[i].sp.setSize(48, 48);
		enemy[i].sp.setRotate( PI);
		enemy[i].show = false;
	}
	for (int i = 0; i < EFFECT_MAX; i++) {
		bomb[i].sp.init(_T("bomb.bmp"), pDevice3D);
		bomb[i].sp.setSize(96, 96);
		bomb[i].sp.setUVAnimation( 8, 2, 2);
		bomb[i].show = false;
	}
	for (int i = 0; i < PLAYER_BIT_MAX; i++) {
		playerBits[i].sp.init(_T("bit.bmp"), pDevice3D);
		playerBits[i].sp.setRotate( -0.5f+i*0.2f);
		playerBits[i].sp.setSize(22, 22);
		playerBits[i].position = i;
	}


	// load wav
	IDirectSound8* pDirectSound8 = NULL;
	DirectSoundCreate8(NULL, &pDirectSound8, NULL);
	if (FAILED(pDirectSound8->SetCooperativeLevel(hwnd, DSSCL_PRIORITY))) {
		return -1;
	}
	DSound bgm( _T("bgn2.wav"), pDirectSound8);
	DSoundEffect shot( _T("shot.wav"), pDirectSound8);

	Sprite background( _T( "background.bmp"), pDevice3D);
	Sprite background2(_T("background.bmp"), pDevice3D);
	background.setSize( 640, 960);
	background2.setSize(640, 960);

	bgm.play();


	int shotCount = 0;
	int enemySponeCount = 0;

	int fps = 0, fpsCount = 0;
	int beforeTime = 0;

	int backgroundY = 0;


	MSG msg = {};
	while( msg.message != WM_QUIT){
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			DispatchMessage(&msg);
		}else {
			if ( SUCCEEDED( pDevice3D->BeginScene())) {
				DWORD ClearColor = 0xff808080;
				InitRender(pDevice3D, RENDER_TRANS);
				pDevice3D->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,
					ClearColor, 1.0f, 0);
				background.draw( pDevice3D);
				background2.draw( pDevice3D);
				sprite1.draw( pDevice3D);
				for (int i = 0; i < PLAYER_BIT_MAX; i++) playerBits[i].sp.draw( pDevice3D);
				for (int i = 0; i < BULLET_MAX; i++) if ( bullet[ i].show ) bullet[i].sp.draw( pDevice3D);
				for (int i = 0; i < ENEMY_MAX; i++) if ( enemy[ i].show ) enemy[i].sp.draw( pDevice3D);


				InitRender(pDevice3D, RENDER_HALFADD);
				for (int i = 0; i < EFFECT_MAX; i++) if ( bomb[ i].show ) bomb[i].sp.draw( pDevice3D);

				fpsText.Draw(_T("FPS : %d\n"), fps);

				pDevice3D->EndScene();
			}
			pDevice3D->Present(NULL, NULL, NULL, NULL);

			sprite1.setPosition( player.x, player.y);

			UpdateControllerState();
			if (GAME_PAD.bConnected) {
				WORD Buttons = GAME_PAD.state.Gamepad.wButtons;
				player.x += (int)((float)GAME_PAD.state.Gamepad.sThumbLX/(6553.0f)*1.5f);
				player.y -= (int)((float)GAME_PAD.state.Gamepad.sThumbLY/(6553.0f)*1.5f);
				
				if (player.x <     15) player.x = 15;
				if (player.x > 640-15) player.x = 640-15;
				if (player.y <     15) player.y = 15;
				if (player.y > 480-15) player.y = 480-15;

				if (Buttons & XINPUT_GAMEPAD_A && (shotCount > 6 || shotCount == 3)) {
					if (shotCount == 3){
						int index = getBulletIndex();
						bullet[index].show = true;
						bullet[index].type = 0;
						bullet[index].x = player.x;
						bullet[index].y = player.y;
						for (int i = 0; i < PLAYER_BIT_MAX; i++) {
							int index = getBulletIndex();
							bullet[index].show = true;
							bullet[index].type = 2;
							bullet[index].x = playerBits[i].x;
							bullet[index].y = playerBits[i].y;
						}
						shot.play();
					}else{
						for (int i = 0; i < 2; i ++) {
							int index = getBulletIndex();
							bullet[index].show = true;
							bullet[index].type = 1+i;
							bullet[index].x = player.x-10+i*20;
							bullet[index].y = player.y;
						}
						for (int i = 0; i < PLAYER_BIT_MAX; i++) {
							int index = getBulletIndex();
							bullet[index].show = true;
							bullet[index].type = 1;
							bullet[index].x = playerBits[ i].x;
							bullet[index].y = playerBits[i].y;
						}
						shot.play();
						shotCount = 0;
					}
				}

				for (int i = 0; i < PLAYER_BIT_MAX; i++) {
					float x = player.x - 100.0f + i * 40.0f;
					float y = player.y;

					if (Buttons & XINPUT_GAMEPAD_B) {
						y = player.y - 50;
						x = player.x - 10.0f + i * 4.0f;
					}

					float d = 0.1f;
					if (i < 3) {
						y += 40.0f - i * 10.0f;
						d = 0.1f + i * 0.05f;
					}
					if (i > 2) {
						y += -10.0f + i * 10.0f;
						d = 0.1f + (5 - i) * 0.05f;
					}
					playerBits[i].x += (x - playerBits[i].x)*d;
					playerBits[i].y += (y - playerBits[i].y)*d;
					playerBits[i].sp.setPosition(playerBits[i].x, playerBits[i].y);
				}
				shotCount++;
			}
			enemySponeCount++;
			if (enemySponeCount > 100) {
				// summorn enemy
				int index = getEnemyIndex();
				enemy[index].show = true;
				enemy[index].hp = 10;
				enemy[index].x = rand()%640;
				enemy[index].y = -48;

				enemySponeCount = 0;
			}

			for (int i = 0; i < ENEMY_MAX; i++) {
				enemy[ i].y += 3;
				enemy[ i].sp.setPosition( enemy[ i].x, enemy[ i].y);
				if (enemy[ i].y > 480 ) enemy[ i].show = false;
				
				// collision by bulelt
				for (int ii = 0; ii < BULLET_MAX; ii++) {
					float distance = sqrtf((enemy[ i].x - bullet[ ii].x)*(enemy[ i].x - bullet[ ii].x) + (enemy[ i].y - bullet[ii].y)*(enemy[i].y - bullet[ii].y));
					if (distance < 48 && enemy[ i].show && bullet[ ii].show ) {
						// disenable shot
						bullet[ii].show = false;
						// damage for enemy
						enemy[ i].hp --;
						if (enemy[i].hp < 0) {
							createBombs( enemy[ i].x, enemy[ i].y, 4);
							enemy[i].show = false; // earsed
						}
					}
				}
			}
			for (int i = 0; i < BULLET_MAX; i++) {
				if ( bullet[ i].type > 0 ) bullet[i].x += (bullet[i].type-1)*2-1;
				bullet[i].y -= 10;
				bullet[i].sp.setPosition( bullet[i].x, bullet[i].y);
				if (bullet[i].y < -10) bullet[i].show = false;
			}
			for (int i = 0; i < EFFECT_MAX; i++) {
				if (bomb[i].sp.isUVFinish()) bomb[i].show = false;
				bomb[i].sp.setPosition( (int)bomb[i].x, (int)bomb[i].y);
				bomb[i].x = bomb[i].x - cosf(bomb[i].r)*bomb[i].s;
				bomb[i].y = bomb[i].y + sinf(bomb[i].r)*bomb[i].s;
			}

			backgroundY = (backgroundY + 1) % 960;
			background.setPosition( 320, backgroundY);
			background2.setPosition( 320, backgroundY-960);

			// calc fps
			fpsCount++;
			if (clock() - beforeTime > 1000) {
				beforeTime = clock();
				fps = fpsCount;
				fpsCount = 0;
			}
		}
	}
	bgm.exit();
	shot.exit();
	pDirectSound8->Release();
	pDevice3D->Release();
	pD3D9->Release();
	return 0;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}

HRESULT UpdateControllerState() {
	DWORD dwResult;
	dwResult = XInputGetState(0, &GAME_PAD.state);
	if (dwResult == ERROR_SUCCESS) {
		GAME_PAD.bConnected = true;
	}else {
		GAME_PAD.bConnected = false;
	}
	return S_OK;
}
void createBomb( float x, float y, float s, float r, int uvSpeed, int size) {
	int index = getEffectIndex( bomb);
	bomb[index].sp.resetUVC();
	bomb[index].sp.setSize( size, size);
	bomb[index].sp.setUVSpeed( uvSpeed);
	bomb[index].x = x;
	bomb[index].y = y;
	bomb[index].s = s;
	bomb[index].r = r;
	bomb[index].show = true;
}
void createBombs( int x, int y, int num) {
	for (int i = 0; i < num; i++) {
		float r = getRand()*PI*2.0f;
		float s = getRand();
		for (int ii = 0; ii < 4; ii++) {
			createBomb((float)x, (float)y, 0.4*s*(float)ii+0.1, r, ii+1, ii*16+48);
		}
	}
}
int getEffectIndex( EFFECT* eff) {
	for (int i = 0; i < EFFECT_MAX; i++)if (!eff[i].show) return i;
	return 0;
}
int getBulletIndex() {
	for (int i = 0; i < BULLET_MAX; i++)if (!bullet[i].show) return i;
	return 0;
}
int getEnemyIndex() {
	for (int i = 0; i < ENEMY_MAX; i++) if (!enemy[i].show) return i;
	return 0;
}
void InitRender( IDirect3DDevice9* pDevice3D, int type) {
	switch (type)
	{
	case RENDER_TRANS:
		pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pDevice3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;
	case RENDER_ADD:
		pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pDevice3D->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		pDevice3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		break;
	case RENDER_HALFADD:
		pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pDevice3D->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		pDevice3D->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	default:
		break;
	}
}
float getRand() {
	return (float)(rand() % 1000) / 1000.0f;
}