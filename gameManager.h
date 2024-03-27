#pragma once


#include "./audio/audio.h"
#include "./BMS_Parser/bms_parser.h"
#include "./D3DRender/D3DRender.h"
#include "./LoadInfo/LoadInfo.h"
#include "./score/score.h"

#include <map>
#include <string>
#include <ctype.h>
#include <vector>
#include <list>

using namespace std;

enum
{
	KEY_1 = 11,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6

};

struct S_NOTE
{
	S_NOTE()
		:node(0),position(600),isLive(false),noteSound("01"),playX(0),judge(true)
	{
	}
	int    node;
	float  position;
	string noteSound;
	bool   isLive;
	bool   isView;
	int	   playX;
	bool  judge;
};

//typedef map<string, CAudio* > WAVEBANK;

typedef map<string, D3DTEXTURE> TEXTURE_BANK;
//typedef vector<S_NOTE*> NOTE_BANK;
typedef list<S_NOTE*> NOTE_BANK;

struct MAINLIST
{
	MAINLIST()
		:groupNum(0),level(0)
	{
	}
	BMSINFO bmsinfo;
	string path;
	string fileName;
	int groupNum;
	int level;
};

typedef list<MAINLIST> BMSLIST;


class CGameManager
{
	
public:
	//void Menu();
	//void Play();

	CGameManager();
	~CGameManager();

	void LoadSystemImage();
	void LoadSystemSound();
	void Load(char** path);
	void LoadOnlyInfo(char** path);
	void LoadBmsInfo();
	
	bool PlayBackGround(DWORD time);
	void PushKey();

	void ShowInterface(DWORD time);	//게임 인터페이스 이미지
	void ShowNote(DWORD time);	//게임 노트 이미지 및 판정
	void ShowCG(DWORD time);	//게임 배경이미지
	
	void SetInitPlay();


	void setRender(C3DRender* pRender);
	void setKeyBuffer(int* keyBuffer);


	void ShowMainMenu(DWORD time);

	int GetNowPart(); //현재 영역


private:
	int m_NowStage;	   //스테이지를 관리한다
	int* m_KeyBuffer; //255까지 범위이다

	CBmsParser* m_pBmsParser;	//파서
	CAudio* m_pAudio;			//오디오
	C3DRender* m_pD3DRender;	//렌더
	CLoadInfo* m_pLoadInfo;		//디렉토리 로더
	CScore*		m_pScore;

	void LoadWave(char* path);
	void LoadFileImage(char* path);

	//텍스쳐 관련
	TEXTURE_BANK m_textureBank;
	D3DTEXTURE m_SystemTexture[10];
	D3DSPRITE  m_SystemSprite[10];
	NOTE_BANK m_noteBank;
	BMSLIST m_bmsList;

	//플레이 초기화용 플레그
	bool m_InitFlagPlayBackGround;
	bool m_InitFlagShowCg;
	bool m_InitFlagShowNote1;
	bool m_InitFlagShowNote2;
	bool m_InitFlagInterFace;
	//소팅용 메소드
	static bool SortMedtod(const MAINLIST& elem1, const MAINLIST& elem2 );

};