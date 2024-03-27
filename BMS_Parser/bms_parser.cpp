//#include "stdafx.h"
#include "bms_parser.h"

CBmsParser::CBmsParser()
{
	m_DataList = new DATALIST;
}

CBmsParser::~CBmsParser()
{
	(*m_DataList)["BGM"].clear();
	(*m_DataList)["KEY1"].clear();
	(*m_DataList)["KEY2"].clear();
	(*m_DataList)["KEY3"].clear();
	(*m_DataList)["KEY4"].clear();
	(*m_DataList)["KEY5"].clear();
	(*m_DataList)["KEY6"].clear();

	(*m_DataList)["KEY7"].clear();
	(*m_DataList)["KEY8"].clear();


	(*m_DataList)["CG"].clear();
	(*m_DataList)["BPM"].clear();
	(*m_DataList)["CG_MASK"].clear();
	(*m_DataList).clear();

	delete m_DataList;
	m_DataList = NULL;
	m_tempString.clear();
	m_bmsinfo.clear();
}
void CBmsParser::OpenFile(char* fileName)
{
	//파일을 얻어온다.. 문자 열로서 저장하도록 하자
	ifstream fin(fileName);

	//buffer 1mb
	int buffersize = sizeof(char) * 1024 * 1024;
	char* temp = (char*)malloc(buffersize);
	memset(temp, 0,buffersize);
	//fin.read(temp,sizeof(1024));
	char ch;
	int cpCount =-1;

	while ( (ch = fin.get()) != EOF ) 
	{
		temp[++cpCount] = ch;
	}

	m_tempString = temp;

	delete temp;
	temp = NULL;
}

void CBmsParser::OpenAllDataFile(char* fileName)
{

	OpenFile(fileName);
	
	//정보를 저장한다
	setBmsInfo();

	//음악데이터를 저장한다;
	setBmsData();

}
void CBmsParser::OpenOnlyInfo(char* fileName)
{
	OpenFile(fileName);
	
	//정보를 저장한다
		OpenFile(fileName);
	
	//정보를 저장한다
	m_bmsinfo.clear();
	int startIndex = m_tempString.find('*',0);
	int StringIndex = ++startIndex;	
	while(true)
	{
		//line 바뀌는 시점까지.
		StringIndex = m_tempString.find('\n',StringIndex);
		StringIndex++;

		//음악데이터 이전까지 
		int chk =	m_tempString.find('#WAV',startIndex) ;
		
		if( chk == -1)
		{
			chk =	m_tempString.find('*',startIndex) ;
		}
		
		if( chk <= StringIndex)
		{
			break;
		}
		
		//데이터
		
		//map에 집어넣자..
		string ext = m_tempString.substr(StringIndex, ( m_tempString.find('\n',StringIndex) - StringIndex ) )  ;
		
		//값이 있는 데이터만 출력
		if(ext.size() != 0)
		{
			//cout << ext << endl;
			int keyPos = ext.find(' ',0);
			int valuePos = ext.size();

			string key = ext.substr(0 ,keyPos);
			string value = ext.substr(keyPos+1 , (keyPos+1-valuePos));
			m_bmsinfo [key] = value;	
		}

	}
}


