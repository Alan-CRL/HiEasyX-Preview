/**
 * @file	HiFunc.h
 * @brief	HiEasyX ��ĳ��������
 * @author	huidong
*/

#pragma once

#include "EasyX/EasyXType.h"

#include <Windows.h>
#include <WinUser.h>
#include <iostream>
#include <time.h>

#include "HiGdiplus.h"

/**
 * @brief �洢������Ļ�Ĵ�С��Ϣ������ʾ����
*/
struct ScreenSize
{
	int left;	///< ����ʾ�������Ͻ� x ����
	int top;	///< ����ʾ�������Ͻ� y ����
	int w;		///< ����ʾ�����ܺͿ��
	int h;		///< ����ʾ�����ܺ͸߶�
};

/**
 * @brief ��ȡ����ʾ����С��Ϣ
*/
ScreenSize GetScreenSize();

/**
 * @brief <pre>
 *		��ȡͼ��ߴ�
 *
 *	��ע��
 *		���Է���ش��� IMAGE ָ��Ϊ�գ���ָ������ͼ���ڵ����
 * </pre>
 *
 * @param[in] pImg			Ŀ��ͼ��
 * @param[out] width		����ͼ���
 * @param[out] height		����ͼ���
*/
void GetImageSize(IMAGE* pImg, int* width, int* height);

/**
 * @brief <pre>
 *		��תͼ�� Alpha ֵ
 *
 *	��ע��
 *		�� alpha ֵ��Ϊ 0 ��һ�����ص� alpha ��Ϊ 0��
 *		ͬʱ�� alpha ֵΪ 0 ��һ�����ص� alpha ��Ϊ 255��
 * </pre>
 *
 * @param[in, out] pBuf		�Դ�ָ��
 * @param[in] size			�Դ��С
 * @return �Դ�ָ�루��ԭ��һ����
*/
DWORD* ReverseAlpha(DWORD* pBuf, int size);

/**
 * @brief <pre>
 *		����ָ���ߴ缰��ɫ��ͼ��
 *
 *	��ע��
 *		color ���Դ���͸���ȡ�
 *		enable_alpha ��Ϊ false ��Ὣ����ͼ��͸������Ϊ 255��
 * </pre>
 *
 * @param[in] w					����ͼ����
 * @param[in] h					����ͼ��߶�
 * @param[in] color				����ͼ�������ɫ
 * @param[in] enable_alpha		�Ƿ�����ͼ��� alpha ��Ϣ
 * @return �Դ�ָ�루��ԭ��һ����
*/
IMAGE CreateImageColor(int w, int h, COLORREF color, bool enable_alpha);

/**
 * @brief <pre>
 *		����ͼ��ı�����ɫ
 *
 *	��ע��
 *		color ���Դ���͸���ȡ�
 *		enable_alpha ��Ϊ false ��Ὣͼ��͸������Ϊ 255��
 * </pre>
 *
 * @param[in] img				���õ�ͼ��
 * @param[in] color				ͼ�������ɫ
 * @param[in] enable_alpha		�Ƿ�������ɫ�� alpha ��Ϣ
 * @return �Դ�ָ�루��ԭ��һ����
*/
void SetImageColor(IMAGE& img, COLORREF color, bool enable_alpha);

/**
 * @brief �õ� IMAGE ����� HBITMAP
 * @param[in] img			Ŀ��ͼ��
 *
 * @param[in] enable_alpha <pre>
 *		�Ƿ�����ͼ��� alpha ��Ϣ
 *
 *	ע�⣺
 *		��ͼ�� alpha ֵȫΪ 0�����ʾ������͸�����
 * </pre>
 *
 * @return ת���õ���λͼ���
*/
HBITMAP Image2Bitmap(IMAGE* img, bool enable_alpha);

/**
 * @brief �õ� HBITMAP ����� IMAGE
 * @param[in] img			Ŀ��ͼ��
 *
 * @param[in] enable_alpha <pre>
 *		�Ƿ�����ͼ��� alpha ��Ϣ
 *
 *	ע�⣺
 *		��ͼ�� alpha ֵȫΪ 0�����ʾ������͸�����
 * </pre>
 *
 * @return ת���õ���λͼ���
*/
IMAGE Bitmap2Image(HBITMAP* hBitmap, bool enable_alpha);

/**
 * @brief HBITMAP ת HICON
 * @param[in] hBmp λͼ���
 * @return ͼ����
*/
HICON Bitmap2Icon(HBITMAP hBmp);

/**
 * @brief ��ȷ��ʱ���������Ծ�ȷ�� 1ms������ ��1ms��
 * @author yangw80 <yw80@qq.com>
 * @date 2011-5-4
 * @param[in] ms ��ʱ���ȣ���λ�����룩
*/
void HpSleep(int ms);

