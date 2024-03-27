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

	//����κ��� Ư�� ���丮�� �����Ұ��ΰ�..
	void LoadDirectory();

	PATHFILE_V GetPathFileList();

private:
	PATHFILE_V m_PathFiles;

};