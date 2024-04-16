/**
 * @file	HiGraphics.h
 * @brief	HiEasyX ��Ļ�ͼģ��
 * @author	Alan-CRL
*/
#pragma once
#include "HiGdiplus.h"

#include <thread>
#include <unordered_map>

namespace HiEasyXPrivate
{
	enum HiDrawLibraryEnum
	{
		EasyX,
		Gdiplus,
		Direct2D
	};

	struct HiDrawSetStuct
	{
		// ��ͼ���趨
		int DrawLibrary = 0;
		int SmoothingMode = 0;

		IMAGE* Target = NULL;
	};
	extern std::unordered_map<std::thread::id, HiDrawSetStuct> HiDrawSet;
}

namespace HiEasyX
{
	// ��ͼ��ʽ���ú���
	void HiSetEasyXStyle(int SmoothingMode = -1);
	void HiSetGdiplusStyle(int SmoothingMode = -1);

	// ��ͼ��ع���
	void HiCircle(int x, int y, int radius);
}