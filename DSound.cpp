#include "DSound.h"

DSound::DSound(){}
DSound::DSound( const TCHAR* FileName, IDirectSound8* pDirectSound8){
	load(FileName, pDirectSound8);
}
int DSound::load( const TCHAR* FileName, IDirectSound8* pDirectSound8) {
	WAVEFORMATEX	WaveFormat;
	int				DataSize;
	FILE* fp;
	if (!(fp = ::_tfopen(FileName, _T("rb"))))
		return false;
	char chunkId[5] = {};
	char tmp[5] = {};
	unsigned int chunkSize = 0;
	fread(chunkId, sizeof(char) * 4, 1, fp);
	fread(&chunkSize, sizeof(unsigned int), 1, fp);
	fread(tmp, sizeof(char) * 4, 1, fp);
	if (strcmp(chunkId, "RIFF") || strcmp(tmp, "WAVE"))
		return -1;
	bool fmtchunk = false;
	bool datachunk = false;
	while (true) {
		fread(chunkId, sizeof(char) * 4, 1, fp);
		fread(&chunkSize, sizeof(unsigned int), 1, fp);
		if (!strcmp(chunkId, "fmt ")) {
			if (chunkSize >= sizeof(WAVEFORMATEX)) {
				fread(&WaveFormat, sizeof(WAVEFORMATEX), 1, fp);
				int diff = chunkSize - sizeof(WAVEFORMATEX);
				fseek(fp, diff, SEEK_CUR);
			}
			else {
				memset(&WaveFormat, 0, sizeof(WAVEFORMATEX));
				fread(&WaveFormat, chunkSize, 1, fp);
			}
			fmtchunk = true;
		}
		else if (!strcmp(chunkId, "data")) {
			DataSize = chunkSize;
			WaveData = new byte[chunkSize];
			if (fread(WaveData, sizeof(byte), chunkSize, fp) != chunkSize) {
				fclose(fp);
				return -1;
			}
			datachunk = true;
		}
		else {
			fseek(fp, chunkSize, SEEK_CUR);
		}
		if (fmtchunk && datachunk) break;
	}
	fclose(fp);
	//second buffer
	DSBUFFERDESC				desc = {};
	if (WaveFormat.nChannels == 1) {
		desc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY |
			DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_STATIC;
		desc.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;
	}
	else {
		desc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GETCURRENTPOSITION2 |
			DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_CTRLPAN | DSBCAPS_STATIC;
		desc.guid3DAlgorithm = GUID_NULL;
	}
	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.dwBufferBytes = DataSize;
	desc.lpwfxFormat = &WaveFormat;

	IDirectSoundBuffer* pPrimaryBuffer = NULL;
	pDirectSound8->CreateSoundBuffer(&desc, &pPrimaryBuffer, NULL);
	pPrimaryBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&pSecondaryBuffer);
	pPrimaryBuffer->Release();

	unsigned char* block1 = NULL;
	unsigned char* block2 = NULL;
	unsigned long blockSize1 = 0;
	unsigned long blockSize2 = 0;
	pSecondaryBuffer->Lock(0, DataSize, (void**)&block1, &blockSize1,
		(void**)&block2, &blockSize2, DSBLOCK_ENTIREBUFFER);
	memcpy(block1, WaveData, DataSize);
	pSecondaryBuffer->Unlock(block1, blockSize1, block2, 0);

}
void DSound::play() {
	pSecondaryBuffer->Play(0, 0, 0);
}
void DSound::exit() {
	pSecondaryBuffer->Stop();
	pSecondaryBuffer->Release();
	delete[] WaveData;
}