void CBmsParser::setBmsInfo()
{
	//혹시모를 잔여 데이터 소거
	m_bmsinfo.clear();

	int startIndex = m_tempString.find('*',0);
	int StringIndex = ++startIndex;	
	while(true)
	{
		//line 바뀌는 시점까지.
		StringIndex = m_tempString.find('\n',StringIndex);
		StringIndex++;

		//음악데이터 이전까지 
		int chk =	m_tempString.find('*',startIndex) ;
		if( chk <= StringIndex)
		{
			break;
		}
		
		//데이터
		
		//map에 집어넣자..
		string ext = m_tempString.substr(StringIndex, ( m_tempString.find('\n',StringIndex) - StringIndex ) )  ;
		
		//값이 있는 데이터만 출력
		if(ext.size() != 0)
		{
			//cout << ext << endl;
			int keyPos = ext.find(' ',0);
			int valuePos = ext.size();

			string key = ext.substr(0 ,keyPos);
			string value = ext.substr(keyPos+1 , (keyPos+1-valuePos));
			m_bmsinfo[key] = value;	
		}

	}

}
void CBmsParser::setBmsData()
{
	(*m_DataList).clear();
	int startIndex = m_tempString.find('*',0);
	int StringIndex = ++startIndex;	

	int listIndex = 0;
	
	OUTDATA bgm;
	OUTDATA key[8];
	OUTDATA cg;
	OUTDATA bpm;
	OUTDATA cg_mask;


	while(true)
	{
		//line 바뀌는 시점까지.
		StringIndex = m_tempString.find('*',StringIndex);
		StringIndex++;

		//음악데이터 이전까지 
		int chk =	m_tempString.find('*',startIndex) ;
		
		if( chk <= StringIndex)
		{
			while(true)
			{

				//이곳부터 음악데이터
				string ext = m_tempString.substr(StringIndex+1, ( m_tempString.find('\n',StringIndex) - StringIndex ) )  ;

				//값이 있는 데이터만 출력
				if(ext.size() != 0)
				{
					//현재 악장
					int indexTemp =  atoi( ext.substr(0,3).c_str() );

					//현재 데이터 타입
					int Data =atoi( ext.substr(3,2).c_str() ) ;

					if(Data != 0 )
					{
						int keyPos = ext.find(':',0);
						int valuePos = ext.size();
						string data = ext.substr(keyPos+1 , (keyPos+1-valuePos));
							
						//사이즈가 384미만이라면.. 최대"192음계"
						int dataSize = (data.size() / 2);
						int size  =192;
						if(dataSize < 192)
						{
							 size = (data.size() -1) /2;
						}

						int dotCount =  192 / size -1 ; //확장해야할 길이
						
						//데이터 길이를 동일하게 맞춘다
						string temp;
						if(dotCount != 0)
						{
							for(int i = 0 ; i < size ; i++)
							{
								temp.append( data.substr(i*2 , 2) );
								for(int j = 0 ; j < dotCount ;j++)
								{
									temp.append("00");
								}
							}// end span
						}
						else
						{
							//최대치 데이터라면..
							temp = data;
						}// end if
							
						//데이터 분류
						for(int i = 0 ; i < 192 ; i++)
						{
						
							string temp3 =  temp.substr(i*2, 2);
							if(temp3.compare("00") == 1)
							{
							
								if(Data ==1)
								{
									OUTPUT te = bgm[indexTemp];
									te[i].append(temp3);
									bgm[indexTemp] = te;
								}
								//key5
								else if(Data >= 11 && Data <= 16)
								{
									OUTPUT te = key[ Data- 11][indexTemp];
									te[i].append(temp3);
									key[ Data- 11][indexTemp] = te;	
								}
								//key7 ex버젼
								else if(Data == 18 || Data == 19)
								{
									//7~8
									OUTPUT te = key[ Data- 12][indexTemp];
									te[i].append(temp3);
									key[ Data- 12][indexTemp] = te;	
								}

							
								else if(Data == 4)
								{
									OUTPUT te = cg[indexTemp];
									te[i].append(temp3);
									cg[indexTemp] = te;
								}
								else if(Data == 3)
								{
									OUTPUT te = bpm[indexTemp];
									te[i].append(temp3);
									bpm[indexTemp] = te;
								}
								else if(Data == 7)
								{
									OUTPUT te = cg_mask[indexTemp];
									te[i].append(temp3);
									cg_mask[indexTemp] = te;
								}

							}// end if

						}// end for - 데이터분류

					}// end if - 데이터 없음

				}//end while

	
				//line 바뀌는 시점까지.
				StringIndex = m_tempString.find('\n',StringIndex);
				StringIndex++;
						
				chk = m_tempString.size();
				
				//문자열 최대치를 넘는다면..
				if(chk <= StringIndex)
				{
					//모든 데이터를 저장한다
					(*m_DataList)["BGM"] = bgm;
					(*m_DataList)["KEY1"] = key[0];
					(*m_DataList)["KEY2"] = key[1];
					(*m_DataList)["KEY3"] = key[2];
					(*m_DataList)["KEY4"] = key[3];
					(*m_DataList)["KEY5"] = key[4];
					(*m_DataList)["KEY6"] = key[5];
					
					//ex버젼
					(*m_DataList)["KEY7"] = key[6];
					(*m_DataList)["KEY8"] = key[7];

					(*m_DataList)["CG"] = cg;
					(*m_DataList)["BPM"] = bpm;

					(*m_DataList)["CG_MASK"] = cg_mask;

					break;
				}//end if - 데이터 최대치

			}// end if 

			break;
		}// end while

	}

}


BMSINFO CBmsParser::GetBmsInfo()
{
	return m_bmsinfo;
}

DATALIST* CBmsParser::GetBmsDataList()
{
	return m_DataList;
}