//#include "stdafx.h"

#include "gameManager.h"

//hex문자를 숫자화
unsigned long atohex(const char *s);

CGameManager::CGameManager()
:m_pBmsParser(NULL), m_pAudio(NULL), m_pD3DRender(NULL) ,m_pLoadInfo(NULL),m_pScore(NULL)
,m_NowStage(0)
{
	if(m_pAudio == NULL)
	{
		m_pAudio = new CAudio;
	}

	if(m_pBmsParser == NULL)
	{
		m_pBmsParser = new CBmsParser;
	}
	
	if(m_pLoadInfo == NULL)
	{
		m_pLoadInfo = new CLoadInfo;
	}

	if(m_pScore == NULL)
	{
		m_pScore = new CScore;
	}

	m_InitFlagPlayBackGround = true;
	m_InitFlagShowCg = true;
	m_InitFlagShowNote1 = true;
	m_InitFlagShowNote2 = true;
	m_InitFlagInterFace = true;

}

CGameManager::~CGameManager()
{
	if(m_pAudio != NULL)
	{
		delete	m_pAudio;
		m_pAudio=0;
	}

	if(m_pBmsParser != NULL)
	{
		delete	m_pBmsParser;
		m_pBmsParser=0;
	}



	//이미지 해재
	/*
	TEXTURE_BANK::iterator textureItor;
	for(textureItor = m_textureBank.begin(); textureItor != m_textureBank.end(); textureItor++)
	{
		delete	textureItor->second.Texture;
	}
	*/
	//m_textureBank.clear();

	//delete[] m_SystemTexture;
	//delete[] m_SystemSprite;
	//m_noteBank.clear();

	//delete m_pD3DRender;
	//m_pD3DRender = 0;
}

void CGameManager::Load(char** path)
{
	char temp[2][255];// =path;
	memccpy(temp[0] , (char*)(((char*)path+0)+0) , 0, sizeof(temp[0]));
	memccpy(temp[1] , (char*)(((char*)path+0)+255) , 0, sizeof(temp[1]));

	string str;
	str.append(temp[0]);
	str.append(temp[1]);

	char temp2[255];
	memccpy(temp2 , str.c_str() ,0 ,255);

	m_pBmsParser->OpenAllDataFile(temp2);
	LoadWave(temp[0]);
	LoadFileImage(temp[0]);
}

void CGameManager::LoadOnlyInfo(char** path)
{
	char temp[2][255];// =path;
	memccpy(temp[0] , (char*)(((char*)path+0)+0) , 0, sizeof(temp[0]));
	memccpy(temp[1] , (char*)(((char*)path+0)+255) , 0, sizeof(temp[1]));

	string str;
	str.append(temp[0]);
	str.append(temp[1]);

	char temp2[255];
	memccpy(temp2 , str.c_str() ,0 ,255);

	m_pBmsParser->OpenOnlyInfo(temp2);
}

void CGameManager::LoadBmsInfo()
{
	//디렉토리의 bms를 위치를 찾아낸다
	m_pLoadInfo->LoadDirectory();
	
	//path리스트를 가져온다
	PATHFILE_V tempVecor = m_pLoadInfo->GetPathFileList();

	for(PATHFILE_V::iterator itor = tempVecor.begin() ; itor != tempVecor.end() ; itor++)
	{
		char tempPath[255] = {0,};
		sprintf(tempPath,"%s%s",itor->path.c_str(),itor->fileName.c_str());
		m_pBmsParser->OpenOnlyInfo(tempPath);
		
		//그룹핑
		static string chkGroup = itor->path.c_str();
		static int GroupNumber = 0;
		if(chkGroup.compare(itor->path.c_str()))
		{
			GroupNumber++;
			chkGroup = itor->path.c_str();
		}
	
		MAINLIST temp;

		//sort를위해..
		temp.groupNum = GroupNumber;
		temp.level = atoi (m_pBmsParser->GetBmsInfo()["#PLAYLEVEL"].c_str());
		
		//
		temp.bmsinfo = m_pBmsParser->GetBmsInfo();
		temp.fileName = itor->fileName;
		temp.path = itor->path;

		m_bmsList.push_back(temp);
	}
	m_bmsList.sort(SortMedtod);
}


void CGameManager::LoadWave(char* path)
{
	string FilePath = path;

	BMSINFO info = m_pBmsParser->GetBmsInfo();
	BMSINFO::iterator itor = info.begin();
	
	for( ; itor != info.end(); itor++)
	{
		string key = itor->first;
		string chKey = key.substr(0,4);
		int ch = chKey.compare( "#WAV");
		
		if( ch== 0)
		{
			string path = FilePath;
			path.append(itor->second);

			char cpath[255];
			strcpy(cpath ,  path.c_str());

			m_pAudio->LoadWav(cpath , key.substr(4,2) );
		}
		else
		{

		}			
	}

}

void CGameManager::LoadFileImage(char* path)
{
	//이미지 뱅크 클리어
	for(TEXTURE_BANK::iterator itor = m_textureBank.begin() ; itor !=  m_textureBank.end() ; itor++)
	{
		if(itor->second.Texture != NULL)
		{
			itor->second.Texture->Release();
		}
	}

	m_textureBank.clear();

	BMSINFO info = m_pBmsParser->GetBmsInfo();
	BMSINFO::iterator itor = info.begin();
	
	for( ; itor != info.end(); itor++)
	{
		string key = itor->first;
		string chKey = key.substr(0,4);
		int ch = chKey.compare( "#BMP");
		
		if( ch== 0)
		{
			string FilePath = path;
			FilePath.append(itor->second);

			char path[255];
			strcpy(path ,  FilePath.c_str());
			

			m_pD3DRender->LoadTexture(path,256,256,&m_textureBank[key.substr(4,2)]);
		}
		else
		{

		}			
	}	

	
}
void CGameManager::LoadSystemImage()
{
	
	//메인메뉴	
	m_pD3DRender->LoadTexture( "./System/MENU.bmp", 1024, 1024, &m_SystemTexture[0]);

	//바닦 에니메이션
	m_pD3DRender->LoadSprite(&m_SystemTexture[0],1,902,831,1023, &m_SystemSprite[4]);
	

	//플레이
	m_pD3DRender->LoadTexture( "./System/PLAY.bmp", 1024, 1024, &m_SystemTexture[1]);

	//KEY DOWN
	m_pD3DRender->LoadSprite(&m_SystemTexture[1],0,605,588,688, &m_SystemSprite[0]);
	


	//NOTE 1
	m_pD3DRender->LoadSprite(&m_SystemTexture[1],660,695,1,87, &m_SystemSprite[1]);
	
	//NOTE 2
	m_pD3DRender->LoadSprite(&m_SystemTexture[1],697,719,1,87, &m_SystemSprite[2]);
	
	//NOTE 3
	m_pD3DRender->LoadSprite(&m_SystemTexture[1],721,739,1,87, &m_SystemSprite[3]);

}

