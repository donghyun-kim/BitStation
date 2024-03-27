#pragma once

#include <xaudio2.h>
#include "SDKwavefile.h"
#include <strsafe.h>
#include <map>
#include <string>

using namespace std;

//--------------------------------------------------------------------------------------
// Helper macros
//--------------------------------------------------------------------------------------
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

typedef map<string ,XAUDIO2_BUFFER* > BUFFERMAP;
typedef map<string ,CWaveFile*> WAVEMAP;
typedef map<string ,BYTE*> BYTEMAP;
typedef map<string ,IXAudio2SourceVoice* >  IXAUDIOMAP;

class CAudio
{
public:
	CAudio();
	~CAudio();

	int LoadWav( char* szFilename , string name);
	int Play(string name,float volum = 1.0f);

private:
	 BOOL m_isRunning;
	 IXAudio2* m_pXAudio2;
	 IXAudio2MasteringVoice* m_pMasteringVoice;


	 IXAUDIOMAP* m_pSourceVoiceMap;
	 BYTEMAP* m_pbWaveDataMap;
	 BUFFERMAP* m_pBufferMap;
	 WAVEMAP* m_pWavMap;
};

