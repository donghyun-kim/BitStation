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
	// DirectSound ��ü ����.
    if( FAILED(DirectSoundCreate(NULL, &lpds, NULL)) )
        return ERR;

    // DirectSound�� ���� ���� ����.    
    if( FAILED(lpds->SetCooperativeLevel(hWnd, DSSCL_NORMAL)) )
        return ERR;

}


int CSoundManager::LoadFile(char* FileName)
{

    // WAV ���� �ε� �� DirectSoundBuffer ����.
    if( LoadWAV(FileName) == ERR )
        return ERR;
	

	return true;
}
int CSoundManager::Play(DWORD dwFlags)
{

	State = SND_PLAYING; // ���� ������ ���� - "�÷��� ��".
    
    if( FAILED(lpdsBuffer->Play(0, 0, dwFlags)) )
        return ERR;

    return 1;


}

int CSoundManager::Stop()
{
    State = SND_STOPPED; // ���� ������ ���� - "����".

    if( FAILED(lpdsBuffer->Stop()) )
        return ERR;

    return 1;

}