void CGameManager::LoadSystemSound()
{
	
	//선택
	m_pAudio->LoadWav("./System/SELECT.wav" , "SYSTEM_SELECT");
}

void CGameManager::ShowMainMenu(DWORD time)
{

	DWORD now = time;
	static DWORD old = time;
	DWORD tick = now - old;
	old = now;
	
	//바닦
	m_pD3DRender->DrawSprite(&m_SystemSprite[4],true,1,
							  0,0 ,0,
							  800,600,
							  7, 2, 100,
							  0,0,0);

	//기본틀
	m_pD3DRender->DrawEx(&m_SystemTexture[0],true,0,
							  0,0 ,0,
							  800,600,
							  0, 800, 0,600,
							  0,0,0);


	//회전하는 레코드
	m_pD3DRender->DrawEx(&m_SystemTexture[0],true,0,
						  140,12 ,0,
						  26,26,
						  380, 408, 639,668,
						  0,0,time/-100.0f);

	static char outbuffer[14][255] = {0,};
	static BMSLIST::iterator bmslistItor = m_bmsList.begin();
	BMSLIST::iterator bmslistItorUp = bmslistItor;
	static BMSLIST::iterator bmslistItorRand = bmslistItor;

	BMSLIST::iterator selectBmsItor;
	
	//회절할때마다 곡을 산출한다..
	if(bmslistItorRand == m_bmsList.begin())
	{
		bmslistItorRand = m_bmsList.end();
	}else{
		bmslistItorRand--;
	}

	for(int i = 0; i < 14; i++)
	{
		if(bmslistItorUp == m_bmsList.end())
		{
			bmslistItorUp = m_bmsList.begin();
		}else{
			bmslistItorUp++;
		}

			
		if(bmslistItorUp != m_bmsList.end())
		{
			sprintf(outbuffer[i],"%d : %s" ,  bmslistItorUp->level , bmslistItorUp->bmsinfo["#TITLE"].c_str());
		}else{
			sprintf(outbuffer[i],"Random" );

		}

		if(i==6)
		{
			if(bmslistItorUp != m_bmsList.end())
			{
				selectBmsItor = bmslistItorUp;
			}else{
				//end 렌덤인경우만..
				selectBmsItor = bmslistItorRand;
			}	
		}
	}
	
	//노래 선택용 키
	static DWORD keySleep1 =0;
	static DWORD skeep1 =0;
	static bool flagUp = true;
	if(m_KeyBuffer[VK_UP] == 1)
	{
		keySleep1+= tick;
		if(keySleep1 > (1000-skeep1) )
		{	
			keySleep1 =0;
			if(skeep1 <960)
			{
				skeep1+= 320;
			}
		
			flagUp = true;
		}

		if(flagUp)
		{	
			if(bmslistItor == m_bmsList.begin())
			{
				bmslistItor = m_bmsList.end();
			}else{
				bmslistItor--;
			}
			m_pAudio->Play("SYSTEM_SELECT" , 1.0f);
			flagUp =false;
		}

		
	}else{
		skeep1=0;
		keySleep1=0;
		flagUp = true;
	}

	static DWORD keySleep2 =0;
	static DWORD skeep2 =0;
	static bool flagDown = true;
	if(m_KeyBuffer[VK_DOWN] == 1)
	{
		keySleep2+= tick;
		if(keySleep2 > (1000-skeep2) )
		{	
			keySleep2 =0;
			if(skeep2 <960)
			{
				skeep2+= 320;
			}
			flagDown = true;
		}


		if(flagDown)
		{	
			if(bmslistItor == m_bmsList.end() )
			{	
				bmslistItor = m_bmsList.begin();
			}else{
				bmslistItor++;
			}
			m_pAudio->Play("SYSTEM_SELECT" , 1.0f);
			flagDown =false;
		}
	}else{
		skeep2=0;
		keySleep2=0;
		flagDown = true;
	}

	//글씨 출력
	m_pD3DRender->DrawString(outbuffer[0],50,110, D3DCOLOR_XRGB(255,255,255));
	m_pD3DRender->DrawString(outbuffer[1],50,140, D3DCOLOR_XRGB(255,255,255));
	m_pD3DRender->DrawString(outbuffer[2],50,170, D3DCOLOR_XRGB(255,255,255));
	m_pD3DRender->DrawString(outbuffer[3],50,200, D3DCOLOR_XRGB(255,255,255));
	m_pD3DRender->DrawString(outbuffer[4],50,230, D3DCOLOR_XRGB(255,255,255));
	m_pD3DRender->DrawString(outbuffer[5],50,260, D3DCOLOR_XRGB(255,255,255));
	m_pD3DRender->DrawString(outbuffer[6],50,290, D3DCOLOR_XRGB(255,255,128));
	m_pD3DRender->DrawString(outbuffer[7],50,320, D3DCOLOR_XRGB(255,255,255));
	m_pD3DRender->DrawString(outbuffer[8],50,350, D3DCOLOR_XRGB(255,255,255));
	m_pD3DRender->DrawString(outbuffer[9],50,380, D3DCOLOR_XRGB(255,255,255));
	m_pD3DRender->DrawString(outbuffer[10],50,410, D3DCOLOR_XRGB(255,255,255));
	m_pD3DRender->DrawString(outbuffer[11],50,440, D3DCOLOR_XRGB(255,255,255));
	m_pD3DRender->DrawString(outbuffer[12],50,470, D3DCOLOR_XRGB(255,255,255));
	m_pD3DRender->DrawString(outbuffer[13],50,500, D3DCOLOR_XRGB(255,255,255));

	static bool flagReturn = true;
	if(m_KeyBuffer[VK_RETURN] == 1)
	{
		if(flagReturn)
		{	
			//음악시작 루틴
			if(selectBmsItor != m_bmsList.end())
			{
				m_pScore->InitScore();
				SetInitPlay();

				char path[2][255] = {0,};
				memccpy(path[0] ,selectBmsItor->path.c_str() ,0,255);
				memccpy(path[1] ,selectBmsItor->fileName.c_str(),0 ,255);
				Load((char**)path);

				//스테이지 변환
				m_NowStage = 1;
				flagReturn =false;
			}
		}
	}else{
		flagReturn = true;
	}


}


