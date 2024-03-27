#pragma once

#include <windows.h>

#include "./D3DRender/D3DRender.h"
#include "./gameManager.h"

class CWindowFrame
{

public:
	CWindowFrame();
	~CWindowFrame();
	int Creat(HINSTANCE hInstance);
	int Updata();
	void Run();

	static CWindowFrame& getMainFrame();	
	LRESULT CALLBACK MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

	MSG  m_msg;
	HWND m_hWnd;

	HINSTANCE m_hInstance;
	char m_WindowClassName[100];			// 기본 창 클래스 이름입니다.
	char m_Title[100];					// 제목 표시줄 텍스트입니다.
	
	//////////////////////////////////
	C3DRender* m_pRender;

	/////////////////////////////////
	CGameManager* m_pGameManager;

	
protected:
	///////////
	int m_KeyBuffer[255];

	//프로시져
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};