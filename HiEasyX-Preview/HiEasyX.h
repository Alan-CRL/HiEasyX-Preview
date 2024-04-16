/**
 * @file		HiEasyX.h
 * @brief		HiEasyX ��ͷ�ļ�
 * @note		HiEasyX �ǻ��� EasyX ����չ�⣬֧�ִ����ര�ڡ�͸������ݻ�ͼ��ϵͳ UI ����ȵȡ�
 *
 * @version		Ver 2.0.0
 * @envir		VisualStudio 2022 | EasyX_20220901 | Windows 11
 * @site		https://github.com/zouhuidong/HiEasyX
 *
 * @author		huidong
 * @qq			1442701429
 * @email		mailhuid@163.com
 *
 * @assistant	Alan-CRL
 * @qq			2685549821
 * @email		alan-crl@foxmail.com
 *
 * @create		2022.07.20
 *
 * @attention	���ʹ��MinGW��TDM�ȱ�����������Ҫ�������ѡ��:-leasyx -lmsimg32 -lgdiplus -lgdi32 -lwinmm
 * 				��Ҫʹ�� C++14 �����ϱ�׼������ʱ��ѡ�� ISO C++14 (�����ϱ�׼)��׼����ӱ���ѡ��:-std=c++14 (�����ϱ�׼)
*/

#pragma once

#define __HIEASYX_H__

#include "HiEasyX/HiDef.h"
#include "HiEasyX/HiMacro.h"
#include "HiEasyX/HiFunc.h"
#include "HiEasyX/HiFPS.h"
#include "HiEasyX/HiDrawingProperty.h"
#include "HiEasyX/HiMouseDrag.h"
#include "HiEasyX/HiMusicMCI.h"
#include "HiEasyX/HiGif.h"

#include "HiEasyX/HiGraphics.h"
#include "HiEasyX/HiSysGUI/HiSysGUI.h"
#include "HiEasyX/HiGUI/HiGUI.h"

#include "HiEasyX/HiGdiplus.h"
#include "HiEasyX/HiCanvas.h"

#include "HiEasyX/HiWindow.h"

//
#include "HiEasyX/HiDrawingSet.h"

// HiEasyX �����ռ���д
namespace hiex = HiEasyX;

// ���ݾɰ� EasyWin32 �����ռ䣬�뾡��ʹ�� HiEasyX �����ռ�
namespace EasyWin32 = HiEasyX;
