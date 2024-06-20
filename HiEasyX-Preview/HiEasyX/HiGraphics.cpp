#include "HiGraphics.h"
#include "EasyX/EasyX.h"

#include "HiMacro.h"
#include "HiWindow.h"

namespace HiEasyXPrivate
{
	std::unordered_map<std::thread::id, HiDrawSetStuct> HiDrawSet;
}

namespace HiEasyX
{
	// ��ͼ��ʽ���ú���
	void HiSetEasyXStyle(int SmoothingMode)
	{
		std::thread::id ThreadId = std::this_thread::get_id();

		HiEasyXPrivate::HiDrawSet[ThreadId].DrawLibrary = HiEasyXPrivate::HiDrawLibraryEnum::EasyX;
		if (SmoothingMode != -1) HiEasyXPrivate::HiDrawSet[ThreadId].SmoothingMode = max(0, min(SmoothingMode, 2));
	}
	void HiSetGdiplusStyle(int SmoothingMode)
	{
		std::thread::id ThreadId = std::this_thread::get_id();

		HiEasyXPrivate::HiDrawSet[ThreadId].DrawLibrary = HiEasyXPrivate::HiDrawLibraryEnum::Gdiplus;
		if (SmoothingMode != -1) HiEasyXPrivate::HiDrawSet[ThreadId].SmoothingMode = max(0, min(SmoothingMode, 2));
	}

	// ��ͼ��ع���
	void HiCircle(int x, int y, int radius)
	{
		std::thread::id ThreadId = std::this_thread::get_id();

		// ������Ҫ�޸�ָ�� id
		BEGIN_TASK();

		// EasyX
		if (HiEasyXPrivate::HiDrawSet[ThreadId].DrawLibrary == HiEasyXPrivate::HiDrawLibraryEnum::EasyX)
		{
			circle(x, y, radius);
		}
		else if (HiEasyXPrivate::HiDrawSet[ThreadId].DrawLibrary == HiEasyXPrivate::HiDrawLibraryEnum::Gdiplus)
		{
			Gdiplus_Try_Starup();
			Gdiplus::Graphics graphics(GetImageHDC(HiEasyX::GetWindowImage()));

			graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
			graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

			Gdiplus::Pen g_pen(ConvertToGdiplusColor(RGBA(0, 0, 0, 255), false), 1); ///< ȫ�ֻ�ˢ
			graphics.DrawEllipse(&g_pen, x - radius, y - radius, radius * 2, radius * 2);
		}

		END_TASK();
		REDRAW_WINDOW();
	}
}