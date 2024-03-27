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

	void ShowInterface(DWORD time);	//���� �������̽� �̹���
	void ShowNote(DWORD time);	//���� ��Ʈ �̹��� �� ����
	void ShowCG(DWORD time);	//���� ����̹���
	
	void SetInitPlay();


	void setRender(C3DRender* pRender);
	void setKeyBuffer(int* keyBuffer);


	void ShowMainMenu(DWORD time);

	int GetNowPart(); //���� ����


private:
	int m_NowStage;	   //���������� �����Ѵ�
	int* m_KeyBuffer; //255���� �����̴�

	CBmsParser* m_pBmsParser;	//�ļ�
	CAudio* m_pAudio;			//�����
	C3DRender* m_pD3DRender;	//����
	CLoadInfo* m_pLoadInfo;		//���丮 �δ�
	CScore*		m_pScore;

	void LoadWave(char* path);
	void LoadFileImage(char* path);

	//�ؽ��� ����
	TEXTURE_BANK m_textureBank;
	D3DTEXTURE m_SystemTexture[10];
	D3DSPRITE  m_SystemSprite[10];
	NOTE_BANK m_noteBank;
	BMSLIST m_bmsList;

	//�÷��� �ʱ�ȭ�� �÷���
	bool m_InitFlagPlayBackGround;
	bool m_InitFlagShowCg;
	bool m_InitFlagShowNote1;
	bool m_InitFlagShowNote2;
	bool m_InitFlagInterFace;
	//���ÿ� �޼ҵ�
	static bool SortMedtod(const MAINLIST& elem1, const MAINLIST& elem2 );

};