#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <list>

using namespace std;

typedef map< string , string > BMSINFO;


typedef map<int , string> OUTPUT;
typedef map< int , OUTPUT > OUTDATA;
typedef map<string , OUTDATA> DATALIST;


class CBmsParser
{
public:
	CBmsParser();
	~CBmsParser();

	void OpenAllDataFile(char* fileName);
	void OpenOnlyInfo(char* fileName);
	
	BMSINFO GetBmsInfo();
	DATALIST* GetBmsDataList();
	

private:
	void OpenFile(char* fileName);
	BMSINFO m_bmsinfo;
	//BMSDATA m_bmsdata;

	DATALIST* m_DataList;

	string m_tempString;

	void setBmsInfo();
	void setBmsData();

};