bool CGameManager::PlayBackGround(DWORD time)
{


	DATALIST* temp = m_pBmsParser->GetBmsDataList();

	static int i = -2; //악장
	static int j = 0;  //음표

	DWORD now = time;
	static DWORD old = time;
	static float bpm = 0;
	static int size = 0;
	//초기화
	if(m_InitFlagPlayBackGround)
	{
		bpm = atoi( m_pBmsParser->GetBmsInfo()["#BPM"].c_str());

		i= -2 + 0;
		j=  0;
		old = time;
		size = (--(*temp)["BGM"].end())->first+1;
		m_InitFlagPlayBackGround =false;
	}

	DWORD tick = now - old;

	int bpmLength =(*temp)["BPM"][i][j].size(); 
	if( bpmLength != 0)
	{
		bpm = atohex(  (*temp)["BPM"][i][j].c_str()  );	
	}

	//하나의 노트당 재생해야 하는 시간을 찾는다
	float chk = (((60 / bpm) * 4)*1000) / 192;
	
	//보간해야 할 시간
	static float chk2 =0;




	if( i  > size )
	{
		return false;
	}


	//재생 시간중의 악장이라면
	if(tick + chk2 >= chk)
	{
		old = now;
		
		//시간의 차이를 보간한다
		chk2 = (tick + chk2) - chk;
		
		//노트를 재생한다
		j++;
		if(j >=  192)
		{
			i++;
			j=0;
		}

	}
	else
	{
		return true;
	}

	
	int length =(*temp)["BGM"][i][j].size(); 
	if( length != 0)
	{
		for(int note = 0 ; note < (length/2) ; note++)
		{
			m_pAudio->Play(((*temp)["BGM"][i][j]).substr(note*2 , 2) , 0.5f );
		}
	}

	//테스트용 key
	/*
	for(int key = 1 ; key < 9 ; key++)
	{
		
		char num2[2] ={0,};

		itoa(key , num2 ,10);

		string skey = "KEY";
		skey.append(num2);
		
		int length =(*temp)[skey.c_str()][i][j].size(); 
		if( length != 0)
		{
			for(int note = 0 ; note < (length/2) ; note++)
			{
				m_pAudio->Play(((*temp)[skey.c_str()][i][j]).substr(note*2 , 2) , 1.0f);
			}
		}
		
	}
	
	*/
	
	return true;
}

