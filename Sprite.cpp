#include "Sprite.h"
Sprite::Sprite(){}
Sprite::Sprite( LPCTSTR fname, IDirect3DDevice9* pDevice3D) {
	D3DXCreateTextureFromFileEx(pDevice3D, fname, 0, 0, 0, 0, D3DFMT_A1R5G5B5, D3DPOOL_MANAGED,
			D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR, D3DCOLOR_ARGB( 255, 0, 0, 0), NULL, NULL, &pTexture);

	pos.x = WINDOW_WIDTH/2.0f;
	pos.y = WINDOW_HEIGHT/2.0f;
	setUVAnimation(1, 1, 1);
}
void Sprite::init(LPCTSTR fname, IDirect3DDevice9* pDevice3D) {
	D3DXCreateTextureFromFileEx(pDevice3D, fname, 0, 0, 0, 0, D3DFMT_A1R5G5B5, D3DPOOL_MANAGED,
		D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR, D3DCOLOR_ARGB(255, 0, 0, 0), NULL, NULL, &pTexture);

	pos.x = WINDOW_WIDTH / 2.0f;
	pos.y = WINDOW_HEIGHT / 2.0f;

	setUVAnimation( 1, 1, 1);
}
void Sprite::draw(IDirect3DDevice9* pDevice3D) {
	pDevice3D->SetTexture(0, pTexture);
	pDevice3D->SetFVF(SPRITE_FVF);
	pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtx, sizeof(Vertex));
	runUV();
}
void Sprite::setSize(int _width, int _height) {
	width = _width;
	height = _height;
	updateSprite();
}
void Sprite::setPosition(int _x, int _y) {
	pos.x = (float)_x;
	pos.y = (float)_y;
	updateSprite();
}
void Sprite::setRotate( float r) {
	rotate = r;
}
void Sprite::updateRotate(Vertex* v) {
	for (int i = 0; i < 4; i++) {
		float x = v[i].x * cosf(rotate) - v[i].y*sinf(rotate);
		float y = v[i].x * sinf(rotate) + v[i].y*cosf(rotate);
		v[i].x = x + pos.x;
		v[i].y = y + pos.y;
	}
}

void Sprite::resetUVC() {
	uv.count = 0;
}
void Sprite::setUVSpeed( int speed) {
	uv.speed = speed;
}
void Sprite::setUVAnimation( int x, int y, int speed) {
	uv.divx = x;
	uv.divy = y;
	uv.speed = speed;
	uv.count = 0;

	if ( x < 1) uv.divx = 1;
	if ( y < 1) uv.divy = 1;
	if (speed < 1) uv.speed = 1;
	updateSprite();
}
void Sprite::runUV() {
	uv.count = (uv.count + 1) % ( uv.divx * uv.divy*uv.speed);
}
void Sprite::updateSprite() {
	float dx = 1.0f/uv.divx;
	float dy = 1.0f/uv.divy;
	int ax = (int)(uv.count/uv.speed) % uv.divx;
	int ay = (int)((int)(uv.count/uv.speed) / uv.divx);

	vtx[0] = { (float)(width / 2), (float)(height / -2), 0.0f, 1.0f, dx*ax+dx	, dy*ay };
	vtx[1] = { (float)(width / 2), (float)(height / 2), 0.0f, 1.0f, dx*ax+dx	, dy*ay +dy };
	vtx[2] = { (float)(width / -2), (float)(height / -2), 0.0f, 1.0f, dx*ax	, dy*ay };
	vtx[3] = { (float)(width / -2), (float)(height / 2), 0.0f, 1.0f, dx*ax	, dy*ay +dy };

	updateRotate( vtx);
}
int Sprite::getUVC() {
	return uv.count;
}
bool Sprite::isUVFinish() {
	if (getUVC() >= (uv.divx * uv.divy-1)*uv.speed) return true;
	return false;
}