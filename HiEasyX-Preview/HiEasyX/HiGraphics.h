/**
 * @file	HiGraphics.h
 * @brief	HiEasyX ��Ļ�ͼģ��
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
	////////////****** ������غ��� ******////////////
	void HiCircle(int x, int y, int radius);
}