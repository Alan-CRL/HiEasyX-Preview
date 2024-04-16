/**
 * @file	HiGraphics.h
 * @brief	HiEasyX 库的绘图模块
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
		// 绘图库设定
		int DrawLibrary = 0;
		int SmoothingMode = 0;

		IMAGE* Target = NULL;
	};
	extern std::unordered_map<std::thread::id, HiDrawSetStuct> HiDrawSet;
}

namespace HiEasyX
{
	// 绘图样式设置函数
	void HiSetEasyXStyle(int SmoothingMode = -1);
	void HiSetGdiplusStyle(int SmoothingMode = -1);

	// 绘图相关功能
	void HiCircle(int x, int y, int radius);
}