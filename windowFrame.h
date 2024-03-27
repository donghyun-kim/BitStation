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
	char m_WindowClassName[100];			// �⺻ â Ŭ���� �̸��Դϴ�.
	char m_Title[100];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
	
	//////////////////////////////////
	C3DRender* m_pRender;

	/////////////////////////////////
	CGameManager* m_pGameManager;

	
protected:
	///////////
	int m_KeyBuffer[255];

	//���ν���
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};