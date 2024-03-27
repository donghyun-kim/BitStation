// ���� ���� ����.
#pragma once

#include <mmsystem.h>
#include <fstream>

using namespace std;

#define SND_STOPPED  0
#define SND_PLAYING   1 
int CSoundManager::LoadWAV(char *FileName, int Flags)
{ 
	/*
    HMMIO               hWAV;  // ���̺� ������ �ڵ�.
    MMCKINFO            Parent, Child;
    WAVEFORMATEX		wfmtx;

    // ���� ���ۿ� ����� ������ �ӽ÷� ����ص� ���.
    UCHAR *WholeSndBuff, 
               *PrimaryBuff = NULL, 
               *SecondaryBuff = NULL; 

    DWORD PrimaryLength = 0, SecondaryLength = 0; // ������ ����.

    // Chunck Info ����ü�� ������ �ʱ�ȭ.
    Parent.ckid             = (FOURCC)0;
    Parent.cksize          = 0;
    Parent.fccType        = (FOURCC)0;
    Parent.dwDataOffset = 0;
    Parent.dwFlags        = 0;

    Child = Parent; // Parent�� �ʱ�ȭ ������ ����.

    // ���̺� ������ ����.
	
    hWAV = mmioOpen(FileName, NULL, MMIO_READ | MMIO_ALLOCBUF);

    //hWAV = mmioOpenA(FileName, NULL, MMIO_READ | MMIO_ALLOCBUF);

    // ���̺� ���� ���¿� ���н�,
    if( !hWAV )
        return ERR; // ����.

    // descend into the RIFF 
    Parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');

    if( mmioDescend(hWAV, &Parent, NULL, MMIO_FINDRIFF) )
    {
        // ������,
        mmioClose(hWAV, 0); // �ݵ��, ���� ���� �ִ� WAV ������ �ݰ� return.

        return ERR;  
    }

    // descend to the WAVEfmt 
    Child.ckid = mmioFOURCC('f', 'm', 't', ' ');

    if( mmioDescend(hWAV, &Child, &Parent, 0) )
    {
        // ������,
        mmioClose(hWAV, 0); // �ݵ��, ���� ���� �ִ� WAV ������ �ݰ� return.

        return ERR;  
    }

    // ���̺� ������ format�� �о�´�.
    if( mmioRead(hWAV, (char *)&wfmtx, sizeof(wfmtx)) != sizeof(wfmtx) )
    {
        // ������,
        mmioClose(hWAV, 0); // �ݵ��, ���� ���� �ִ� WAV ������ �ݰ� return.

        return ERR;
    }

    // ���̺� ������ �ݵ�� wFormatTag�� WAVE_FORMAT_PCM�̾�� �Ѵ�.
    if( wfmtx.wFormatTag != WAVE_FORMAT_PCM )
    {
        // ������,
        mmioClose(hWAV, 0); // �ݵ��, ���� ���� �ִ� WAV ������ �ݰ� return.

        return ERR; 
    }

    if( mmioAscend(hWAV, &Child, 0) )
    {
       // ������,
       mmioClose(hWAV, 0); // �ݵ��, ���� ���� �ִ� WAV ������ �ݰ� return.

       return ERR;  
    }

    Child.ckid = mmioFOURCC('d', 'a', 't', 'a');

    if( mmioDescend(hWAV, &Child, &Parent, MMIO_FINDCHUNK) )
    {
        // ������,
        mmioClose(hWAV, 0); // �ݵ��, ���� ���� �ִ� WAV ������ �ݰ� return.

        return ERR;  
    }

    // WAV�� ���� ������ ũ�⸸ŭ�� �޸𸮿� �Ҵ��Ѵ�.
    WholeSndBuff = new UCHAR[ Child.cksize ];

    // �Ҵ�� �޸𸮿� WAV �����͸� �о� �´�.
    mmioRead(hWAV, (char *)WholeSndBuff, Child.cksize);

    // ������ �ݴ´�.
    mmioClose(hWAV, 0);

    // ���� ���� ���.
    Rate  = wfmtx.nSamplesPerSec;
    Size  = Child.cksize;
    State = SND_STOPPED; // ���� ���� ���� - "����".

    // ���� ������ format ���� ���.
    memset(&pcmwf, 0, sizeof(WAVEFORMATEX));

    pcmwf.wFormatTag        = WAVE_FORMAT_PCM;  // �׻� WAVE_FORMAT_PCM���� ä���ش�.
    pcmwf.nChannels           = 1; // MONO 
    pcmwf.nSamplesPerSec  = 11025; // 11 khz. ���� ��� ��� 11025.
    pcmwf.nBlockAlign          = 1;                
    pcmwf.nAvgBytesPerSec = pcmwf.nSamplesPerSec * pcmwf.nBlockAlign;
    pcmwf.wBitsPerSample   = 8; // ��Ʈ ��.
    pcmwf.cbSize                 = 0; // cbSize�� �׻� 0.

    // ���� ���� ���� ���.
    dsbd.dwSize   = sizeof(DSBUFFERDESC);
    dsbd.dwFlags  = Flags | DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE;
    dsbd.dwBufferBytes = Child.cksize;
    dsbd.lpwfxFormat = &pcmwf;

    // ���� ���� ����.
    if( FAILED(lpds->CreateSoundBuffer(&dsbd,&lpdsBuffer,NULL)) )
    {
       // ������,
       delete[] WholeSndBuff; // �ݵ�� �Ҵ�� �޸𸮸� ������ �� ����.

       return ERR;
    }

    // ���� ���ۿ� ��� ����.
    if( FAILED(lpdsBuffer->Lock(0, Child.cksize, (LPVOID*)&PrimaryBuff, &PrimaryLength, 
                                                                    (LPVOID*)&SecondaryBuff, &SecondaryLength, 
                                                                    DSBLOCK_FROMWRITECURSOR)) )
        return ERR;

    // ���ۿ� ���� ���� ����.
    memcpy(PrimaryBuff, WholeSndBuff, PrimaryLength); // ��ȯ ������ �պκ��� �����ϰ�,
    memcpy(SecondaryBuff, (WholeSndBuff+PrimaryLength), SecondaryLength); // ��ȯ ������ �޺κ��� �����Ѵ�.

    // ���� ���ۿ� ��� ����.
    if( FAILED(lpdsBuffer->Unlock(PrimaryBuff, PrimaryLength, SecondaryBuff, SecondaryLength)) )
        return ERR;

    delete[] WholeSndBuff; // �ݵ�� �Ҵ�� �޸𸮸� ������ �� ����.

    return 1;
	*/

	fstream     kWave;

	// Wave ���� ���� 
	kWave.open (FileName, ios::in | ios::binary);

	if (kWave.is_open ())
	{
		char aiID [5];

		// "RIFF" ���ڿ��� �� 
		kWave.read (aiID, sizeof (char) * 4);
		aiID [4] = '\0';


		//"RIFF"�� �Ƹ� ���� ����
		if (0 != strcmp (aiID, "RIFF"))
		{
			return ERR;
		}

		int iSize;
		//Wave ������ ũ�⸦ �о� ���δ�. 
		kWave.read ((char *) &iSize, sizeof (int));

		//"WAVE" ���ڿ��� �� 
		kWave.read (aiID, sizeof (char) * 4);
		aiID [4] = '\0';

	
		//"WAVW"�� �ƴϸ� ���� ����
		if (0 != strcmp (aiID, "WAVE"))
		{
			return ERR;
		}

		//���� ûũ �ĺ��ڸ� �����Ѵ�. 
		kWave.seekg (4, ios::cur);

		//���� ������ �о� ���δ�.  
		int iFormatLength;
		kWave.read ((char *) &iFormatLength, sizeof (int));

		//WAVEFORMATEX ����ü��ŭ �о� ���δ�. �̴� ���� ����ü���� 2����Ʈ�� ���о���̰� �ȴ�. 
		WAVEFORMATEX kWaveFormat;

		kWave.read ((char *) &kWaveFormat, sizeof (WAVEFORMATEX));
		//����ü���� �о���� 2����Ʈ �̿ܿ� �ٽ� 2����Ʈ�� ���� �ؾ� �Ѵ�.
		kWave.seekg (2, ios::cur);
		kWaveFormat.cbSize = 0;
	

		//���̺� ���� ũ�⸦ �о���δ�.
		int iDataSize;
		kWave.read ((char *) &iDataSize, sizeof (int));
		


		//���� ���۸� �дµ� �ʿ��� �޸𸮸� �Ҵ��� �� �д´�.
		UCHAR * pkSoundBuffer = new UCHAR [iDataSize];
		kWave.read ((char *) pkSoundBuffer, iDataSize);
		
		State = SND_STOPPED; // ���� ���� ���� - "����".
		
		//DirectSound Buffer�� �����ϰ� ���̺굥���͸� �����Ѵ�.
		DSBUFFERDESC         kBufferDesc;

		ZeroMemory (&kBufferDesc, sizeof (DSBUFFERDESC));
		kBufferDesc.dwSize        = sizeof (DSBUFFERDESC);   // ����ü�� ũ�⸦ ��Ÿ����.
		kBufferDesc.dwBufferBytes = iDataSize;  // ���� ���̺� �������� ũ�⸦ ��Ÿ����.
		kBufferDesc.lpwfxFormat   = &kWaveFormat; // ���Ͽ��� �о� �鿴�� ����ü�� ���ȴ�.
		kBufferDesc.dwFlags       = DSBCAPS_CTRLVOLUME;  // ���۰� ���� �÷��׸� �����Ѵ�.


		// ���۸� �����Ѵ�. 
		if (FAILED (lpds->CreateSoundBuffer (&kBufferDesc, &lpdsBuffer, NULL)) )
		{
			return ERR;
		}

		// ���۸� ��ٴ�.
		LPVOID lpvAudio;
		DWORD  dwBytes;

		if (FAILED (lpdsBuffer->Lock(0, 0,&lpvAudio, &dwBytes, NULL, NULL, DSBLOCK_ENTIREBUFFER)) )
		{
			return ERR;
		}

		// ���۸� �����Ѵ�. 
		memcpy (lpvAudio, pkSoundBuffer, dwBytes);
		lpdsBuffer->Unlock(lpvAudio, dwBytes, NULL, 0);


		// ���� �޸𸮸� �����Ѵ�. 
		delete [] pkSoundBuffer;
		kWave.close ();
		
		
	}



return 1;

} // end LoadWAV.
