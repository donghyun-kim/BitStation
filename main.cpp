
#include "./windowframe.h"

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
	
	///�������� ����
	CWindowFrame& frame = CWindowFrame::getMainFrame();

	frame.Creat(hInstance);
	frame.Run();
	
}