void CGameManager::PushKey()
{
	static bool bAuto = false;
	static S_NOTE soundNote[9];
	static bool flagEsc = true;

	if(m_KeyBuffer[VK_ESCAPE] == 1)
	{
		if(flagEsc)
		{
			m_NowStage =0;
			bAuto =false;

			for(int i = 0 ; i < 9 ;i++)
			{
				soundNote[i].judge = false;
				soundNote[i].isLive =false;
				soundNote[i].position = 600;

			}

			flagEsc =false;
		}
	}else{

		flagEsc = true;
	}




	for(NOTE_BANK::iterator countItor = m_noteBank.begin() ; countItor !=  m_noteBank.end() ; countItor++)
	{	
		if( (*countItor)->isLive  )
		{
			//오토플레이
			if(bAuto)
			{
				//185~181 best;
				//190~186 good;
				//180~176 nomal
				//175~171 bad;
				//170~166 pool;


				//190 ~ 195
				if((*countItor)->position <=  165.0f)
				{	
					switch((*countItor)->node)
					{
					case 1:
						m_KeyBuffer['Z'] = 1;
						break;

					case 2:
						m_KeyBuffer['S'] = 1;
						break;

					case 3:
						m_KeyBuffer['X'] = 1;
						break;

					case 4:
						m_KeyBuffer['D'] = 1;
						break;

					case 5:
						m_KeyBuffer['C'] = 1;
						break;

					case 6:
						m_KeyBuffer[VK_SHIFT] = 1;
						break;

					case 7:
						m_KeyBuffer['F'] = 1;
						break;
					case 8:
						m_KeyBuffer['V'] = 1;
						break;
					}
				}
			}


			/////////////////////////////
			//현재 누를 음악을 준비한다
			if(soundNote[(*countItor)->node].position > (*countItor)->position)
			{
				soundNote[(*countItor)->node].position = (*countItor)->position;
				soundNote[(*countItor)->node].noteSound = (*countItor)->noteSound;
				soundNote[(*countItor)->node].isLive = true;
				soundNote[(*countItor)->node].node = (*countItor)->node;
				//soundNote[(*countItor)->node].judge = (*countItor)->judge;
			}
	
			//선을 넘어가면 보이지 않게한다
			if((*countItor)->position <=  175)
			{		
				(*countItor)->isView = false;	
			}

			//죽은걸로 처리한다
			if((*countItor)->position <=  160)
			{		
				float judgePos = soundNote[(*countItor)->node].position;
				if(soundNote[(*countItor)->node].isLive && soundNote[(*countItor)->node].judge)
				{
					switch(m_pScore->Judgement(judgePos))
					{
						case 1:
						break;

						case 2:
						break;

						case 3:
						break;

						case 4:
						break;

						case 5:
						break;

						case 6:
						break;
						soundNote[(*countItor)->node].judge = false;
						
					}
					
				}

				//쓰지 않는 노드로..
				(*countItor)->isLive = false;	
				soundNote[(*countItor)->node].isLive = false;
				soundNote[(*countItor)->node].position = 600;
				 soundNote[(*countItor)->node].judge = true;
			}
		}
		
	}




	//f1 키를 쓰자.
	static bool flagF1 = true;
	if(m_KeyBuffer[VK_F1] == 1)
	{
		if(flagF1)
		{
			m_pAudio->Play("SYSTEM_SELECT" , 1.0f);
			
			if(!bAuto)
			{
				bAuto = true;
			}else{
				bAuto = false;
				memset(m_KeyBuffer,-1,sizeof(int)*255);
			}
			flagF1 =false;
		}
	}else{

		flagF1 = true;
	}
	
	static bool flag1 = true;
	if(m_KeyBuffer[VK_SHIFT] == 1)
	{
		//판정코드

		//스크레치 바닦
		m_pD3DRender->DrawEx(&m_SystemSprite[0].Texture, 1+(72-49), 1+(201-84)+(151-84), 0,
							  (104-65),(500-83) ,  
							  939, 978, 1, 418,
							  0,0,0);
		if(flag1)
		{
			float judgePos = soundNote[6].position;
			if(soundNote[6].isLive && soundNote[6].judge)
			{
				switch(m_pScore->Judgement(judgePos))
				{
					case 1:
					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
					break;
				}
				soundNote[6].judge = false;
			}
			m_pAudio->Play(soundNote[6].noteSound , 1.0f);
			flag1 =false;
		}
	}else{
		flag1 = true;
	}

	static bool flag2 = true;
	if(m_KeyBuffer['Z'] == 1)
	{
		//key 1번 바닦
		m_pD3DRender->DrawEx(&m_SystemSprite[0].Texture, 1+(72-49)+(104-65), 1+(201-84)+(151-84), 0,
							  (128-105),(500-83) ,  
							  978, 1002, 1, 418,
							  0,0,0);

		if(flag2)
		{
			float judgePos = soundNote[1].position;
			if(soundNote[1].isLive&& soundNote[1].judge)
			{
				switch(m_pScore->Judgement(judgePos) )
				{
					case 1:
					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
					break;
				}
				soundNote[1].judge = false;
			}
			
			m_pAudio->Play(soundNote[1].noteSound  , 1.0f);
			flag2 =false;
		}
	}else{
		flag2 = true;
	}

	static bool flag3 = true;
	if(m_KeyBuffer['S'] == 1)
	{
		//key 2번 바닦
		m_pD3DRender->DrawEx(&m_SystemSprite[0].Texture, 1+(72-49)+(104-65)+(128-105), 1+(201-84)+(151-84), 0,
							  (128-105),(500-83) ,  
							  1002, 1024, 1, 418,
							  0,0,0);
		if(flag3)
		{
			float judgePos = soundNote[2].position;
			if(soundNote[2].isLive && soundNote[2].judge)
			{
				switch(m_pScore->Judgement(judgePos))
				{
					case 1:
					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
					break;
				}
				soundNote[2].judge = false;
			}
			m_pAudio->Play(soundNote[2].noteSound  , 1.0f);
			flag3 =false;
		}
	}else{
		flag3 = true;
	}

	static bool flag4 = true;
	if(m_KeyBuffer['X'] == 1)
	{
		//key 3번 바닦
		m_pD3DRender->DrawEx(&m_SystemSprite[0].Texture, 1+(72-49)+(104-65)+(128-105)+(149-129), 1+(201-84)+(151-84), 0,
							  (128-105),(500-83) ,  
							  978, 1002, 1, 418,
							  0,0,0);

		if(flag4)
		{
			float judgePos = soundNote[3].position;
			if(soundNote[3].isLive&& soundNote[3].judge)
			{
				switch(m_pScore->Judgement(judgePos) )
				{
					case 1:
					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
					break;
					
				}
				soundNote[3].judge = false;
			}
			m_pAudio->Play(soundNote[3].noteSound  , 1.0f);
			flag4 =false;
		}
	}else{
		flag4 = true;
	}

	static bool flag5 = true;
	if(m_KeyBuffer['D'] == 1)
	{
		//key 4번 바닦
		m_pD3DRender->DrawEx(&m_SystemSprite[0].Texture, 1+(72-49)+(104-65)+(128-105)+(149-129)+(128-105), 1+(201-84)+(151-84), 0,
							  (149-129),(500-83),  
							   1002, 1024, 1, 418,
							  0, 0 ,0);
		if(flag5)
		{
			float judgePos = soundNote[4].position;
			if(soundNote[4].isLive && soundNote[4].judge)
			{
				switch(m_pScore->Judgement(judgePos))
				{
					case 1:
					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
					break;
				}
				soundNote[4].judge = false;
			}
			m_pAudio->Play(soundNote[4].noteSound  , 1.0f);
			flag5 =false;
		}
	}else{
		flag5 = true;
	}

	static bool flag6 = true;
	if(m_KeyBuffer['C'] == 1)
	{
		//key 5번 바닦
		m_pD3DRender->DrawEx(&m_SystemSprite[0].Texture, 1+(72-49)+(104-65)+(128-105)+(149-129)+(128-105)+(149-129), 1+(201-84)+(151-84), 0,
							  (128-105),(500-83) ,  
							   978, 1002, 1, 418,
							  0,0,0);
		if(flag6)
		{
			float judgePos = soundNote[5].position;
			if(soundNote[5].isLive && soundNote[5].judge)
			{
				switch(m_pScore->Judgement(judgePos))
				{
					case 1:
					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
					break;
				}
				soundNote[5].judge = false;
			}
			m_pAudio->Play(soundNote[5].noteSound  , 1.0f);
			flag6 =false;
		}
	}else{
		flag6 = true;
	}

	static bool flag7 = true;
	if(m_KeyBuffer['F'] == 1)
	{
		//key 6번 바닦
		m_pD3DRender->DrawEx(&m_SystemSprite[0].Texture, 1+(72-49)+(104-65)+(128-105)+(149-129)+(128-105)+(149-129)+(128-105), 1+(201-84)+(151-84), 0,
							  (149-129),(500-83),  
							   1002, 1024, 1, 418,
							  0, 0 ,0);
		if(flag7)
		{

			m_pAudio->Play(soundNote[7].noteSound  , 1.0f);
			flag7 =false;
		}
	}else{
		flag7 = true;
	}

	static bool flag8 = true;
	if(m_KeyBuffer['V'] == 1)
	{
		//key 7번 바닦
		m_pD3DRender->DrawEx(&m_SystemSprite[0].Texture, 1+(72-49)+(104-65)+(128-105)+(149-129)+(128-105)+(149-129)+(128-105)+(149-129), 1+(201-84)+(151-84), 0,
							  (128-105),(500-83) ,  
							   978, 1002, 1, 418,
							  0,0,0);
		if(flag8)
		{
			m_pAudio->Play(soundNote[8].noteSound  , 1.0f);
			flag8 =false;
		}
	}else{
		flag8 = true;
	}

	//오토 플레이중 키 자동 해제
	if(bAuto)
	{
		m_KeyBuffer[VK_SHIFT] = -1;
		m_KeyBuffer['Z'] = -1;
		m_KeyBuffer['S'] = -1;
		m_KeyBuffer['X'] = -1;
		m_KeyBuffer['D'] = -1;
		m_KeyBuffer['C'] = -1;
		m_KeyBuffer['F'] = -1;
		m_KeyBuffer['V'] = -1;

		//memset(m_KeyBuffer,-1,sizeof(int)*255);
	}
}





