#include <windows.h>

#include "HiEasyX.h"

int main()
{
	initgraph();

	hiex::HiSetEasyXStyle(); // Ô­Éú»æÍ¼¿â
	circle(120, 240, 100);

	hiex::HiSetGdiplusStyle(); // GDI+ »æÍ¼¿â£¨¿¹¾â³Ý£©
	circle(360, 240, 100);

	getmessage(EM_KEY);

	closegraph();

	return 0;
}