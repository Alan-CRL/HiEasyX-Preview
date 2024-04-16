#include <windows.h>

#include "HiEasyX.h"

int main()
{
	initgraph();

	HiDrawingLibraryStyle.DrawingLibrary = 0; // Ô­Éú»æÍ¼¿â
	circle(120, 240, 100);

	HiDrawingLibraryStyle.DrawingLibrary = 1; // GDI+ »æÍ¼¿â£¨¿¹¾â³Ý£©
	circle(360, 240, 100);

	getmessage(EM_KEY);

	closegraph();

	return 0;
}