void CGameManager::ShowInterface(DWORD time)	//게임 인터페이스 이미지
{
	m_pD3DRender->DrawEx(&m_SystemSprite[0].Texture,  1, 0, 0,  
							  (623-309),(201-84) ,  
							  309, 623, 84, 201,
							  0,0,0);

	
	m_pD3DRender->DrawEx(&m_SystemSprite[0].Texture,  1, (201-84)+1, 0,  
							  (72-49),(69-1) ,  
							  49, 72, 1, 69,
							  0,0,0);

	//스크레치
	m_pD3DRender->DrawEx(&m_SystemSprite[0].Texture,  1+(72-49), (201-84)+1,0,  
							  (134-96),(69-1) ,  
							  96, 134, 1, 69,
							  0,0,0);

	int keyWidth = 0;
	int keyXStart =0;
	int keyXEnd =0;

	int rolWidth =0;
	int rolXStart =0;
	int rolXEnd=0;

	DATALIST* temp = m_pBmsParser->GetBmsDataList();

	bool chk = true;
	

	
	static int chk2 = 0;
	if(m_InitFlagInterFace)
	{
		chk2 =	(*temp)["KEY7"].size();
		m_InitFlagInterFace = false;
	}


	if(chk2 != 0)
	{
		chk = false;
	}

	//몇키로..

	if(chk)
	{
		keyXStart =194;
		keyXEnd =308;

		rolXStart = 141;
		rolXEnd = 314-20;

		keyWidth = (keyXEnd-keyXStart);
		rolWidth = (rolXEnd - rolXStart);

	}
	else
	{
		keyXStart =151;
		keyXEnd =308;

		rolXStart = 141;
		rolXEnd = 313;

		keyWidth = (keyXEnd-keyXStart);
		rolWidth = (rolXEnd - rolXStart)+22;
	}

	
	
	//건반 5사이즈
	m_pD3DRender->DrawEx(&m_SystemSprite[0].Texture, 1+(72-49)+(134-96), (201-84)+1, 0,
							  keyWidth,(151-84) ,  
							  keyXStart, keyXEnd, 86, 151,
							  0,0,0);
	//판정 라인
	m_pD3DRender->DrawEx(&m_SystemSprite[0].Texture, 1+(72-49), 1+(201-84)+(151-84), 0,
							  rolWidth,(78-70) ,  
							  rolXStart, rolXEnd, 70, 78,
							  0,0,0);

	////////////////////
	// 건반 사이즈마다..

	
	//우측 퍼센트 게이지
	m_pD3DRender->DrawEx(&m_SystemSprite[0].Texture, 1+(72-49)+(134-96)+keyWidth, (201-84)+1, 0,
							  (442-348),(69-1) ,  
							  348, 442, 1, 69,
							  0,0,0);

	//좌측 에너지 게이지
	m_pD3DRender->DrawEx(&m_SystemSprite[0].Texture, 1 , 1+(201-84)+(69-1), 0,
							  ( 25-1),(417-0) ,  
							  1, 25, 0, 417,
							  0,0,0);

	//우측 퍼센트 게이지 상단1
	m_pD3DRender->DrawEx(&m_SystemSprite[0].Texture, 1+(72-49)+(134-96)+keyWidth, 1+(201-84)+(69-1), 0,
							  (103-65),(83-70) ,  
							  65, 103, 70, 83,
							  0,0,0);


	//우측 퍼센트 게이지 상단2
	m_pD3DRender->DrawEx(&m_SystemSprite[0].Texture, 1+(72-49)+(134-96)+keyWidth, 1+(201-84)+(69-1)+(83-70), 0,
							  (57-49),(474-70) ,  
							  49, 57, 70, 474,
							  0,0,0);


	///////////////////////////////////////////////////////
	

	//스크레치 바닦
	m_pD3DRender->DrawEx(&m_SystemSprite[0].Texture, 1+(72-49), 1+(201-84)+(151-84), 0,
							  (104-65),(500-83) ,  
							  65, 104, 83, 500,
							  0,0,0);

	//key 1번 바닦
	m_pD3DRender->DrawEx(&m_SystemSprite[0].Texture, 1+(72-49)+(104-65), 1+(201-84)+(151-84), 0,
							  (128-105),(500-83) ,  
							  105, 128, 83, 500,
							  0,0,0);

	//key 2번 바닦
	m_pD3DRender->DrawEx(&m_SystemSprite[0].Texture, 1+(72-49)+(104-65)+(128-105), 1+(201-84)+(151-84), 0,
							  (149-129),(500-83),  
							  129, 148, 83, 500,
							  0, 0 ,0);

	//key 3번 바닦
	m_pD3DRender->DrawEx(&m_SystemSprite[0].Texture, 1+(72-49)+(104-65)+(128-105)+(149-129), 1+(201-84)+(151-84), 0,
							  (128-105),(500-83) ,  
							  105, 128, 83, 500,
							  0,0,0);

	//key 4번 바닦
	m_pD3DRender->DrawEx(&m_SystemSprite[0].Texture, 1+(72-49)+(104-65)+(128-105)+(149-129)+(128-105), 1+(201-84)+(151-84), 0,
							  (149-129),(500-83),  
							  129, 149, 83, 500,
							  0, 0 ,0);
	//key 5번 바닦
	m_pD3DRender->DrawEx(&m_SystemSprite[0].Texture, 1+(72-49)+(104-65)+(128-105)+(149-129)+(128-105)+(149-129), 1+(201-84)+(151-84), 0,
							  (128-105),(500-83) ,  
							  105, 128, 83, 500,
							  0,0,0);

	if(!chk)
	{
	//key 4번 바닦
	m_pD3DRender->DrawEx(&m_SystemSprite[0].Texture, 1+(72-49)+(104-65)+(128-105)+(149-129)+(128-105)+(149-129)+(128-105), 1+(201-84)+(151-84), 0,
							  (149-129),(500-83),  
							  129, 149, 83, 500,
							  0, 0 ,0);
	//key 5번 바닦
	m_pD3DRender->DrawEx(&m_SystemSprite[0].Texture, 1+(72-49)+(104-65)+(128-105)+(149-129)+(128-105)+(149-129)+(128-105)+(149-129), 1+(201-84)+(151-84), 0,
							  (128-105),(500-83) ,  
							  105, 128, 83, 500,
							  0,0,0);
	}


	//디버그용
	char buffer[255];
	sprintf(buffer,"m_Best : %d", m_pScore->m_Best);
	m_pD3DRender->DrawString(buffer,350,540,D3DCOLOR_XRGB(255,255,255));
	
	char buffer1[255];
	sprintf(buffer1,"m_Good : %d", m_pScore->m_Good);
	m_pD3DRender->DrawString(buffer1,450,540,D3DCOLOR_XRGB(255,255,255));
	
	char buffer2[255];
	sprintf(buffer2,"m_Nomal : %d", m_pScore->m_Nomal);
	m_pD3DRender->DrawString(buffer2,550,540,D3DCOLOR_XRGB(255,255,255));
	
	char buffer3[255];
	sprintf(buffer3,"m_Bad : %d", m_pScore->m_Bad);
	m_pD3DRender->DrawString(buffer3,350,560,D3DCOLOR_XRGB(255,255,255));
	
	char buffer4[255];
	sprintf(buffer4,"m_Pool : %d", m_pScore->m_Pool);
	m_pD3DRender->DrawString(buffer4,450,560,D3DCOLOR_XRGB(255,255,255));

	char buffer5[255];
	sprintf(buffer5,"m_Miss : %d", m_pScore->m_Miss);
	m_pD3DRender->DrawString(buffer5,550,560,D3DCOLOR_XRGB(255,255,255));


	char buffer6[255];
	sprintf(buffer6,"m_Combo : %d", m_pScore->m_Combo);
	m_pD3DRender->DrawString(buffer6,350,520,D3DCOLOR_XRGB(255,255,255));
	
	char buffer7[255];
	sprintf(buffer7,"m_LastMaxCombo : %d", m_pScore->m_LastMaxCombo);
	m_pD3DRender->DrawString(buffer7,450,520,D3DCOLOR_XRGB(255,255,255));
	



}

