#include <windows.h>
#include <thread>

#include "HiEasyX.h"

int main()
{
	initgraph();

	hiex::HiSetEasyXStyle(); // ԭ����ͼ��
	circle(120, 240, 100);

	hiex::HiSetGdiplusStyle(); // GDI+ ��ͼ�⣨�Զ�����ݣ�
	circle(360, 240, 100);

	getmessage(EM_KEY);

	closegraph();

	return 0;
}