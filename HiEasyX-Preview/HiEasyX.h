/**
 * @file		HiEasyX.h
 * @brief		HiEasyX 库头文件
 * @note		HiEasyX 是基于 EasyX 的扩展库，支持创建多窗口、透明抗锯齿绘图、系统 UI 组件等等。
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
 * @attention	如果使用MinGW和TDM等编译器，则需要添加链接选项:-leasyx -lmsimg32 -lgdiplus -lgdi32 -lwinmm
 * 				需要使用 C++14 及以上标准，编译时需选择 ISO C++14 (或以上标准)标准或添加编译选项:-std=c++14 (或以上标准)
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

// HiEasyX 命名空间缩写
namespace hiex = HiEasyX;

// 兼容旧版 EasyWin32 命名空间，请尽量使用 HiEasyX 命名空间
namespace EasyWin32 = HiEasyX;
