#ifndef _INC_DSoundEffect
#define _INC_DSoundEffect
#define DSOUNDEFFECT_MAX 5

#include "DSound.h"

class DSoundEffect
{
private:
	struct Origin{
		DSound se;
	};
	Origin sounds[ DSOUNDEFFECT_MAX];
	int index = 0;
public:
	DSoundEffect(const TCHAR* FileName, IDirectSound8* pDirectSound8);
	void play();
	void exit();
};
#endif
