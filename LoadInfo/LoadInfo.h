#pragma once

#include "windows.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct PATHFILE
{
	PATHFILE()
		:path(""),fileName("")
	{
	}
	string path;
	string fileName;
};

typedef vector <PATHFILE> PATHFILE_V;



class CLoadInfo
{
public:
	CLoadInfo();
	~CLoadInfo();

	//현재로부터 특정 디렉토리를 지정할것인가..
	void LoadDirectory();

	PATHFILE_V GetPathFileList();

private:
	PATHFILE_V m_PathFiles;

};