/**
 * @brief ���Ƿ�λ�ھ�����
 * @param[in] x		λ��
 * @param[in] y		λ��
 * @param[in] rct		����
 * @return ���Ƿ�λ�ھ�����
*/
bool IsInRect(int x, int y, RECT rct);

/**
 * @brief ��ȡ ExMessage ����Ϣ����
 * @param[in] msg ��Ϣ
 * @return EM_ ��Ϣ�����е�һ�֣���ʧ�ܷ��� 0
*/
UINT GetExMessageType(ExMessage msg);

/**
 * @brief ���ô���͸����
 * @param[in] HWnd ���ھ��
 * @param[in] enable �Ƿ����ô���͸����
 * @param[in] alpha ����͸����ֵ 0-255
*/
void SetWindowTransparent(HWND HWnd, bool enable, int alpha = 0xFF);

namespace HiEasyX
{
	/**
	 * @brief ����ͼ�񣨿ɰ���͸��ͨ����
	 * @param[in] Dstimg ָ��Ŀ��λͼ��ָ�루���ֱ�ӻ��Ƶ������������� hiex::GetWindowImage() ��
	 * @param[in] DstimgX Ŀ��λͼ�ϻ��Ƶ����ϽǺ�����
	 * @param[in] DstimgY Ŀ��λͼ�ϻ��Ƶ����Ͻ�������
	 * @param[in] Srcimg ָ��Դλͼ��ָ��
	 * @param[in] transparency ����͸����
	 *
	 *	ע�⣺
	 *		���Ƴ�����λͼ��С���� Srcimg �е�ͼ��һ��
	 * </pre>
	*/
	void TransparentImage(IMAGE* Dstimg, int DstimgX, int DstimgY, IMAGE* Srcimg, int transparency = 255);

	/**
	 * @brief ����ͼ�񣨿ɰ���͸��ͨ����
	 * @param[in] Dstimg ָ��Ŀ��λͼ��ָ�루���ֱ�ӻ��Ƶ������������� hiex::GetWindowImage() ��
	 * @param[in] DstimgX Ŀ��λͼ�ϻ��Ƶ����ϽǺ�����
	 * @param[in] DstimgY Ŀ��λͼ�ϻ��Ƶ����Ͻ�������
	 * @param[in] DstimgWidth Ŀ��λͼ�ϻ��ƵĿ��
	 * @param[in] DstimgHeight Ŀ��λͼ�ϻ��Ƶĸ߶�
	 * @param[in] Srcimg ָ��Դλͼ��ָ��
	 * @param[in] SrcimgX Դλͼ��Ҫ���Ƶ���������ϽǺ�����
	 * @param[in] SrcimgY Դλͼ��Ҫ���Ƶ���������Ͻ�������
	 * @param[in] SourceWidth Դλͼ��Ҫ���Ƶ�����Ŀ��
	 * @param[in] SourceHeight Դλͼ��Ҫ���Ƶ�����ĸ߶�
	 * @param[in] transparency ����͸����
	 *
	 *	ע�⣺
	 *		�� Ŀ��λͼ�ϻ��ƵĿ�� ������ Դλͼ��Ҫ���Ƶ�����Ŀ�ߣ���ôԴλͼ�е����򽫻ᱻ�����ѹ������ӦĿ��λͼ�ϵĻ�������
	 * </pre>
	*/
	void TransparentImage(IMAGE* Dstimg, int DstimgX, int DstimgY, int DstimgWidth, int DstimgHeight, IMAGE* Srcimg, int SrcimgX, int SrcimgY, int SourceWidth, int SourceHeight, int transparency = 255);

	/**
	 * @brief ����ͼ�񣨿ɰ���͸��ͨ����
	 * @param[in] img ָ��ĿԴλͼ��ָ��
	 * @param[in] w ָ���������Ŀ�ȣ�Ϊ 0 �򰴸߶ȱ������ţ�
	 * @param[in] h ָ���������ĸ߶ȣ�Ϊ 0 �򰴿�ȱ������ţ�
	 *
	 *	ע�⣺
	 *		�������ͼ���ַ���䣬�ɲ����ձ�������
	 * </pre>
	*/
	bool ZoomImage_Gdiplus_Alpha(IMAGE* img, int w, int h);

	/**
	 * @brief �ںϣ����ӣ�ͼ��͸����
	 * @param[in] img ָ��ĿԴλͼ��ָ��
	 * @param[in] transparency ��Ҫ�ںϣ����ӣ���͸����
	 * </pre>
	*/
	void OverlayImageTransparency(IMAGE* img, int transparency);

	/**
	 * @brief �Ƴ�ͼ��͸��ͨ��
	 * @param[in] img ָ��ĿԴλͼ��ָ��
	 * </pre>
	*/
	void RemoveImageTransparency(IMAGE* img);
}
