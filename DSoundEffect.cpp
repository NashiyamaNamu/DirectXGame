#include "DSoundEffect.h"



DSoundEffect::DSoundEffect(const TCHAR* FileName, IDirectSound8* pDirectSound8){
	memset(sounds, NULL, sizeof(sounds));
	for (int i = 0; i < DSOUNDEFFECT_MAX; i++) sounds[i].se.load( FileName, pDirectSound8);
}
void DSoundEffect::play() {
	sounds[index].se.play();
	index = (index + 1) % DSOUNDEFFECT_MAX;
}
void DSoundEffect::exit() {
	for (int i = 0; i < DSOUNDEFFECT_MAX; i++) sounds[i].se.exit();
}
