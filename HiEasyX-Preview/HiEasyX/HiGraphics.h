/**
 * @file	HiGraphics.h
 * @brief	HiEasyX 库的绘图模块
 * @author	Alan-CRL
*/
#pragma once
#include "HiGdiplus.h"

struct HiDrawingLibrary
{
	int DrawingLibrary = 0;
};
extern HiDrawingLibrary HiDrawingLibraryStyle;

namespace HiEasyX
{
	////////////****** 绘制相关函数 ******////////////
	void HiCircle(int x, int y, int radius);
}