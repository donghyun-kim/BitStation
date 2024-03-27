
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

	// ������ ����	   
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


	//Ŭ���̾�Ʈ �۾� ������ �˾Ƴ���.
    
	RECT  rcWindowClient;

	int XOffset = 0;
	int YOffset = 0;

	GetClientRect(m_hWnd,&rcWindowClient);
 
	//Ŭ���̾�Ʈ ũ�⿡ ���� ������ �������� ����Ѵ�.
	XOffset = (800 - (rcWindowClient.right - rcWindowClient.left));
	YOffset = (600 - (rcWindowClient.bottom - rcWindowClient.top));

	//���� ���ϴ� �ػ󵵰� �ǵ��� â�� ũ�⸦ �����Ѵ�.
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

//���ν���
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
			//����
			return FALSE;
		}
		else
		{
			//�޼��� ó��
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
		//�̺�Ʈ �߻�ó��
		if(!Updata())
		break;

		//�޸𸮿��� ����� ���� �ִٸ�..
		if(m_pRender == NULL || m_pGameManager == NULL)
		break;
		
		//�ð��� �����´�
		

		//ȭ�� �׸��� ����
		if(!m_pRender->BeginScene())
		{
			//���� �����
			m_pRender->Clear();
			
			DWORD dTick = m_pRender->getTime();

			//���� ��Ʈ�� ���ɴϴ�
			switch(m_pGameManager->GetNowPart())
			{
			case 0:
				//�޴�
				m_pGameManager->ShowMainMenu(dTick);
				break;

			case 1:
				//����
				//Ű���� �̺�Ʈ ó��
				m_pGameManager->PushKey();

				//��Ʈ ���
				m_pGameManager->ShowNote(dTick);
					
				//�������̽�
				m_pGameManager->ShowInterface(dTick);
						
				//���ϸ��̼� ���
				m_pGameManager->ShowCG(dTick);

				//��׶���
				m_pGameManager->PlayBackGround(dTick);
				break;

			case 2:
				//���
				break;
			}
						
			//fpsǥ��
			m_pRender->ShowFPS(dTick);
		}
		m_pRender->EndScene();

		//���۸� ��ȯ
		m_pRender->Flip();
	}
}