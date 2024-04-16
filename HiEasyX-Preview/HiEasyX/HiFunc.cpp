#include "HiFunc.h"
#include "EasyX/EasyX.h"

#define RGBA(r, g, b, a)				(COLORREF)( ((b)<<16) | ((g)<<8) | (r) | ((a)<<24) )

ScreenSize GetScreenSize()
{
	int left = GetSystemMetrics(SM_XVIRTUALSCREEN);
	int top = GetSystemMetrics(SM_YVIRTUALSCREEN);
	int w = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int h = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	return { left,top,w,h };
}

// ��ȡͼ��ߴ�
// ���Է���ش��� IMAGE ָ��Ϊ�գ���ָ������ͼ���ڵ����
void GetImageSize(IMAGE* pImg, int* width, int* height)
{
	// ��ͨ IMAGE ָ��
	if (pImg)
	{
		*width = pImg->getwidth();
		*height = pImg->getheight();
	}

	// nullptr ��Ӧ��ͼ����
	else
	{
		IMAGE* pOld = GetWorkingImage();
		SetWorkingImage();
		*width = getwidth();
		*height = getheight();
		SetWorkingImage(pOld);
	}
}

DWORD* ReverseAlpha(DWORD* pBuf, int size)
{
	for (int i = 0; i < size; i++)
		if (pBuf[i] & 0xff000000)
			pBuf[i] &= 0x00ffffff;
		else
			pBuf[i] |= 0xff000000;
	return pBuf;
}

IMAGE CreateImageColor(int w, int h, COLORREF color, bool enable_alpha)
{
	IMAGE img(w, h);
	DWORD* pMem = GetImageBuffer(&img);
	DWORD bgColor = (((enable_alpha ? color >> 24 : 255) << 24) | (GetRValue(color) << 16) | (GetGValue(color) << 8) | GetBValue(color));
	for (int i = 0; i < w * h; i++) pMem[i] = bgColor;
	return img;
}

void SetImageColor(IMAGE& img, COLORREF color, bool enable_alpha)
{
	int w = img.getwidth();
	int h = img.getheight();
	DWORD* pMem = GetImageBuffer(&img);
	DWORD bgColor = (((enable_alpha ? color >> 24 : 255) << 24) | (GetRValue(color) << 16) | (GetGValue(color) << 8) | GetBValue(color));
	for (int i = 0; i < w * h; i++) pMem[i] = bgColor;
}

HBITMAP Image2Bitmap(IMAGE* img, bool enable_alpha)
{
	// ���Խ���
	// ��ͼ�������κ����� alpha ��Ϊ 0�������� alpha
	// ��ͼ�� alpha ȫ��Ϊ 0�����ʾ��ȫ��͸��

	DWORD* pBuf = GetImageBuffer(img);
	if (!enable_alpha)
		for (int i = 0; i < img->getwidth() * img->getheight(); i++)
			pBuf[i] &= 0x00ffffff;
	return CreateBitmap(img->getwidth(), img->getheight(), 1, 32, pBuf);
}

IMAGE Bitmap2Image(HBITMAP* hBitmap, bool enable_alpha)
{
	BITMAP bm;
	GetObject(*hBitmap, sizeof(bm), &bm);
	int width = bm.bmWidth;
	int height = bm.bmHeight;

	IMAGE img(width, height);
	DWORD* pBuf = GetImageBuffer(&img);

	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = -height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;

	HDC hMemDC = CreateCompatibleDC(NULL);
	GetDIBits(hMemDC, *hBitmap, 0, height, pBuf, &bmi, DIB_RGB_COLORS);
	DeleteDC(hMemDC);

	if (!enable_alpha)
		for (int i = 0; i < width * height; i++)
			pBuf[i] |= 0xFF000000;

	return img;
}

HICON Bitmap2Icon(HBITMAP hBmp)
{
	BITMAP bmp = {};
	GetObject(hBmp, sizeof(BITMAP), &bmp);

	HBITMAP hbmMask = CreateCompatibleBitmap(GetDC(nullptr), bmp.bmWidth, bmp.bmHeight);

	ICONINFO ii = { 0 };
	ii.fIcon = TRUE;
	ii.hbmColor = hBmp;
	ii.hbmMask = hbmMask;

	HICON hIcon = CreateIconIndirect(&ii);
	DeleteObject(hbmMask);

	return hIcon;
}

void HpSleep(int ms)
{
	static clock_t oldclock = clock();		// ��̬��������¼��һ�� tick
	static int c = CLOCKS_PER_SEC / 1000;

	oldclock += ms * c;						// ���� tick

	if (clock() > oldclock)					// ����Ѿ���ʱ��������ʱ
		oldclock = clock();
	else
		while (clock() < oldclock)			// ��ʱ
			Sleep(1);						// �ͷ� CPU ����Ȩ������ CPU ռ����
	//			Sleep(0);						// ���߾��ȡ����� CPU ռ����
}

