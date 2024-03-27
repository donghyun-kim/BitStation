
#include "./windowframe.h"

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
	
	///프레임을 생성
	CWindowFrame& frame = CWindowFrame::getMainFrame();

	frame.Creat(hInstance);
	frame.Run();
	
}
