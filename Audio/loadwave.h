// 현재 사운드 상태.
#pragma once

#include <mmsystem.h>
#include <fstream>

using namespace std;

#define SND_STOPPED  0
#define SND_PLAYING   1 
int CSoundManager::LoadWAV(char *FileName, int Flags)
{ 
	/*
    HMMIO               hWAV;  // 웨이브 파일의 핸들.
    MMCKINFO            Parent, Child;
    WAVEFORMATEX		wfmtx;

    // 사운드 버퍼에 저장될 내용을 임시로 기억해둘 장소.
    UCHAR *WholeSndBuff, 
               *PrimaryBuff = NULL, 
               *SecondaryBuff = NULL; 

    DWORD PrimaryLength = 0, SecondaryLength = 0; // 사운드의 길이.

    // Chunck Info 구조체의 내용을 초기화.
    Parent.ckid             = (FOURCC)0;
    Parent.cksize          = 0;
    Parent.fccType        = (FOURCC)0;
    Parent.dwDataOffset = 0;
    Parent.dwFlags        = 0;

    Child = Parent; // Parent의 초기화 내용을 복사.

    // 웨이브 파일을 연다.
	
    hWAV = mmioOpen(FileName, NULL, MMIO_READ | MMIO_ALLOCBUF);

    //hWAV = mmioOpenA(FileName, NULL, MMIO_READ | MMIO_ALLOCBUF);

    // 웨이브 파일 오픈에 실패시,
    if( !hWAV )
        return ERR; // 에러.

    // descend into the RIFF 
    Parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');

    if( mmioDescend(hWAV, &Parent, NULL, MMIO_FINDRIFF) )
    {
        // 에러시,
        mmioClose(hWAV, 0); // 반드시, 현재 열려 있는 WAV 파일을 닫고 return.

        return ERR;  
    }

    // descend to the WAVEfmt 
    Child.ckid = mmioFOURCC('f', 'm', 't', ' ');

    if( mmioDescend(hWAV, &Child, &Parent, 0) )
    {
        // 에러시,
        mmioClose(hWAV, 0); // 반드시, 현재 열려 있는 WAV 파일을 닫고 return.

        return ERR;  
    }

    // 웨이브 파일의 format을 읽어온다.
    if( mmioRead(hWAV, (char *)&wfmtx, sizeof(wfmtx)) != sizeof(wfmtx) )
    {
        // 에러시,
        mmioClose(hWAV, 0); // 반드시, 현재 열려 있는 WAV 파일을 닫고 return.

        return ERR;
    }

    // 웨이브 파일은 반드시 wFormatTag가 WAVE_FORMAT_PCM이어야 한다.
    if( wfmtx.wFormatTag != WAVE_FORMAT_PCM )
    {
        // 에러시,
        mmioClose(hWAV, 0); // 반드시, 현재 열려 있는 WAV 파일을 닫고 return.

        return ERR; 
    }

    if( mmioAscend(hWAV, &Child, 0) )
    {
       // 에러시,
       mmioClose(hWAV, 0); // 반드시, 현재 열려 있는 WAV 파일을 닫고 return.

       return ERR;  
    }

    Child.ckid = mmioFOURCC('d', 'a', 't', 'a');

    if( mmioDescend(hWAV, &Child, &Parent, MMIO_FINDCHUNK) )
    {
        // 에러시,
        mmioClose(hWAV, 0); // 반드시, 현재 열려 있는 WAV 파일을 닫고 return.

        return ERR;  
    }

    // WAV의 사운드 데이터 크기만큼을 메모리에 할당한다.
    WholeSndBuff = new UCHAR[ Child.cksize ];

    // 할당된 메모리에 WAV 데이터를 읽어 온다.
    mmioRead(hWAV, (char *)WholeSndBuff, Child.cksize);

    // 파일을 닫는다.
    mmioClose(hWAV, 0);

    // 사운드 정보 기록.
    Rate  = wfmtx.nSamplesPerSec;
    Size  = Child.cksize;
    State = SND_STOPPED; // 현재 사운드 상태 - "정지".

    // 사운드 파일의 format 정보 기록.
    memset(&pcmwf, 0, sizeof(WAVEFORMATEX));

    pcmwf.wFormatTag        = WAVE_FORMAT_PCM;  // 항상 WAVE_FORMAT_PCM으로 채워준다.
    pcmwf.nChannels           = 1; // MONO 
    pcmwf.nSamplesPerSec  = 11025; // 11 khz. 거의 모든 경우 11025.
    pcmwf.nBlockAlign          = 1;                
    pcmwf.nAvgBytesPerSec = pcmwf.nSamplesPerSec * pcmwf.nBlockAlign;
    pcmwf.wBitsPerSample   = 8; // 비트 수.
    pcmwf.cbSize                 = 0; // cbSize는 항상 0.

    // 사운드 버퍼 정보 기록.
    dsbd.dwSize   = sizeof(DSBUFFERDESC);
    dsbd.dwFlags  = Flags | DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE;
    dsbd.dwBufferBytes = Child.cksize;
    dsbd.lpwfxFormat = &pcmwf;

    // 사운드 버퍼 생성.
    if( FAILED(lpds->CreateSoundBuffer(&dsbd,&lpdsBuffer,NULL)) )
    {
       // 에러시,
       delete[] WholeSndBuff; // 반드시 할당된 메모리를 해제한 후 리턴.

       return ERR;
    }

    // 사운드 버퍼에 잠금 설정.
    if( FAILED(lpdsBuffer->Lock(0, Child.cksize, (LPVOID*)&PrimaryBuff, &PrimaryLength, 
                                                                    (LPVOID*)&SecondaryBuff, &SecondaryLength, 
                                                                    DSBLOCK_FROMWRITECURSOR)) )
        return ERR;

    // 버퍼에 사운드 정보 복사.
    memcpy(PrimaryBuff, WholeSndBuff, PrimaryLength); // 순환 버퍼의 앞부분을 복사하고,
    memcpy(SecondaryBuff, (WholeSndBuff+PrimaryLength), SecondaryLength); // 순환 버퍼의 뒷부분을 복사한다.

    // 사운드 버퍼에 잠금 해제.
    if( FAILED(lpdsBuffer->Unlock(PrimaryBuff, PrimaryLength, SecondaryBuff, SecondaryLength)) )
        return ERR;

    delete[] WholeSndBuff; // 반드시 할당된 메모리를 해제한 후 리턴.

    return 1;
	*/

	fstream     kWave;

	// Wave 파일 열기 
	kWave.open (FileName, ios::in | ios::binary);

	if (kWave.is_open ())
	{
		char aiID [5];

		// "RIFF" 문자열과 비교 
		kWave.read (aiID, sizeof (char) * 4);
		aiID [4] = '\0';


		//"RIFF"가 아면 리턴 에러
		if (0 != strcmp (aiID, "RIFF"))
		{
			return ERR;
		}

		int iSize;
		//Wave 파일의 크기를 읽어 들인다. 
		kWave.read ((char *) &iSize, sizeof (int));

		//"WAVE" 문자열과 비교 
		kWave.read (aiID, sizeof (char) * 4);
		aiID [4] = '\0';

	
		//"WAVW"가 아니면 리턴 에러
		if (0 != strcmp (aiID, "WAVE"))
		{
			return ERR;
		}

		//포멧 청크 식별자를 무시한다. 
		kWave.seekg (4, ios::cur);

		//포멧 정보를 읽어 들인다.  
		int iFormatLength;
		kWave.read ((char *) &iFormatLength, sizeof (int));

		//WAVEFORMATEX 구조체만큼 읽어 들인다. 이는 기존 구조체에서 2바이트를 더읽어들이게 된다. 
		WAVEFORMATEX kWaveFormat;

		kWave.read ((char *) &kWaveFormat, sizeof (WAVEFORMATEX));
		//구조체에서 읽어들인 2바이트 이외에 다시 2바이트를 무시 해야 한다.
		kWave.seekg (2, ios::cur);
		kWaveFormat.cbSize = 0;
	

		//웨이브 버퍼 크기를 읽어들인다.
		int iDataSize;
		kWave.read ((char *) &iDataSize, sizeof (int));
		


		//사운드 버퍼를 읽는데 필요한 메모리를 할당한 후 읽는다.
		UCHAR * pkSoundBuffer = new UCHAR [iDataSize];
		kWave.read ((char *) pkSoundBuffer, iDataSize);
		
		State = SND_STOPPED; // 현재 사운드 상태 - "정지".
		
		//DirectSound Buffer을 생성하고 웨이브데이터를 복사한다.
		DSBUFFERDESC         kBufferDesc;

		ZeroMemory (&kBufferDesc, sizeof (DSBUFFERDESC));
		kBufferDesc.dwSize        = sizeof (DSBUFFERDESC);   // 구조체의 크기를 나타낸다.
		kBufferDesc.dwBufferBytes = iDataSize;  // 버퍼 웨이브 데이터의 크기를 나타낸다.
		kBufferDesc.lpwfxFormat   = &kWaveFormat; // 파일에서 읽어 들였던 구조체가 사용된다.
		kBufferDesc.dwFlags       = DSBCAPS_CTRLVOLUME;  // 버퍼가 갖는 플래그를 설정한다.


		// 버퍼를 생성한다. 
		if (FAILED (lpds->CreateSoundBuffer (&kBufferDesc, &lpdsBuffer, NULL)) )
		{
			return ERR;
		}

		// 버퍼를 잠근다.
		LPVOID lpvAudio;
		DWORD  dwBytes;

		if (FAILED (lpdsBuffer->Lock(0, 0,&lpvAudio, &dwBytes, NULL, NULL, DSBLOCK_ENTIREBUFFER)) )
		{
			return ERR;
		}

		// 버퍼를 복사한다. 
		memcpy (lpvAudio, pkSoundBuffer, dwBytes);
		lpdsBuffer->Unlock(lpvAudio, dwBytes, NULL, 0);


		// 버퍼 메모리를 삭제한다. 
		delete [] pkSoundBuffer;
		kWave.close ();
		
		
	}



return 1;

} // end LoadWAV.
