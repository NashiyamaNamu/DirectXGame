#include "DText.h"



DText::DText()
{
}


DText::~DText()
{
	if (pFont != NULL) pFont->Release();
}
void DText::setColor(DWORD c) {
	color = c;
}
void DText::setPosition( int x_, int y_) {
	x = x_;
	y = y_;
}
void DText::Draw(const TCHAR* Str, ...) {
	va_list args;
	va_start( args, Str);
	int len = _vsctprintf( Str, args) + 1;
	if (Buf.size() < (UINT)len) Buf.resize( len);
	_vstprintf( &Buf[ 0], Str, args);
	SetRect( &Rect, 0, 0, 0 ,0);
	pFont->DrawText( NULL, &Buf[ 0], -1, &Rect, DT_LEFT | DT_CALCRECT, color);
	Rect.left += x;
	Rect.right += x;
	Rect.top += y;
	Rect.bottom += y;
	pFont->DrawText( NULL, &Buf[ 0], -1, &Rect, DT_LEFT, color);
}
bool DText::Create( IDirect3DDevice9* pD3DDevice, int FontHeight){
	HFONT hFont = NULL;
	LOGFONT LogFont = {};

	hFont = (HFONT)GetStockObject( DEFAULT_GUI_FONT);

	GetObject( hFont, sizeof( LOGFONT), &LogFont);
	if (FAILED(D3DXCreateFont(pD3DDevice, FontHeight, LogFont.lfWidth, LogFont.lfWeight,
		0, LogFont.lfItalic, LogFont.lfCharSet, LogFont.lfOutPrecision,
		LogFont.lfQuality, LogFont.lfPitchAndFamily, LogFont.lfFaceName, &pFont))) return false;
	return true;
}