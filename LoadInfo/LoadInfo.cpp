#include "LoadInfo.h"

CLoadInfo::CLoadInfo()
{

}

CLoadInfo::~CLoadInfo()
{
}

//����κ��� Ư�� ���丮�� �����Ұ��ΰ�..
void CLoadInfo::LoadDirectory()
{
	PATHFILE_V m_PathAndFile;
	
	//Ư�� ���丮�� ���� ���� ���丮�� �����Ѵ�
	HANDLE hSrch;
	HANDLE hSrch2;
	BOOL bResult = TRUE;
	BOOL bResult2 = TRUE;
	WIN32_FIND_DATAA wfd;
	WIN32_FIND_DATAA wfd2;

	
	//���� �������
	//GetCurrentDirectoryA(255, directoryBuffer);
	//string dirPath = directoryBuffer;
	
	//Ư�� ���� ���丮 ����
	char* nowFindPath = ".\\Music\\";
	char* findOption = "*.*";

	string findPath;
	findPath.append(nowFindPath);
	findPath.append(findOption);

	//���� ���丮�� ���� ���丮 ���
	hSrch = FindFirstFileA(findPath.c_str(),&wfd);
	if(hSrch == INVALID_HANDLE_VALUE) return;
	while(bResult)
	{
		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//���丮�� .���� �����Ҽ� ���� ex)���� Ȥ�� ������ �浵 ����
			if(wfd.cFileName[0] != '.' )
			{
				//cout << nowFindPath << wfd.cFileName  << endl;
				
				//bms������
				string temp;
				temp.append(nowFindPath);
				temp.append(wfd.cFileName);
				temp.append("\\*.bms");
				hSrch2 = FindFirstFileA(temp.c_str(),&wfd2);
				bResult2 = TRUE;
				
				while(bResult2)
				{
					//cout <<"  "<< wfd2.cFileName << endl;
					
					if((unsigned int)hSrch2 != (unsigned int)0xffffffff)
					{

						PATHFILE pathfileTemp;
						
						pathfileTemp.path.append(nowFindPath);
						pathfileTemp.path.append(wfd.cFileName);
						pathfileTemp.path.append("\\");

						pathfileTemp.fileName = wfd2.cFileName;

						m_PathFiles.push_back(pathfileTemp);
						bResult2 = FindNextFileA(hSrch2 , &wfd2);
					}
					else
					{
						//������ �����ϴ�
						bResult2 = FALSE;
					}
					
				}

				//bme����
				/*
				temp.clear();
				temp.append(nowFindPath);
				temp.append(itor->c_str());
				temp.append("\\*.bme");
				hSrch = FindFirstFileA(temp.c_str(),&wfd2);
				bResult2 = TRUE;
				while(bResult2)
				{
					cout <<"  "<<  wfd2.cFileName << endl;
					bResult2 = FindNextFileA(hSrch , &wfd2);
				}
				*/
			}
		}
		bResult = FindNextFileA(hSrch , &wfd);
	}
	FindClose(hSrch2);
	FindClose(hSrch);

}

PATHFILE_V CLoadInfo::GetPathFileList()
{
	return m_PathFiles;
}