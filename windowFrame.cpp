
#include "windowFrame.h"
#include "./gameManager.h"

CWindowFrame::CWindowFrame()
{
	memset(m_KeyBuffer,-1,sizeof(m_KeyBuffer));
	m_pGameManager = new CGameManager;
}

CWindowFrame::~CWindowFrame()
{
	//delete	p_mRender;
	UnregisterClass( m_WindowClassName , m_hInstance);
	
}

CWindowFrame& CWindowFrame::getMainFrame()
{
	static CWindowFrame frame;
	return frame;
}


int CWindowFrame::Creat(HINSTANCE hInstance)
{
	//HINSTANCE hInst;	
	m_hInstance = hInstance;

	HACCEL hAccelTable;

	
	strcpy(m_Title , "BitStaion");
	strcpy(m_WindowClassName , "BitStation");


	///////////////////////////
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= 0;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_T));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszMenuName	= 0;//MAKEINTRESOURCE(IDC_T);
	wcex.lpszClassName	= m_WindowClassName;
	wcex.hIconSm		= 0;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&wcex);
	////////////////////

	// 윈도우 생성	   
	m_hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
		m_WindowClassName, m_Title, 
							WS_SYSMENU , 0, 0,
							//WS_OVERLAPPEDWINDOW , 0, 0,
							800, 600,
							NULL, NULL,
							hInstance, NULL);

	if (!m_hWnd)
	{
		return FALSE;
	}
	/////////
	
	ShowWindow(m_hWnd,SW_SHOW);
	//ShowCursor(FALSE);
	UpdateWindow(m_hWnd);


	//클라이언트 작업 영역을 알아낸다.
    
	RECT  rcWindowClient;

	int XOffset = 0;
	int YOffset = 0;

	GetClientRect(m_hWnd,&rcWindowClient);
 
	//클라이언트 크기에 따른 렌더링 오프셋을 계산한다.
	XOffset = (800 - (rcWindowClient.right - rcWindowClient.left));
	YOffset = (600 - (rcWindowClient.bottom - rcWindowClient.top));

	//실제 원하는 해상도가 되도록 창의 크기를 변경한다.
	SetWindowPos(m_hWnd,NULL,0,0,
				 800 + XOffset,
				 600 + YOffset,
				 NULL);

	m_pRender = new C3DRender;
	m_pRender->Creat(m_hWnd);
	m_pGameManager->setRender(m_pRender);
	m_pGameManager->setKeyBuffer(m_KeyBuffer);
	

	m_pGameManager->LoadBmsInfo();
	m_pGameManager->LoadSystemImage();
	m_pGameManager->LoadSystemSound();
	
}


LRESULT CALLBACK CWindowFrame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return getMainFrame().MainProc(hWnd, message, wParam,  lParam);
}

//프로시져
LRESULT CALLBACK CWindowFrame::MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		m_KeyBuffer[wParam] = 1;
		break;
	case WM_KEYUP:
		m_KeyBuffer[wParam] = -1;
		break;

	case WM_CREATE:
		{
		

		}
		break;

	case WM_COMMAND:
		break;

	case WM_PAINT:
		{
			//PAINTSTRUCT ps;
			//HDC hdc;
			//hdc = BeginPaint(hWnd, &ps);
			//EndPaint(hWnd, &ps);
		}
		break;

	case WM_DESTROY:
		
		delete m_pGameManager;
		m_pGameManager = NULL;

		delete m_pRender;
		m_pRender = NULL;

		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


int CWindowFrame::Updata()
{

	while(PeekMessage(&m_msg ,NULL , 0, 0, PM_REMOVE ) )
	{
		if(m_msg.message == WM_QUIT)
		{
			//종료
			return FALSE;
		}
		else
		{
			//메세지 처리
			TranslateMessage(&m_msg);
			DispatchMessage(&m_msg);
			
			return TRUE;
		}
	}

	return TRUE;
	
}

void CWindowFrame::Run()
{
	while(true)
	{
		//이벤트 발생처리
		if(!Updata())
		break;

		//메모리에서 해재된 것이 있다면..
		if(m_pRender == NULL || m_pGameManager == NULL)
		break;
		
		//시간을 가져온다
		

		//화면 그리기 시작
		if(!m_pRender->BeginScene())
		{
			//버퍼 지우기
			m_pRender->Clear();
			
			DWORD dTick = m_pRender->getTime();

			//현재 파트를 얻어옵니다
			switch(m_pGameManager->GetNowPart())
			{
			case 0:
				//메뉴
				m_pGameManager->ShowMainMenu(dTick);
				break;

			case 1:
				//게임
				//키보드 이벤트 처리
				m_pGameManager->PushKey();

				//노트 출력
				m_pGameManager->ShowNote(dTick);
					
				//인터페이스
				m_pGameManager->ShowInterface(dTick);
						
				//에니메이션 재생
				m_pGameManager->ShowCG(dTick);

				//백그라운드
				m_pGameManager->PlayBackGround(dTick);
				break;

			case 2:
				//결과
				break;
			}
						
			//fps표기
			m_pRender->ShowFPS(dTick);
		}
		m_pRender->EndScene();

		//버퍼를 전환
		m_pRender->Flip();
	}
}