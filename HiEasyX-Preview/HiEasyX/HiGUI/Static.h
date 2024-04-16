/**
 * @file	Static.h
 * @brief	HiGUI �ؼ���֧����̬�ؼ�
 * @author	huidong
*/

#pragma once

#include "ControlBase.h"

namespace HiEasyX
{
	/**
	 * @brief ��̬���ı���ͼ�񣩿ؼ�
	*/
	class Static : public ControlBase
	{
	public:

		struct Char
		{
			TCHAR ch;
			COLORREF cText;
			COLORREF cBk;
		};

	protected:

		std::vector<Char> m_vecText;
		ImageBlock* m_pImgBlock = nullptr;

		virtual void Init();

	public:

		Static();

#ifdef UNICODE
		Static(int x, int y, int w, int h, std::wstring wstrText = L"");
#else
		Static(int x, int y, int w, int h, std::string strText = "");
#endif

#ifdef UNICODE
		virtual std::vector<Char> Convert(std::wstring wstrText);
#else
		virtual std::vector<Char> Convert(std::string strText);
#endif

#ifdef UNICODE
		virtual std::wstring Convert(std::vector<Char> vecText);
#else
		virtual std::string Convert(std::vector<Char> vecText);
#endif

		virtual std::vector<Char>& GetTextVector() { return m_vecText; }

		virtual void ClearText();

		/**
		 * @brief ����ı�
		 * @param[in] wstr				�ı�
		 * @param[in] isSetTextColor	�Ƿ����ô��ı���ɫ
		 * @param[in] cText				�ı���ɫ
		 * @param[in] isSetBkColor		�Ƿ����ô��ı�������ɫ
		 * @param[in] cBk				�ı�����ɫ
		*/
#ifdef UNICODE
		virtual void AddText(
			std::wstring wstr,
			bool isSetTextColor = false,
			COLORREF cText = 0,
			bool isSetBkColor = false,
			COLORREF cBk = 0
			);
#else
		virtual void AddText(
			std::string str,
			bool isSetTextColor = false,
			COLORREF cText = 0,
			bool isSetBkColor = false,
			COLORREF cBk = 0
			);
#endif

#ifdef UNICODE
		void SetText(std::wstring wstrText) override;
#else
		void SetText(std::string strText) override;
#endif

		void SetText(std::vector<Char> vecText);

		/**
		 * @brief ��ȡ�����ͼ��
		*/
		virtual ImageBlock* GetImage() { return m_pImgBlock; }

		/**
		 * @brief ������ʾͼ��
		 * @param[in] pImgBlockmg Ҫ��ʾ��ͼ���
		*/
		virtual void SetImage(ImageBlock* pImgBlockmg);

		void Draw_Text(int nTextOffsetX = 0, int nTextOffsetY = 0) override;

		void Draw(bool draw_child = true) override;
	};
}

