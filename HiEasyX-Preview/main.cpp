#include <windows.h>

#include "HiEasyX.h"

int main()
{
	initgraph();

	HiDrawingLibraryStyle.DrawingLibrary = 0; // ԭ����ͼ��
	circle(120, 240, 100);

	HiDrawingLibraryStyle.DrawingLibrary = 1; // GDI+ ��ͼ�⣨����ݣ�
	circle(360, 240, 100);

	getmessage(EM_KEY);

	closegraph();

	return 0;
}