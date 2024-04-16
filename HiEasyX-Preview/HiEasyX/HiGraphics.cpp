#include "HiGraphics.h"
#include "EasyX/EasyX.h"

#include "HiMacro.h"
#include "HiWindow.h"

HiDrawingLibrary HiDrawingLibraryStyle;

namespace HiEasyX
{
	////////////****** 全局变量 ******////////////
	//颜色，粗细等等

	////////////****** 绘图相关函数 ******////////////
	void HiCircle(int x, int y, int radius)
	{
		BEGIN_TASK();
		if (HiDrawingLibraryStyle.DrawingLibrary == 0) circle(x, y, radius);
		else
		{
			Gdiplus_Try_Starup();
			Gdiplus::Graphics graphics(GetImageHDC(HiEasyX::GetWindowImage()));

			graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
			graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

			Gdiplus::Pen g_pen(ConvertToGdiplusColor(RGBA(0, 0, 0, 255), false), 1); ///< 全局画刷
			graphics.DrawEllipse(&g_pen, x - radius, y - radius, radius * 2, radius * 2);
		}

		END_TASK();
		REDRAW_WINDOW();
	}
}