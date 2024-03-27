#include "stdafx.h"
#include "sound_manager.h"
#include "LoadWave.h" 



CSoundManager::CSoundManager()
{

}

CSoundManager::~CSoundManager()
{
	//SAFERELEASE( lpdsBuffer );
    //SAFERELEASE( lpds );
}

int CSoundManager::init(HWND hWnd)
{
	// DirectSound 객체 생성.
    if( FAILED(DirectSoundCreate(NULL, &lpds, NULL)) )
        return ERR;

    // DirectSound의 협력 레벨 설정.    
    if( FAILED(lpds->SetCooperativeLevel(hWnd, DSSCL_NORMAL)) )
        return ERR;

}


int CSoundManager::LoadFile(char* FileName)
{

    // WAV 파일 로드 및 DirectSoundBuffer 생성.
    if( LoadWAV(FileName) == ERR )
        return ERR;
	

	return true;
}
int CSoundManager::Play(DWORD dwFlags)
{

	State = SND_PLAYING; // 현재 사운드의 상태 - "플레이 중".
    
    if( FAILED(lpdsBuffer->Play(0, 0, dwFlags)) )
        return ERR;

    return 1;


}

int CSoundManager::Stop()
{
    State = SND_STOPPED; // 현재 사운드의 상태 - "정지".

    if( FAILED(lpdsBuffer->Stop()) )
        return ERR;

    return 1;

}