void CGameManager::ShowNote(DWORD time)	//게임 노트 이미지 및 판정
{
		//1.5, 2.0, 2.5 ,3.0 , 3.5 ,4.0 , 4.5
	static int playX = 0;

	static int i = -1 ; //악장	
	static int j = 0  - playX; //음표 //2배
	static int n = 0 -  playX;

	//디버그용
	char buffer[255];
	sprintf(buffer,"배속값 : %d", playX);
	m_pD3DRender->DrawString(buffer,600,580,D3DCOLOR_XRGB(255,255,255));
	

	char buffer1[255];
	sprintf(buffer1,"악장 : %d", i);
	m_pD3DRender->DrawString(buffer1,400,580,D3DCOLOR_XRGB(255,255,255));
	

	char buffer2[255];
	sprintf(buffer2,"음표 : %d", j);
	m_pD3DRender->DrawString(buffer2,500,580,D3DCOLOR_XRGB(255,255,255));
	
	//f3  키를 쓰자.
	static bool flagF3 = true;
	if(m_KeyBuffer[VK_F3] == 1)
	{
		if(flagF3)
		{
			m_pAudio->Play("SYSTEM_SELECT" , 1.0f);
			playX= playX+= 32;
			//j -=playX;
			//n =playX;
			
			//
			for(NOTE_BANK::iterator countItor = m_noteBank.begin() ; countItor !=  m_noteBank.end() ; countItor++)
			{
				//(*countItor)->position  = (((((*countItor)->position) - 182)/ (float)(287+n)))/3.0f;
				if((*countItor)->isLive && playX != (*countItor)->playX)
				{
				}
			}
			
			flagF3 =false;
		}
	}else{

		flagF3 = true;
	}
	
	//f4  키를 쓰자.
	static bool flagF4 = true;
	if(m_KeyBuffer[VK_F4] == 1)
	{
		if(flagF4)
		{
			m_pAudio->Play("SYSTEM_SELECT" , 1.0f);
			playX= playX-= 32;
			j +=playX;
			//n =playX;
			
			//
			for(NOTE_BANK::iterator countItor = m_noteBank.begin() ; countItor !=  m_noteBank.end() ; countItor++)
			{
				//(*countItor)->position  = (((((*countItor)->position) - 182)/ (float)(287+n)))/3.0f;
				if((*countItor)->isLive && playX != (*countItor)->playX)
				{
					//(*countItor)->position  -= (((600 - 182)/ (float)(287+(*countItor)->playX)))/3.0f;
					//float playXTemp = playX - (*countItor)->playX;
					//(*countItor)->position  =  ( (600-(*countItor)->position) / (float)(287+n) )/3.0;
								
					//(*countItor)->position  -= (((600 - 182)/ (float)(287+n)))/3.0f;
					//(*countItor)->position  =  ( (600-(*countItor)->position) / (float)(287+(*countItor)->playX) )/3.0;

				}
			
			}
			
			flagF4 =false;
		}
	}else{

		flagF4 = true;
	}

	DATALIST* temp = m_pBmsParser->GetBmsDataList();

	DWORD now = time;
	static DWORD old = time;
	static float bpm = 0;
	//마지막 악장을 찾는다
	//OUTDATA::iterator itor = ((*temp)["BGM"].end()->first+1);
	static int size = 0;
	
	if(m_InitFlagShowNote1)
	{

		bpm = atoi( m_pBmsParser->GetBmsInfo()["#BPM"].c_str());

		i= -1+0;
		j=  0;
		old = time;
		size = (--(*temp)["BGM"].end())->first+2;
		m_InitFlagShowNote1 =false;
	}

	DWORD tick = now - old;
	



	int bpmLength =(*temp)["BPM"][i-1][j].size(); 
	if( bpmLength != 0)
	{
		bpm = atohex(  (*temp)["BPM"][i-1][j].c_str()  );	
	}

	//하나의 노트당 재생해야 하는 시간을 찾는다
	float chk = ((((60 / bpm) * 4)*1000) / 192) ;
	
	//보간해야 할 시간
	static float chk2 =0;


	

	if( i  > size )
	{
		//return;
		m_NowStage =0;
	}

	for(NOTE_BANK::iterator countItor = m_noteBank.begin() ; countItor !=  m_noteBank.end() ; countItor++)
	{
		if((*countItor)->isView)
		{

			switch ((*countItor)->node)
			{
				case 1:

					m_pD3DRender->DrawSprite ( &m_SystemSprite[2] ,   25+37,	(*countItor)->position,   0,   24, 21, 1, 4,  180, 0,  0,0);
					break;

				case 2:
					m_pD3DRender->DrawSprite ( &m_SystemSprite[3] ,   25+37+24, (*countItor)->position,   0,   20, 21, 1, 4,  180, 0,  0,0);
					break;

				case 3:
					m_pD3DRender->DrawSprite ( &m_SystemSprite[2] ,   25+37+24+20,	(*countItor)->position,   0,   24, 21, 1, 4,  180, 0,  0,0);
					break;

				case 4:
					m_pD3DRender->DrawSprite ( &m_SystemSprite[3] ,   25+37+24+20+24, (*countItor)->position,   0,   20, 21, 1, 4,  180, 0,  0,0);
					break;

				case 5:
					m_pD3DRender->DrawSprite ( &m_SystemSprite[2] ,   25+37+24+20+24+20,	(*countItor)->position,   0,   24, 21, 1, 4,  180, 0,  0,0);
					break;
					
					//스크레치
				case 6:
					m_pD3DRender->DrawSprite ( &m_SystemSprite[1] ,   25,		(*countItor)->position,  0 ,   37, 21, 1, 4,  180, 0,  0,0);
					break;

				case 7:
					m_pD3DRender->DrawSprite ( &m_SystemSprite[3] ,   25+37+24+20+24+20+24, (*countItor)->position,   0,   20, 21, 1, 4,  180, 0,  0,0);
					break;

				case 8:
					m_pD3DRender->DrawSprite ( &m_SystemSprite[2] ,   25+37+24+20+24+20+24+20,	(*countItor)->position,   0,   24, 21, 1, 4,  180, 0,  0,0);
					break;

			}
		}
	}

	//하나의 노트당 재생해야 하는 시간을 찾는다

	DWORD now2 = time;
	static DWORD old2 = time;
	float MoveTime = (chk/3.0f) ;
	static float TempMoveTime =0;

	if(m_InitFlagShowNote2)
	{
		old2 = time;
		m_InitFlagShowNote2 =false;
	}

	DWORD tick2 = now2 - old2;
	

	if(tick2 + TempMoveTime > MoveTime)
	{
		old2 = now2;
		//시간의 차이를 보간한다
		TempMoveTime = (tick2 + TempMoveTime) - MoveTime;

		for(NOTE_BANK::iterator countItor = m_noteBank.begin() ; countItor !=  m_noteBank.end() ; countItor++)
		{
			(*countItor)->position  -= (((600 - 183)/ (float)(192+(*countItor)->playX)))/3.0f;
		}
		
	}
	


	//재생 시간중의 악장이라면
	if(tick + chk2 >= chk)
	{
		old = now;
		
		//시간의 차이를 보간한다
		chk2 = (tick + chk2) - chk;
		
		//노트를 재생한다
		j++;
		if(j >=  192)
		{
			i++;
			j=0;
		}
	}
	else
	{
		return;
	}

	// 1악장 전에 투입
	for(int key = 1 ; key < 9 ; key++)
	{
		char num2[2] ={0,};

		itoa(key , num2 ,10);

		string skey = "KEY";
		skey.append(num2);
		
		int length =(*temp)[skey.c_str()][i][j].size(); 
		if( length != 0)
		{
			bool flag = true;
			for(NOTE_BANK::iterator countItor = m_noteBank.begin() ; countItor !=  m_noteBank.end() ; countItor++)
			{
				S_NOTE* pNote = *countItor;
				
				if(pNote->isLive == false)
				{
					//재사용 한다~
					pNote->playX = playX;
					pNote->judge = true;
					pNote->isLive = true;
					pNote->isView = true;
					pNote->position = 600;
					pNote->node = key;
					pNote->noteSound = ((*temp)[skey.c_str()][i][j]).substr(0 , 2) ;
					flag = false;
					break;
				}
			}	
			
			//재사용할수있는 노트가 없다면..
			if(flag)
			{
				// 벡터에 추가한다.
				S_NOTE* insertNote = new S_NOTE;
				
				insertNote->playX = playX;
				insertNote->judge = true;
				insertNote->isLive = true;
				insertNote->isView = true;
				insertNote->node = key;
				insertNote->position = 600;
				insertNote->noteSound = ((*temp)[skey.c_str()][i][j]).substr(0 , 2) ;

				m_noteBank.push_back(insertNote);
				//%미리 버퍼로 50개 정도 만들어두면 좋을듯하다
			}
		}	
	}

}