bool IsInRect(int x, int y, RECT rct)
{
	if (rct.left > rct.right)    std::swap(rct.left, rct.right);
	if (rct.top > rct.bottom)    std::swap(rct.top, rct.bottom);
	return x >= rct.left && x <= rct.right && y >= rct.top && y <= rct.bottom;
}

UINT GetExMessageType(ExMessage msg)
{
	switch (msg.message)
	{
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
		return EM_MOUSE;
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		return EM_KEY;
		break;
	case WM_CHAR:
		return EM_CHAR;
		break;
	case WM_ACTIVATE:
	case WM_MOVE:
	case WM_SIZE:
		return EM_WINDOW;
		break;
	default:
		return 0;
		break;
	}
}

void SetWindowTransparent(HWND HWnd, bool enable, int alpha)
{
	//���Խ��ۣ��˺��������� UpdateLayeredWindow һͬʹ��

	LONG nRet = ::GetWindowLong(HWnd, GWL_EXSTYLE);
	nRet |= WS_EX_LAYERED;
	::SetWindowLong(HWnd, GWL_EXSTYLE, nRet);

	if (!enable) alpha = 0xFF;
	SetLayeredWindowAttributes(HWnd, 0, alpha, LWA_ALPHA);
}

namespace HiEasyX
{
	void TransparentImage(::IMAGE* Dstimg, int DstimgX, int DstimgY, ::IMAGE* Srcimg, int transparency)
	{
		HDC dstDC = GetImageHDC(Dstimg);
		HDC srcDC = GetImageHDC(Srcimg);
		int w = Srcimg->getwidth();
		int h = Srcimg->getheight();

		// �ṹ��ĵ�������Ա��ʾ�����͸���ȣ�0 ��ʾȫ͸����255 ��ʾ��͸����
		BLENDFUNCTION bf = { AC_SRC_OVER, 0, (BYTE)transparency, AC_SRC_ALPHA };
		// ʹ�� Windows GDI ����ʵ�ְ�͸��λͼ
		AlphaBlend(dstDC, DstimgX, DstimgY, w, h, srcDC, 0, 0, w, h, bf);
	}
	void TransparentImage(::IMAGE* Dstimg, int DstimgX, int DstimgY, int DstimgWidth, int DstimgHeight, ::IMAGE* Srcimg, int SrcimgX, int SrcimgY, int SourceWidth, int SourceHeight, int transparency)
	{
		HDC dstDC = GetImageHDC(Dstimg);
		HDC srcDC = GetImageHDC(Srcimg);

		// �ṹ��ĵ�������Ա��ʾ�����͸���ȣ�0 ��ʾȫ͸����255 ��ʾ��͸����
		BLENDFUNCTION bf = { AC_SRC_OVER, 0, (BYTE)transparency, AC_SRC_ALPHA };
		// ʹ�� Windows GDI ����ʵ�ְ�͸��λͼ
		AlphaBlend(dstDC, DstimgX, DstimgY, DstimgWidth, DstimgHeight, srcDC, SrcimgX, SrcimgY, SourceWidth, SourceHeight, bf);
	}

	bool ZoomImage_Gdiplus_Alpha(::IMAGE* img, int w, int h)
	{
		if (w == 0 && h == 0) return false;

		int DstW = img->getwidth();
		int DstH = img->getheight();

		if (w == 0) w = int(DstW * (double(h) / double(DstH)));
		if (h == 0) h = int(DstH * (double(w) / double(DstW)));

		::IMAGE DstImage = *img;
		HDC DstDC = GetImageHDC(&DstImage);

		Resize(img, w, h);
		SetImageColor(*img, RGBA(0, 0, 0, 0), true);
		HDC SrcDC = GetImageHDC(img);

		// �ṹ��ĵ�������Ա��ʾ�����͸���ȣ�0 ��ʾȫ͸����255 ��ʾ��͸����
		BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
		// ʹ�� Windows GDI ����ʵ�ְ�͸��λͼ
		AlphaBlend(SrcDC, 0, 0, w, h, DstDC, 0, 0, DstW, DstH, bf);

		return true;
	}

	void OverlayImageTransparency(::IMAGE* img, int transparency)
	{
		int width = img->getwidth();
		int height = img->getheight();

		DWORD* pBuf = GetImageBuffer(img);
		for (int i = 0; i < width * height; i++)
		{
			int alpha = (pBuf[i] >> 24) * transparency / 255;
			pBuf[i] = (pBuf[i] & 0x00FFFFFF) | (alpha << 24);
		}
	}

	void RemoveImageTransparency(::IMAGE* img)
	{
		DWORD* pBuf = GetImageBuffer(img);
		for (int i = 0; i < img->getwidth() * img->getheight(); i++)
			pBuf[i] |= 0xFF000000;
	}
}