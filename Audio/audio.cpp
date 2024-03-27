//#include "stdafx.h
#define _WIN32_DCOM
#include "audio.h"
#include <objbase.h>

#pragma comment(lib, "ole32")
//HRESULT FindMediaFileCch( WCHAR* strDestPath, int cchDest, LPCWSTR strFilename );

CAudio::CAudio()
:m_isRunning(0),
m_pXAudio2(0),
m_pMasteringVoice(0),
m_pSourceVoiceMap(0),
m_pBufferMap(0),
m_pWavMap(0),
m_pbWaveDataMap(0)
{
	//#싱글톤 형식으로 바꾸어야 할것이다
	HRESULT hr;

	UINT32 flags = 0;
	
	m_isRunning = true;
	
	CoInitializeEx( NULL, COINIT_MULTITHREADED );

	if( FAILED( hr = XAudio2Create( &m_pXAudio2, flags ) ) )
    {
		//문제 발생
	}

    if( FAILED( hr = m_pXAudio2->CreateMasteringVoice( &m_pMasteringVoice ) ) )
    {
		//문제 발생 xaudio 해제
		delete m_pXAudio2;       
		CoUninitialize();
    }

	m_pSourceVoiceMap = new IXAUDIOMAP;
	m_pbWaveDataMap = new BYTEMAP;
	m_pBufferMap = new BUFFERMAP;
	m_pWavMap = new WAVEMAP;
}

CAudio::~CAudio()
{

	m_isRunning = false;

	WAVEMAP::iterator wavefileitor = (*m_pWavMap).begin();
	for(  ; wavefileitor != (*m_pWavMap).end() ; wavefileitor++)
	{	
		if(wavefileitor->second != NULL)
		{
			delete wavefileitor->second;
		}
	}

	BUFFERMAP::iterator bufferitor = (*m_pBufferMap).begin();
	for(  ; bufferitor != (*m_pBufferMap).end() ; bufferitor++)
	{	
		if(bufferitor->second != NULL)
		{
			delete bufferitor->second;
		}
	}

	IXAUDIOMAP::iterator sourceitor = (*m_pSourceVoiceMap).begin();
	for(  ; sourceitor != (*m_pSourceVoiceMap).end() ; sourceitor++)
	{	
		if(sourceitor->second != NULL)
		{
			sourceitor->second->DestroyVoice();
		}
	}

	BYTEMAP::iterator waveitor = (*m_pbWaveDataMap).begin();
	for(  ; waveitor != (*m_pbWaveDataMap).end() ; waveitor++)
	{	
		if(waveitor->second != NULL)
		{
			SAFE_DELETE_ARRAY (waveitor->second );
		}
	}

	m_pMasteringVoice->DestroyVoice();
    SAFE_RELEASE( m_pXAudio2 );
	//CoUninitialize();
}

int CAudio::LoadWav( char* szFilename ,  string name)
{
	

	HRESULT hr = S_OK;

	if((*m_pWavMap)[name]  == NULL)
	{
		(*m_pWavMap)[name] = new CWaveFile;
	}

    //
    // Read in the wave file
    //
	if( FAILED( hr = (*m_pWavMap)[name]->Open( szFilename, NULL, WAVEFILE_READ ) ) )
    {
       // wprintf( L"Failed reading WAV file: %#X (%s)\n", hr, strFilePath );
        return hr;
    }

    // Calculate how many bytes and samples are in the wave
    DWORD cbWaveSize = (*m_pWavMap)[name]->GetSize();

    // Read the sample data into memory
	
	(*m_pbWaveDataMap)[name] = new BYTE[ cbWaveSize ];
	
	if( FAILED( hr = (*m_pWavMap)[name]->Read( (*m_pbWaveDataMap)[name], cbWaveSize, &cbWaveSize ) ) )
    {
        //wprintf( L"Failed to read WAV data: %#X\n", hr );
        SAFE_DELETE_ARRAY(  (*m_pbWaveDataMap)[name] );
        return hr;
    }


	if( (*m_pBufferMap)[name]  == NULL)
	{
		(*m_pBufferMap)[name] = new XAUDIO2_BUFFER;
	}

	//초기화
	(*m_pBufferMap)[name]->AudioBytes = 0;
	(*m_pBufferMap)[name]->Flags = 0;
	(*m_pBufferMap)[name]->LoopBegin = 0;
	(*m_pBufferMap)[name]->LoopCount = 0;
	(*m_pBufferMap)[name]->LoopLength = 0;
	(*m_pBufferMap)[name]->pAudioData = 0;
	(*m_pBufferMap)[name]->pContext = 0;
	(*m_pBufferMap)[name]->PlayBegin = 0;
	(*m_pBufferMap)[name]->PlayLength = 0;
	
    
	//설정
	(*m_pBufferMap)[name]->pAudioData = (*m_pbWaveDataMap)[name];
    (*m_pBufferMap)[name]->Flags = XAUDIO2_END_OF_STREAM;  // tell the source voice not to expect any data after this buffer
    (*m_pBufferMap)[name]->AudioBytes = cbWaveSize;

	return 0;
}

int CAudio::Play( string name , float volum)
{
	if(!m_isRunning  || (*m_pWavMap)[name] == NULL)
		return FALSE;

	HRESULT hr = S_OK;

	//객체를 생성뒤 다시 생성하려 할때
	if( (*m_pSourceVoiceMap)[name] != NULL)
	{
		//(*m_pSourceVoiceMap)[name]->Stop();
		(*m_pSourceVoiceMap)[name]->DestroyVoice();
	}

	// Create the source voice    
	if( FAILED( hr = m_pXAudio2->CreateSourceVoice( &(*m_pSourceVoiceMap)[name], (*m_pWavMap)[name]->GetFormat() ) ) )
    {
        //wprintf( L"Error %#X creating source voice\n", hr );
        //SAFE_DELETE_ARRAY( m_pbWaveData );
        return hr;
    }

    if( FAILED( hr = (*m_pSourceVoiceMap)[name]->SubmitSourceBuffer(  (*m_pBufferMap)[name]  ) ) )
    {
        //wprintf( L"Error %#X submitting source buffer\n", hr );
        //m_pSourceVoice->DestroyVoice();
        //SAFE_DELETE_ARRAY( m_pbWaveData );
        return hr;
    }


	hr = (*m_pSourceVoiceMap)[name]->SetVolume(volum,0);
	hr = (*m_pSourceVoiceMap)[name]->Start();

	return TRUE;

}