void CGameManager::ShowCG(DWORD time)	//게임 배경이미지
{
	DATALIST* temp = m_pBmsParser->GetBmsDataList();
	
	static D3DTEXTURE* pSowCG = NULL;
	static D3DTEXTURE* pSowCG_Mask = NULL;
	static char strtem[255] ={0,};
	static char strtem2[255] ={0,};
	static int i = -2; //악장
	static int j = 0; //음표
	DWORD now = time;
	static DWORD old = time;
	static float bpm = 0;
	static int size = 0;

	//초기화
	if(m_InitFlagShowCg)
	{
		bpm = atoi( m_pBmsParser->GetBmsInfo()["#BPM"].c_str());
		pSowCG = NULL;
		pSowCG_Mask = NULL;
		i= -2 +0;
		j=  0;
		old = time;
		size = (--(*temp)["BGM"].end())->first+1;
		m_InitFlagShowCg =false;
	}

		DWORD tick = now - old;



	int bpmLength =(*temp)["BPM"][i][j].size(); 
	if( bpmLength != 0)
	{
		bpm = atohex(  (*temp)["BPM"][i][j].c_str()  );	
	}

	//하나의 노트당 재생해야 하는 시간을 찾는다
	float chk = (((60 / bpm) * 4)*1000) / 192;
	
	//보간해야 할 시간
	static float chk2 =0;

	if( i  > size )
	{
		return;
	}
	
	int outX = 315;
	int outY = 50;
	int outSizeX = 480;
	int outSizeY = 480;

	//이전 출력했던 cg 출력

	
	m_pD3DRender->DrawString(strtem2  ,580 ,10, D3DCOLOR_XRGB(255,255,255));
	m_pD3DRender->DrawString(strtem  ,480 ,10, D3DCOLOR_XRGB(255,255,255));

	//이미지
	m_pD3DRender->DrawEx( pSowCG  ,  outX,   outY,  0,  outSizeX, outSizeY,    0, 256,  0, 256,  0,0,0);
	
	//마스킹
	m_pD3DRender->DrawEx( pSowCG_Mask  , true, 0, outX,   outY,  0,  outSizeX, outSizeY,    0, 256,  0, 256,  0,0,0);


	//재생 시간중의 악장이라면
	if(tick + chk2 >= chk)
	{
		old = now;
		
		//시간의 차이를 보간한다
		chk2 = (tick + chk2) - chk;
		
		//노트를 재생한다
		j++;
		if(j >=  192)
		{
			i++;
			j=0;
		}
	}
	else
	{
		return;
	}

	int length =(*temp)["CG"][i][j].size(); 

	if( length != 0)
	{
		//cg를 변경한다
		pSowCG = &m_textureBank[((*temp)["CG"][i][j]).substr(0 , 2) ];
		strcpy( strtem ,((*temp)["CG"][i][j]).substr(0 , 2).c_str());
	}

	int length2 =(*temp)["CG_MASK"][i][j].size(); 
	if( length2 != 0)
	{
		//mask를 변경한다
		pSowCG_Mask = &m_textureBank[((*temp)["CG_MASK"][i][j]).substr(0 , 2)];
		strcpy( strtem2 ,((*temp)["CG_MASK"][i][j]).substr(0 , 2).c_str());
	}
	
}

