#include "LoadInfo.h"

CLoadInfo::CLoadInfo()
{

}

CLoadInfo::~CLoadInfo()
{
}

//현재로부터 특정 디렉토리를 지정할것인가..
void CLoadInfo::LoadDirectory()
{
	PATHFILE_V m_PathAndFile;
	
	//특정 디렉토리로 부터 하위 디랙토리를 조사한다
	HANDLE hSrch;
	HANDLE hSrch2;
	BOOL bResult = TRUE;
	BOOL bResult2 = TRUE;
	WIN32_FIND_DATAA wfd;
	WIN32_FIND_DATAA wfd2;

	
	//현재 경로추출
	//GetCurrentDirectoryA(255, directoryBuffer);
	//string dirPath = directoryBuffer;
	
	//특정 하위 디랙토리 추출
	char* nowFindPath = ".\\Music\\";
	char* findOption = "*.*";

	string findPath;
	findPath.append(nowFindPath);
	findPath.append(findOption);

	//하위 디렉토리로 부터 디렉토리 출력
	hSrch = FindFirstFileA(findPath.c_str(),&wfd);
	if(hSrch == INVALID_HANDLE_VALUE) return;
	while(bResult)
	{
		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//디렉토리는 .으로 시작할수 없다 ex)상위 혹은 현재라는 뜻도 잇음
			if(wfd.cFileName[0] != '.' )
			{
				//cout << nowFindPath << wfd.cFileName  << endl;
				
				//bms버젼입
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
						//파일이 없습니다
						bResult2 = FALSE;
					}
					
				}

				//bme버젼
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