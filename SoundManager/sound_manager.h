#pragma once

//dx sound를 사용합니다
#include <mmreg.h>
#include <dsound.h>

#pragma comment( lib, "winmm" )   
#pragma comment( lib, "dsound" )   

#ifndef DSBCAPS_CTRLDEFAULT
#define DSBCAPS_CTRLDEFAULT (DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME)
#endif

#define DSVOLUME_TO_DB(Volume)  ((DWORD)(-30*(100-Volume))) // 볼륨 조정 매크로.


#define ERR 0

class CSoundManager
{
public:
	CSoundManager();
	~CSoundManager();

	int init(HWND hWnd);

	int LoadFile(char* FileName);

	int Play(DWORD dwFlags);
	int Stop();

	int LoadWAV(char *FileName, int Flags = DSBCAPS_CTRLDEFAULT);


protected:
	//HWND hWnd;

    int Rate;
    int Size;
    int State;
    
    DSBUFFERDESC dsbd;
    WAVEFORMATEX pcmwf;
    
    LPDIRECTSOUND lpds;
    LPDIRECTSOUNDBUFFER lpdsBuffer;


};