// hex문자를 숫자화
unsigned long atohex(const char *s)
{
  int rv = 0;
  while (*s)
  {
    int v = *s - '0';
    if (v > 9)
      v -= 7;
    v &= 15; /* in case it's lower case */
    rv = rv*16 + v;
    s++;
  }
  return rv;
}


void CGameManager::setRender(C3DRender* pRender)
{
	m_pD3DRender = pRender;
}

void CGameManager::setKeyBuffer(int* keyBuffer)
{
	m_KeyBuffer = keyBuffer;
}

//소팅용 메소드1
bool CGameManager::SortMedtod(const MAINLIST& elem1, const MAINLIST& elem2 )
{
	if( elem1.groupNum == elem2.groupNum )
	{
		// 이조건으로정렬
		return elem1.level < elem2.level;
	} 

     // 다르면이조건으로정렬

      return elem1.groupNum < elem2.groupNum;
}

int CGameManager::GetNowPart()
{
	return m_NowStage;
}

void CGameManager::SetInitPlay()
{

	//모든키를 초기화
	memset(m_KeyBuffer,-1,sizeof(int)*255);

	//화면에 그려졌던 노트를 전부 초기화
	for(NOTE_BANK::iterator Itor = m_noteBank.begin() ; Itor !=  m_noteBank.end() ; Itor++)
	{
		(*Itor)->isView =false;
		(*Itor)->isLive =false;
		(*Itor)->position = 600;

	}

	//기존 로드했던 음악을 제거하고 다시 할당
	
	if(m_pAudio != NULL)
	{
		delete m_pAudio;
		m_pAudio  = new CAudio;
		LoadSystemSound();
	}
	
	//초기화 플러그
	m_InitFlagPlayBackGround = true;
	m_InitFlagShowCg = true;
	m_InitFlagShowNote1 = true;
	m_InitFlagShowNote2 = true;
	m_InitFlagInterFace= true;
}