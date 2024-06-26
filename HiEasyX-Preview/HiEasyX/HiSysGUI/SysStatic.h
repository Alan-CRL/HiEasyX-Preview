/**
 * @file	SysStatic.h
 * @brief	HiSysGUI �ؼ���֧����̬�ؼ�
 * @author	huidong
*/

#pragma once

#include "SysControlBase.h"

#include "../EasyX/EasyXType.h"

namespace HiEasyX
{
	/**
	 * @brief ϵͳ��̬�ؼ�
	*/
	class SysStatic : public SysControlBase
	{
	protected:

		void RealCreate(HWND hParent) override;

	public:

		SysStatic();

#ifdef UNICODE
		SysStatic(HWND hParent, RECT rct, std::wstring strText = L"");
		SysStatic(HWND hParent, int x, int y, int w, int h, std::wstring strText = L"");
#else
		SysStatic(HWND hParent, RECT rct, std::string strText = "");
		SysStatic(HWND hParent, int x, int y, int w, int h, std::string strText = "");
#endif

		/**
		 * @brief �����ı�����
		 * @param[in] center �Ƿ����
		*/
		void Center(bool center);

		/**
		 * @brief ����ͼƬ
		 * @param[in] enable	�Ƿ�����ͼ��
		 * @param[in] img		ͼ��
		*/
		void Image(bool enable, IMAGE* img = nullptr);
	};
}
