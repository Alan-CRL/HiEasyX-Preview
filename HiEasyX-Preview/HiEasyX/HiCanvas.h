/**
 * @file	HiCanvas.h
 * @brief	HiEasyX ��Ļ���ģ��
 * @author	huidong
*/

#pragma once

#include "HiDrawingProperty.h"
#include <vector>
#include <list>
#include <string>

#include "HiGdiplus.h"

HDC GetImageHDC(IMAGE* pImg);

namespace HiEasyX
{
	/**
	 * @brief ����͸���Ȼ����ɫ
	 * @param[in] cDst		ԭλ������
	 * @param[in] cSrc		���������أ�������͸���Ȼ����ɫ��
	 *
	 * @param[in] isCalculated <pre>
	 *		���������ص��Ƿ��Ѿ���������͸����
	 *
	 *	��ע��
	 *		�˲�������һЩ�������������͸�� png ͼ���е����ؾ����Ѿ��˹�͸���ȵġ�
	 * </pre>
	 *
	 * @param[in] alpha		������ src �ϵ�͸���ȣ�Ĭ��Ϊ 255���������ӣ�
	 * @return ��Ϻ����ɫ������ alpha ֵ��
	*/
	COLORREF MixAlphaColor(COLORREF cDst, COLORREF cSrc, bool isCalculated, BYTE alpha = 255);

	/**
	 * @brief <pre>
	 *		���ٸ���ͼ�񣨿ɿ���͸��ͨ����
	 *
	 *	��ע��
	 *		��δ�����κ�͸��ͨ������ͬ��ֱ�Ӹ���ͼ�񡣴�ʱ������ԭͼ���͸������Ϣ�����򲻱���͸������Ϣ��
	 * </pre>
	 *
	 * @param[in] x					ͼ����� x ����
	 * @param[in] y					ͼ����� y ����
	 * @param[in] pDst				����ͼ��ָ��
	 * @param[in] wDst				����ͼ���
	 * @param[in] hDst				����ͼ���
	 * @param[in] pSrc				�����ͼ��ָ��
	 * @param[in] wSrc				�����ͼ���
	 * @param[in] hSrc				�����ͼ���
	 * @param[in] crop				�����ͼ��ü�����right �� bottom Ϊ 0 ��ʾ���ü���
	 * @param[in] alpha				����͸���ȣ�͸�� 0 ~ 255 ��͸����
	 *
	 * @param[in] bUseSrcAlpha <pre>
	 *		�Ƿ�ʹ�ô����ͼ��͸���Ƚ��л�ϣ��뱣֤ IMAGE �к���͸������Ϣ��
	 *
	 *	��ע��
	 *		EasyX �е�ͼ��һ����͸���ȣ�Ĭ����Ϊ 0����ȫ͸��������һ�㲻ʹ��ԭͼ͸���ȡ�
	 *		ͨ��ֻ�� png ͼ�񣬻����ص����ɵ�ͼ��ź���͸������Ϣ��
	 * </pre>
	 *
	 * @param[in] isCalculated <pre>
	 *		��Ǵ����ͼ���Ƿ��Ѿ�����û�Ϻ����ɫ������ͼ��͸����ʱ��Ч��
	 *
	 *	ע�⣺
	 *		png ͼ��������ɫ���ѽ��й�������㡣
	 *		������ԭͼ��㲻�ټ�������ɫ��ֻ������ͼ�������㡣
	 * </pre>
	*/
	void CopyImage_Alpha(
		int x,
		int y,
		DWORD* pDst, int wDst, int hDst,
		DWORD* pSrc, int wSrc, int hSrc,
		RECT crop = { 0 },
		BYTE alpha = 255,
		bool bUseSrcAlpha = false,
		bool isCalculated = false
	);

	/**
	 * @brief ��תͼ�񣨱���͸����Ϣ������Ӧ��С��
	 * @param[in] pImg			ԭͼ��
	 * @param[in] radian		��ת����
	 * @param[in] bkcolor		���������ɫ
	 * @return	��ת���ͼ��
	*/
	IMAGE RotateImage_Alpha(IMAGE* pImg, double radian, COLORREF bkcolor = BLACK);

	/**
	 * @brief	����ͼ�񣨴ֲڵġ�������ֵ�����ţ�����͸������Ϣ��
	 * @param[in] srcimg		ԭͼ��
	 * @param[in] width			Ŀ����
	 * @param[in] height		Ŀ��߶ȣ�Ϊ 0 ����ݿ�Ȱ��������ţ�
	 * @return ���ź��ͼ��
	*/
	IMAGE ZoomImage_Rough_Alpha(IMAGE* srcimg, int width, int height = 0);

	/**
	 * @brief	����ͼ��˫���Բ�ֵ������͸������Ϣ��
	 * @param[in] srcimg		ԭͼ��
	 * @param[in] width			Ŀ����
	 * @param[in] height		Ŀ��߶ȣ�Ϊ 0 ����ݿ�Ȱ��������ţ�
	 * @return ���ź��ͼ��
	*/
	IMAGE ZoomImage_Alpha(IMAGE* srcimg, int width, int height = 0);

	/**
	 * @brief	ͼ�����ţ����� Win32 API���ȽϿ죬����͸������Ϣ��
	 * @param[in] srcimg		ԭͼ��
	 * @param[in] width			Ŀ����
	 * @param[in] height		Ŀ��߶ȣ�Ϊ 0 ����ݿ�Ȱ��������ţ�
	 * @return ���ź��ͼ��
	*/
	IMAGE ZoomImage_Win32_Alpha(IMAGE* srcimg, int width, int height = 0);

	/**
	 * @brief ����
	*/
	class Canvas : public IMAGE
	{
	public:

		// ����˺������� BindToWindow ����
		friend void BindWindowCanvas(Canvas*, HWND);

	protected:

		/////// ���� ///////

		DrawingProperty m_property;			///< ��������ͼ���ԣ����ڱ���ɵĻ�ͼ����ָ�룩

		DWORD* m_pBuf = nullptr;			///< ͼ���ڴ�ָ��
		int m_nWidth, m_nHeight;			///< ͼ����
		int m_nBufSize;						///< ͼ�����

		bool m_bBindToImgPointer;			///< �û����Ƿ�󶨵�ͼ��ָ��
		IMAGE* m_pImg;						///< �����󶨵�ͼ��ָ�루�������󶨵�ָ�룩

		bool m_bBatchDraw;					///< �Ƿ���������������

		COLORREF m_cGPLineColor = WHITE;	///< GDI+ ��ͼʱʹ�õ�������ɫ
		COLORREF m_cGPFillColor = WHITE;	///< GDI+ ��ͼʱʹ�õ������ɫ
		float m_fGPLineWidth = 1.f;			///< GDI+ ��ͼʱ���������
		bool m_bGPAlpha = false;			///< GDI+ ��ͼʱ�Ƿ�����͸����
		Gdiplus::SmoothingMode m_enuSmoothingMode = Gdiplus::SmoothingModeAntiAlias;	///< GDI+ �����ģʽ

		HWND m_hBindWindow;					///< �󶨵��Ĵ���
		bool m_bAutoMarkFlushWindow = true;	///< �󶨵�����ʱ������Ƿ��ڻ��ƺ��Զ�������Ҫ����˫����

		/////// ���� ///////

		/**
		 * @brief ��մ󲿷�����
		*/
		void CleanUpSettings();

		/**
		 * @brief �������� HiWindow �Ĵ�������������˴��ڣ�
		 * @return �Ƿ������ɹ�
		*/
		bool BeginWindowTask();

		/**
		 * @brief ���� HiWindow �Ĵ�������
		*/
		void EndWindowTask();

		/**
		 * @brief <pre>
		 *		���� EasyX ��ͼ����ǰ�����øû���ΪĿ���ͼ����
		 *
		 *	��ע��
		 *		�����˴��ڣ����Զ�������������
		 * </pre>
		 *
		 * @return �Ƿ����óɹ�
		*/
		bool BeginDrawing();

		/**
		 * @brief ���� EasyX ��ͼ������ϣ��ָ���ǰ�Ļ�ͼ״̬
		*/
		void EndDrawing();

		/**
		 * @brief <pre>
		 *		�������󶨵����ڣ�������С�洰���Զ�������
		 *
		 *	��ע��
		 *		�˺���ֻӦ�ñ� BindWindowCanvas ��������
		 * </pre>
		 *
		 * @param[in] hWnd Ŀ�괰��
		 * @param[in] pImg ����ͼ�񻺳���
		 * @return �˻���
		*/
		Canvas& BindToWindow(HWND hWnd, IMAGE* pImg);

	public:

		/////// ������������ ///////

		Canvas();

		Canvas(int w, int h, COLORREF cBk = BLACK);

		/**
		 * @brief ����ͼ�����ݣ���ͼ��ָ������� BindToImage��
		 * @param[in] pImg ԭͼ��
		*/
		Canvas(IMAGE* pImg);
		Canvas(IMAGE img);

		/**
		 * @brief ����ͼ�����ݣ���ͼ��ָ������� BindToImage��
		 * @param[in] pImg ԭͼ��
		*/
		Canvas& operator= (IMAGE* pImg);
		Canvas& operator= (IMAGE img);

		/**
		 * @brief <pre>
		 *		���¼���ͼ��ߴ���Ϣ
		 *
		 * ��ע��
		 *		������ͼ��ָ�룬���ⲿ����ͼ���С������ô˺���
		 * </pre>
		*/
		void UpdateSizeInfo();

		/**
		 * @brief ���軭����С�������˴��ڣ��򲻽�����ã�
		 * @param[in] w Ŀ����
		 * @param[in] h Ŀ��߶�
		*/
		void Resize(int w, int h) override;

		/**
		 * @brief <pre>
		 *		�󶨵�ͼ��ָ��
		 *
		 *	ע�⣺
		 *		�󶨵�ͼ��ָ���������ⲿ������ͼ���С������Ҫ���� UpdateSizeInfo ���¼���ͼ����Ϣ
		 * </pre>
		 *
		 * @param[in] pImg Ŀ��ͼ��ָ��
		 * @return �˻���
		*/
		Canvas& BindToImage(IMAGE* pImg);

		/**
		 * @brief <pre>
		 *		��ȡ���� IMAGE ָ��
		 *
		 *	ע�⣺
		 *		�е�ʱ�򻭲����˱�� IMAGE�����Ի�ͼʱ����ֱ��ʹ�� this��������ô˺�����
		 * </pre>
		*/
		IMAGE* GetImagePointer() { return m_bBindToImgPointer ? m_pImg : this; }

		/**
		 * @brief �ȼ��� GetImagePointer()
		*/
		IMAGE* Pt() { return m_bBindToImgPointer ? m_pImg : this; }

		/**
		 * @brief ��ȡͼ�񻺳���ָ��
		*/
		DWORD* GetBuffer() const { return m_pBuf; }

		/**
		 * @brief ��ȡͼ�񻺳�����С����ͼ��������� * �ߣ�
		*/
		int GetBufferSize() const { return m_nBufSize; }

		/**
		 * @brief ��ȡ������ HDC
		*/
		HDC GetHDC() { return GetImageHDC(GetImagePointer()); }

		int getwidth() const { return m_nWidth; }
		int getheight() const { return m_nHeight; }
		int GetWidth() const { return m_nWidth; }
		int GetHeight() const { return m_nHeight; }

		/**
		 * @brief <pre>
		 *		�󶨵�����ʱ�������Ƿ���ÿ�λ��ƺ��Զ����ˢ�´���˫����
		 *
		 *	��ע��
		 *		���ˢ�´���˫���岢����ζ�ż�ʱˢ�¡�
		 *		��Ǻ󣬴��ڽ�������һ�ν��ܵ�������Ϣʱ����˫���塣
		 * </pre>
		*/
		void EnableAutoMarkFlushWindow(bool enable);
		bool IsEnableAutoMarkFlushWindow() const { return m_bAutoMarkFlushWindow; }

		/////// ��ͼ״̬���ú��� ///////

		/**
		 * @brief <pre>
		 *		��ʼ���������ƣ��ú����������ڿ���˫���壩
		 *
		 *	��ע��
		 *		���øú����󣬵�ǰ��ͼĿ�꽫ת�Ƶ��û������˺�ÿ�λ��Ʋ���ָ���ͼĿ��
		 * </pre>
		*/
		void BeginBatchDrawing();

		/**
		 * @brief <pre>
		 *		������������
		 *
		 *	��ע��
		 *		��ͼĿ�꽫�ָ�����������ǰ��״̬
		 * </pre>
		*/
		void EndBatchDrawing();

		/////// EasyX ���Ļ�����ͼ���� ///////

		/**
		 * @brief �ж�ĳ���Ƿ�λ��ͼ����
		 * @param[in] x ����
		 * @param[in] y ����
		 * @param[out] pIndex ���ظõ���������
		 * @return �Ƿ�λ��ͼ����
		*/
		bool IsValidPoint(int x, int y, int* pIndex = nullptr);

		/**
		 * @brief <pre>
		 *		�ñ���ɫ��ջ���
		 *
		 *	��ע��
		 *		�˺��������Ա���ɫ��͸���ȣ���ֱ�ӶԻ������� 255 ��͸���ȣ�����͸������
		 * </pre>
		*/
		void Clear(bool isSetColor = false, COLORREF bkcolor = BLACK);

		/**
		 * @brief �ñ���ɫ��ջ����������� Clear �������˺���Ĭ�ϱ�������ɫ�е�͸���ȣ�
		*/
		void Clear_Alpha(bool isSetColor = false, COLORREF bkcolor = BLACK, bool ignore_alpha = false);

		LINESTYLE GetLineStyle();
		void SetLineStyle(LINESTYLE style);
		void SetLineStyle(int style, int thickness = 1, const DWORD* puserstyle = nullptr, DWORD userstylecount = 0);
		void SetLineThickness(int thickness);
		int GetLineThickness();

		FILLSTYLE GetFillStyle();
		void SetFillStyle(FILLSTYLE style);
		void SetFillStyle(int style, long hatch = 0, IMAGE* ppattern = nullptr);
		void SetFillStyle(BYTE* ppattern8x8);

		int GetRop2();
		void SetRop2(int mode);

		int GetPolyFillMode();
		void SetPolyFillMode(int mode);

		COLORREF GetLineColor();
		void SetLineColor(COLORREF color);

		COLORREF GetTextColor();
		void SetTextColor(COLORREF color);

		COLORREF GetFillColor();
		void SetFillColor(COLORREF color);

		COLORREF GetBkColor();
		void SetBkColor(COLORREF color);

		int GetBkMode();
		void SetBkMode(int mode);

		/**
		 * @brief ���û�ͼ״̬Ϊԭʼ״̬
		*/
		void SetDefault();

		COLORREF GetPixel(int x, int y);
		void PutPixel(int x, int y, COLORREF c);

		/**
		 * @brief  ֱ�Ӳ����Դ��ȡ��
		*/
		COLORREF GetPixel_Direct(int x, int y);

		/**
		 * @brief ֱ�Ӳ����Դ���Ƶ�
		*/
		void PutPixel_Direct(int x, int y, COLORREF c);

		/**
		 * @brief ֱ�Ӳ����Դ���ƴ���͸���ȵĵ㣨ʹ�� COLORREF �е�͸���ȣ�
		*/
		void PutPixel_Direct_Alpha(int x, int y, COLORREF c);

		void Line(int x1, int y1, int x2, int y2, bool isSetColor = false, COLORREF c = 0);
		void Line(POINT pt1, POINT pt2, bool isSetColor = false, COLORREF c = 0);

		void Rectangle(int left, int top, int right, int bottom, bool isSetColor = false, COLORREF c = 0);
		void Rectangle(RECT rct, bool isSetColor = false, COLORREF c = 0);
		void FillRectangle(int left, int top, int right, int bottom, bool isSetColor = false, COLORREF cLine = 0, COLORREF cFill = 0);
		void FillRectangle(RECT rct, bool isSetColor = false, COLORREF cLine = 0, COLORREF cFill = 0);
		void SolidRectangle(int left, int top, int right, int bottom, bool isSetColor = false, COLORREF c = 0);
		void SolidRectangle(RECT rct, bool isSetColor = false, COLORREF c = 0);
		void ClearRectangle(int left, int top, int right, int bottom);
		void ClearRectangle(RECT rct);

		void Circle(int x, int y, int radius, bool isSetColor = false, COLORREF c = 0);
		void FillCircle(int x, int y, int radius, bool isSetColor = false, COLORREF cLine = 0, COLORREF cFill = 0);
		void SolidCircle(int x, int y, int radius, bool isSetColor = false, COLORREF c = 0);
		void ClearCircle(int x, int y, int radius);

		void Ellipse(int left, int top, int right, int bottom, bool isSetColor = false, COLORREF c = 0);
		void Ellipse(RECT rct, bool isSetColor = false, COLORREF c = 0);
		void FillEllipse(int left, int top, int right, int bottom, bool isSetColor = false, COLORREF cLine = 0, COLORREF cFill = 0);
		void FillEllipse(RECT rct, bool isSetColor = false, COLORREF cLine = 0, COLORREF cFill = 0);
		void SolidEllipse(int left, int top, int right, int bottom, bool isSetColor = false, COLORREF c = 0);
		void SolidEllipse(RECT rct, bool isSetColor = false, COLORREF c = 0);
		void ClearEllipse(int left, int top, int right, int bottom);
		void ClearEllipse(RECT rct);

		void RoundRect(int left, int top, int right, int bottom, int ellipsewidth, int ellipseheight, bool isSetColor = false, COLORREF c = 0);
		void RoundRect(RECT rct, int ellipsewidth, int ellipseheight, bool isSetColor = false, COLORREF c = 0);
		void FillRoundRect(int left, int top, int right, int bottom, int ellipsewidth, int ellipseheight, bool isSetColor = false, COLORREF cLine = 0, COLORREF cFill = 0);
		void FillRoundRect(RECT rct, int ellipsewidth, int ellipseheight, bool isSetColor = false, COLORREF cLine = 0, COLORREF cFill = 0);
		void SolidRoundRect(int left, int top, int right, int bottom, int ellipsewidth, int ellipseheight, bool isSetColor = false, COLORREF c = 0);
		void SolidRoundRect(RECT rct, int ellipsewidth, int ellipseheight, bool isSetColor = false, COLORREF c = 0);
		void ClearRoundRect(int left, int top, int right, int bottom, int ellipsewidth, int ellipseheight);
		void ClearRoundRect(RECT rct, int ellipsewidth, int ellipseheight);

		void Arc(int left, int top, int right, int bottom, double stangle, double endangle, bool isSetColor = false, COLORREF c = 0);
		void Arc(RECT rct, double stangle, double endangle, bool isSetColor = false, COLORREF c = 0);
		void Pie(int left, int top, int right, int bottom, double stangle, double endangle, bool isSetColor = false, COLORREF c = 0);
		void Pie(RECT rct, double stangle, double endangle, bool isSetColor = false, COLORREF c = 0);
		void FillPie(int left, int top, int right, int bottom, double stangle, double endangle, bool isSetColor = false, COLORREF cLine = 0, COLORREF cFill = 0);
		void FillPie(RECT rct, double stangle, double endangle, bool isSetColor = false, COLORREF cLine = 0, COLORREF cFill = 0);
		void SolidPie(int left, int top, int right, int bottom, double stangle, double endangle, bool isSetColor = false, COLORREF c = 0);
		void SolidPie(RECT rct, double stangle, double endangle, bool isSetColor = false, COLORREF c = 0);
		void ClearPie(int left, int top, int right, int bottom, double stangle, double endangle);
		void ClearPie(RECT rct, double stangle, double endangle);

		void Polyline(const POINT* points, int num, bool isSetColor = false, COLORREF c = 0);
		void Polygon(const POINT* points, int num, bool isSetColor = false, COLORREF c = 0);
		void FillPolygon(const POINT* points, int num, bool isSetColor = false, COLORREF cLine = 0, COLORREF cFill = 0);
		void SolidPolygon(const POINT* points, int num, bool isSetColor = false, COLORREF c = 0);
		void ClearPolygon(const POINT* points, int num);

		void PolyBezier(const POINT* points, int num, bool isSetColor = false, COLORREF c = 0);

		/**
		 * @brief ���ĳ����
		 * @param[in] x				�����ʼλ��
		 * @param[in] y				�����ʼλ��
		 * @param[in] color			�����ɫ
		 *
		 * @param[in] filltype <pre>
		 *		���ģʽ������������ѡ��
		 *		FLOODFILLBORDER		ָ�� color Ϊ���߽���ɫ������������ɫ��ֹͣ���
		 *		FLOODFILLSURFACE	ָ�� color Ϊ��������ɫ����ֻ������ɫ
		 * </pre>
		 *
		 * @param[in] isSetColor		�Ƿ����������ɫ
		 * @param[in] cFill			�����ɫ
		*/
		void FloodFill(int x, int y, COLORREF color, int filltype = FLOODFILLBORDER, bool isSetColor = false, COLORREF cFill = 0);

		/**
		 * @brief ��ָ��λ������ı�
		 * @param[in] x				λ��
		 * @param[in] y				λ��
		 * @param[in] lpszText			�ı�
		 * @param[in] isSetColor		�Ƿ�������ɫ
		 * @param[in] c				�ı���ɫ
		 * @return �ı����ؿ��
		*/
		int OutTextXY(int x, int y, LPCTSTR lpszText, bool isSetColor = false, COLORREF c = 0);

		int OutTextXY(int x, int y, TCHAR ch, bool isSetColor = false, COLORREF c = 0);

		/**
		 * @brief ��ָ��λ�������ʽ���ı�
		 * @param[in] x			λ��
		 * @param[in] y			λ��
		 * @param[in] _Size		��ʽ���ı���󳤶�
		 * @param[in] _Format		��ʽ���ַ���
		 * @param[in]				��������
		 * @return �ı����ؿ��
		*/
		int OutTextXY_Format(int x, int y, int _Size, LPCTSTR _Format, ...);

		/**
		 * @brief ��ȡ�ı����ؿ��
		 * @param[in] lpszText �ı�
		 * @return ��ȡ�ı����ؿ��
		*/
		int TextWidth(LPCTSTR lpszText);

		int TextWidth(TCHAR c);
		int TextHeight(LPCTSTR lpszText);
		int TextHeight(TCHAR c);
		int Draw_Text(LPCTSTR str, RECT* pRect, UINT uFormat, bool isSetColor = false, COLORREF c = 0);
		int Draw_Text(TCHAR ch, RECT* pRect, UINT uFormat, bool isSetColor = false, COLORREF c = 0);

		/**
		 * @brief ��ĳ��������������
		 * @param[in] lpszText			�ı�
		 * @param[in] rct				�������Ĭ��Ϊ��������
		 * @param[in] isSetColor		�Ƿ�������ɫ
		 * @param[in] c					�ı���ɫ
		*/
		void CenterText(LPCTSTR lpszText, RECT rct = { -1 }, bool isSetColor = false, COLORREF c = 0);

		/**
		 * @brief ���������ʽ���ı�
		 * @param[in] _Size			��ʽ���ı���󳤶�
		 * @param[in] _Format		��ʽ���ַ���
		 * @param[in]				��������
		*/
		void CenterText_Format(int _Size, LPCTSTR _Format, ...);

		LOGFONT GetTextStyle();
		void SetTextStyle(int nHeight, int nWidth, LPCTSTR lpszFace);
		void SetTextStyle(int nHeight, int nWidth, LPCTSTR lpszFace, int nEscapement, int nOrientation, int nWeight, bool bItalic, bool bUnderline, bool bStrikeOut);
		void SetTextStyle(int nHeight, int nWidth, LPCTSTR lpszFace, int nEscapement, int nOrientation, int nWeight, bool bItalic, bool bUnderline, bool bStrikeOut, BYTE fbCharSet, BYTE fbOutPrecision, BYTE fbClipPrecision, BYTE fbQuality, BYTE fbPitchAndFamily);
		void SetTextStyle(LOGFONT font);

		/**
		 * @brief ���������С
		 * @param[in] nHeight	�߶�
		 * @param[in] nWidth	��ȣ�Ϊ 0 ʱ���Զ���߶�ƥ�䣩
		*/
		void SetFont(int nHeight, int nWidth = 0);

		/**
		 * @brief ����ʹ�����������
		 * @param[in] lpsz ��������
		*/
		void SetTypeface(LPCTSTR lpsz);

		/**
		 * @brief �����ַ�������д�Ƕȣ���λ 0.1 �ȣ�
		 * @param[in] lfEscapement �Ƕ�
		*/
		void SetTextEscapement(LONG lfEscapement);

		/**
		 * @brief ����ÿ���ַ�����д�Ƕȣ���λ 0.1 �ȣ�
		 * @param[in] lfOrientation �Ƕ�
		*/
		void SetTextOrientation(LONG lfOrientation);

		/**
		 * @brief �����ַ��ıʻ���ϸ����Χ Ĭ�� 0 ~ 1000 ��֣�
		 * @param[in] lfWeight ��ϸ
		*/
		void SetTextWeight(LONG lfWeight);

		/**
		 * @brief ���������Ƿ�Ϊб��
		 * @param[in] lfItalic �Ƿ�ʹ��б��
		*/
		void SetTextItalic(bool lfItalic);

		/**
		 * @brief ���������Ƿ����»���
		 * @param[in] lfUnderline �Ƿ�ʹ���»���
		*/
		void SetTextUnderline(bool lfUnderline);

		/**
		 * @brief ���������Ƿ���ɾ����
		 * @param[in] lfStrikeOut �Ƿ�ʹ��ɾ����
		*/
		void SetTextStrikeOut(bool lfStrikeOut);

		/**
		 * @brief ��ȡǰ��ɫ
		*/
		COLORREF GetColor();

		/**
		 * @brief ����ǰ��ɫ
		 * @param[in] color ǰ��ɫ
		*/
		void SetColor(COLORREF color);

		int GetX();
		int GetY();

		void MoveTo(int x, int y);
		void MoveRel(int dx, int dy);

		void LineTo(int x, int y, bool isSetColor = false, COLORREF c = 0);
		void LineRel(int dx, int dy, bool isSetColor = false, COLORREF c = 0);

		void OutText(LPCTSTR lpszText, bool isSetColor = false, COLORREF c = 0);
		void OutText(TCHAR ch, bool isSetColor = false, COLORREF c = 0);

		/**
		 * @brief �����ʽ���ı�
		 * @param[in] _Size			��ʽ���ı���󳤶�
		 * @param[in] _Format		��ʽ���ַ���
		 * @param[in]				��������
		 * @return �ı����ؿ��
		*/
		int OutText_Format(int _Size, LPCTSTR _Format, ...);

		/**
		 * @brief <pre>
		 *		����ͼƬ�ļ�������
		 *
		 *	��ע��
		 *		������͸��ͨ�������Ƶ������ϵ����ݲ��ᱣ��ԭͼ���͸������Ϣ
		 * </pre>
		 *
		 * @param[in] lpszImgFile		ͼ���ļ�·��
		 * @param[in] x					�����������λ��
		 * @param[in] y					�����������λ��
		 * @param[in] bResize			�Ƿ����������С����������ͼ�񣨶����޿�ߵĻ������Զ�������С��
		 * @param[in] nWidth			ͼ��Ŀ������ߴ磬Ϊ 0 ��ʾ������
		 * @param[in] nHeight			ͼ��Ŀ������ߴ磬Ϊ 0 ��ʾ������
		 * @param[in] alpha				����͸����
		 * @param[in] bUseSrcAlpha		�Ƿ�ʹ��ԭͼ��͸������Ϣ���л�ϣ���֧����͸������Ϣ�� png ͼ��
		 * @param[in] isCalculated		ԭͼ�Ƿ��Ѿ����͸����
		 * @return ��ȡ���� IMAGE ����
		*/
		IMAGE Load_Image_Alpha(
			LPCTSTR lpszImgFile,
			int x = 0,
			int y = 0,
			bool bResize = false,
			int nWidth = 0,
			int nHeight = 0,
			BYTE alpha = 255,
			bool bUseSrcAlpha = false,
			bool isCalculated = false
		);

		/**
		 * @brief ����ͼ�񵽸û���
		 * @param[in] x					ͼ������λ��
		 * @param[in] y					ͼ������λ��
		 * @param[in] pImg				������ͼ��
		 * @param[in] crop				�ü�����
		 * @param[in] alpha				����͸����
		 * @param[in] bUseSrcAlpha		�Ƿ�ʹ��ԭͼ͸����
		 * @param[in] isCalculated		ԭͼ�Ƿ��Ѿ����͸����
		*/
		void PutImageIn_Alpha(
			int x,
			int y,
			IMAGE* pImg,
			RECT crop = { 0 },
			BYTE alpha = 255,
			bool bUseSrcAlpha = false,
			bool isCalculated = false
		);

		/**
		 * @brief ���û�����ͼ����Ƶ���һ������
		 * @param[in] x				����λ��
		 * @param[in] y				����λ��
		 * @param[in] pImg			Ŀ����ƻ���
		 * @param[in] crop			�ü�����Ĭ�ϲ��ü���
		 * @param[in] alpha			����͸����
		 * @param[in] bUseSrcAlpha	�Ƿ�ʹ�ô˻���͸����
		 * @param[in] isCalculated	���������Ƿ��Ѿ�͸�����
		*/
		void RenderTo(
			int x,
			int y,
			IMAGE* pImg = nullptr,
			RECT crop = { 0 },
			BYTE alpha = 255,
			bool bUseSrcAlpha = false,
			bool isCalculated = false
		);

		/**
		 * @brief EasyX ԭ����ת����
		 * @param[in] radian		��ת����
		 * @param[in] bkcolor		��䱳��ɫ
		 * @param[in] autosize		�Ƿ�����Ӧ��תͼ���С
		 * @param[in] highquality	������
		*/
		void RotateImage(double radian, COLORREF bkcolor = BLACK, bool autosize = false, bool highquality = true);

		/**
		 * @brief ��תͼ�񣨱��� Alpha ��Ϣ��
		 * @param[in] radian	��ת����
		 * @param[in] bkcolor	��䱳��ɫ
		*/
		void RotateImage_Alpha(double radian, COLORREF bkcolor = BLACK);

		/**
		 * @brief ����ͼ�񣨴ֲڵġ�������ֵ�����ţ�����͸������Ϣ��
		 * @param[in] nW	Ŀ����
		 * @param[in] nH	Ŀ��߶ȣ�Ϊ 0 ����ݿ�Ȱ��������ţ�
		*/
		void ZoomImage_Rough_Alpha(int nW, int nH = 0);

		/**
		 * @brief ����ͼ��˫���Բ�ֵ������͸������Ϣ��
		 * @param[in] nW	Ŀ����
		 * @param[in] nH	Ŀ��߶ȣ�Ϊ 0 ����ݿ�Ȱ��������ţ�
		*/
		void ZoomImage_Alpha(int nW, int nH = 0);

		/**
		 * @brief ����ͼ�񣨻��� Win32 API���ȽϿ죬����͸������Ϣ��
		 * @param[in] nW	Ŀ����
		 * @param[in] nH	Ŀ��߶ȣ�Ϊ 0 ����ݿ�Ȱ��������ţ�
		*/
		void ZoomImage_Win32_Alpha(int nW, int nH = 0);

		/**
		 * @brief ����ͼ�񣨻��� Win32 API���ȽϿ죬����͸������Ϣ��
		 * @param[in] nW	Ŀ����
		 * @param[in] nH	Ŀ��߶ȣ�Ϊ 0 ����ݿ�Ȱ��������ţ�
		*/
		void ZoomImage_Gdiplus_Alpha(int nW, int nH = 0);

		/////// GDI+ ��ػ�ͼ���� ///////

		////////////////////////////////////////////////////////////////////
		//
		// ע�⣺
		//		GDI+ ��ͼ�������� EasyX ԭ����������ͬ���Ļ�ͼ��ɫ��
		//		������Ϊ GDI+ �Ļ�ͼ����֧��͸������ EasyX ԭ��������֧�֡�
		//
		//		����ʹ�� RGBA �� SET_ALPHA �����ô�͸���ȵ���ɫ
		//
		////////////////////////////////////////////////////////////////////

		void GP_SetLineColor(COLORREF color);
		void GP_SetFillColor(COLORREF color);
		void GP_SetLineWidth(float width);

		COLORREF GP_GetLineColor() const { return m_cGPLineColor; }
		COLORREF GP_GetFillColor() const { return m_cGPFillColor; }
		float GP_GetLineWidth() const { return m_fGPLineWidth; }

		/**
		 * @brief ���� GDI+ ����ʱ�Ƿ�ʹ��͸���ȣ�Ĭ�ϲ�ʹ�ã�
		*/
		void GP_EnableAlpha(bool enable);

		/**
		 * @brief ���� GDI+ �����ģʽ
		*/
		void GP_SetSmoothingMode(Gdiplus::SmoothingMode smoothing_mode);

		bool GP_IsEnbaleAlpha() const { return m_bGPAlpha; }
		Gdiplus::SmoothingMode GP_GetSmoothingMode() const { return m_enuSmoothingMode; }

		void GP_Line(float x1, float y1, float x2, float y2, bool isSetColor = false, COLORREF linecolor = 0);

		void GP_Polygon(int points_num, POINT* points, bool isSetColor = false, COLORREF linecolor = 0);
		void GP_SolidPolygon(int points_num, POINT* points, bool isSetColor = false, COLORREF fillcolor = 0);
		void GP_FillPolygon(int points_num, POINT* points, bool isSetColor = false, COLORREF linecolor = 0, COLORREF fillcolor = 0);

		void GP_Rectangle(float x, float y, float w, float h, bool isSetColor = false, COLORREF linecolor = 0);
		void GP_SolidRectangle(float x, float y, float w, float h, bool isSetColor = false, COLORREF fillcolor = 0);
		void GP_FillRectangle(float x, float y, float w, float h, bool isSetColor = false, COLORREF linecolor = 0, COLORREF fillcolor = 0);

		void GP_RoundRect(float x, float y, float w, float h, float ellipsewidth, float ellipseheight, bool isSetColor = false, COLORREF linecolor = 0);
		void GP_SolidRoundRect(float x, float y, float w, float h, float ellipsewidth, float ellipseheight, bool isSetColor = false, COLORREF fillcolor = 0);
		void GP_FillRoundRect(float x, float y, float w, float h, float ellipsewidth, float ellipseheight, bool isSetColor = false, COLORREF linecolor = 0, COLORREF fillcolor = 0);

		void GP_Ellipse(float x, float y, float w, float h, bool isSetColor = false, COLORREF linecolor = 0);
		void GP_SolidEllipse(float x, float y, float w, float h, bool isSetColor = false, COLORREF fillcolor = 0);
		void GP_FillEllipse(float x, float y, float w, float h, bool isSetColor = false, COLORREF linecolor = 0, COLORREF fillcolor = 0);

		void GP_Pie(float x, float y, float w, float h, float stangle, float endangle, bool isSetColor = false, COLORREF linecolor = 0);
		void GP_SolidPie(float x, float y, float w, float h, float stangle, float endangle, bool isSetColor = false, COLORREF fillcolor = 0);
		void GP_FillPie(float x, float y, float w, float h, float stangle, float endangle, bool isSetColor = false, COLORREF linecolor = 0, COLORREF fillcolor = 0);

		void GP_Arc(float x, float y, float w, float h, float stangle, float endangle, bool isSetColor = false, COLORREF linecolor = 0);
	};

	/**
	 * @brief ͼ���
	*/
	class ImageBlock
	{
	private:
		Canvas* m_pCanvas = nullptr;
		bool m_isCreated = false;			///< �����Ƿ�Ϊ�Լ�������

		void DeleteMyCanvas();				///< ɾ���Լ������Ļ���

	public:
		int x = 0, y = 0;					///< ͼ����ʾ��ͼ���λ��
		RECT rctCrop = { 0 };				///< �ü���Ϣ
		bool bUseSrcAlpha = false;			///< �Ƿ�ʹ��ͼ������� alpha ����

		bool isAlphaCalculated = false;		///< ͼ��ɫֵ�Ƿ��ѻ��͸���ȣ�ʹ������͸����ʱ��Ч��

		BYTE alpha = 255;					///< ���Ƶ�ͼ��ʱ�ĵ���͸����
		bool bVisible = true;				///< ͼ���Ƿ�ɼ�

		ImageBlock();

		ImageBlock(Canvas* pCanvas);

		ImageBlock(int _x, int _y, Canvas* pCanvas);

		/**
		 * @brief �½�����
		 * @param[in] _x		λ��
		 * @param[in] _y		λ��
		 * @param[in] w		���
		 * @param[in] h		�߶�
		 * @param[in] cBk		����ɫ
		*/
		ImageBlock(int _x, int _y, int w, int h, COLORREF cBk = 0);

		virtual ~ImageBlock();

		/**
		 * @brief �����ⲿ������ֱ���½�����
		 * @param[in] w		���
		 * @param[in] h		�߶�
		 * @param[in] cBk		����ɫ
		 * @return ����
		*/
		Canvas* CreateCanvas(int w, int h, COLORREF cBk = 0);

		Canvas* GetCanvas() const { return m_pCanvas; }
		void SetCanvas(Canvas* pCanvas);

		int GetWidth() const { return m_pCanvas ? m_pCanvas->GetWidth() : 0; }
		int GetHeight() const { return m_pCanvas ? m_pCanvas->GetHeight() : 0; }
		POINT GetPos() const { return { x,y }; }
		void SetPos(int _x, int _y);

		/**
		 * @brief ���Ƶ�����
		 * @param[in] pImg		Ŀ����ƻ���
		 * @param[in] _alpha	����͸����
		*/
		virtual void Render(IMAGE* pImg, BYTE _alpha);
	};

	/**
	 * @brief ͼ��
	*/
	class Layer : public std::list<ImageBlock*>
	{
	private:
		DrawingProperty m_property[2];		///< �����ϴεĻ�ͼ����

	public:
		bool bVisible = true;				///< ͼ���Ƿ�ɼ�
		BYTE alpha = 255;					///< ͼ��������ͼ���ĵ���͸����
		bool bOutline = false;				///< �Ƿ���ʾ����
		bool bText = false;					///< ����ʾ�������Ƿ���ʾ����

		/**
		 * @brief ��Ⱦ������
		 * @param[in] pImg				Ŀ����ƻ���
		 * @param[in] bShowOutline		�Ƿ���ʾ����
		 * @param[in] bShowText		�Ƿ���ʾ�����ı�
		 * @param[in] wstrAddedText	���������ı�
		*/
#ifdef UNICODE
		void Render(IMAGE* pImg = nullptr, bool bShowOutline = false, bool bShowText = true, std::wstring wstrAddedText = L"");
#else
		void Render(IMAGE* pImg = nullptr, bool bShowOutline = false, bool bShowText = true, std::string wstrAddedText = "");
#endif
	};

	/**
	 * @brief ����ͼ��˳���ʶ
	*/
	enum LayerOrder
	{
		LAYERORDER_BOTTOM_MOST,
		LAYERORDER_BOTTOM,
		LAYERORDER_NORMAL,
		LAYERORDER_TOP,
		LAYERORDER_TOP_MOST
	};

	/**
	 * @brief <pre>
	 *		����
	 *
	 *	��ע��
	 *		ͼ������Խ��ͼ��Խ��ǰ
	 * </pre>
	*/
	class Scene : public std::vector<Layer*>
	{
	private:

		DrawingProperty m_property;				///< ����֮ǰ�Ļ�ͼ����

		// ����ͼ��
		Layer m_layerBottomMost;				///< ��ײ�
		Layer m_layerBottom;					///< �ײ�
		Layer m_layerTop;						///< ����
		Layer m_layerTopMost;					///< ���

	public:

		/**
		 * @brief <pre>
		 *		��ȡ����ͼ��Ŀ���
		 *
		 *	��ע��
		 *		ͼ������Խ��ͼ��Խ��ǰ
		 * </pre>
		 *
		 * @return ����ͼ��Ŀ���
		*/
		std::vector<Layer*> GetAllLayer();

		/**
		 * @brief ��ȡ����ͼ�������
		*/
		size_t GetAllLayerSize() const;

		/**
		 * @brief <pre>
		 *		��ȡ����ͼ�㣨������ͨͼ���������ͼ�㣬�� LayerOrder��
		 *
		 *	��ע��
		 *		��������������ͼ��
		 * </pre>
		 *
		 * @param[in] order ����ͼ������
		 * @return ����ͼ��
		*/
		Layer* GetSpecialLayer(LayerOrder order);

		/**
		 * @brief ��Ⱦ������
		 * @param[in] pImg				Ŀ����ƻ���
		 * @param[in] bShowAllOutline	�Ƿ���ʾ����
		 * @param[in] bShowAllText		�Ƿ���ʾ�����ı�
		*/
		void Render(IMAGE* pImg = nullptr, bool bShowAllOutline = false, bool bShowAllText = true);
	};
	}

////////////////********* �궨�� *********////////////////

//
// ׼������͸��ͼ�Σ��Ȼ���ͼ�ε���ʱ�����У�Ȼ�����������Ҫ���Ƶĵط���
// nGraphW, nGraphH		������͸��ͼ�εĿ��
//
// ע�⣺��Ҫ��� DRAW_TNS_RENDER_TO ��ʹ��
//
// ʹ�÷�����
//		�� DRAW_TNS_INIT_GRAPHICS ��� DRAW_TNS_RENDER_TO ��֮�䣬����һ������顣
//		������������ʹ�� Canvas ���� graphics ���л��ơ�
//		����ʱ���� Canvas ����ͨ��ͼ�������ɣ����� GDI+ ϵ�з�װ�������� "GP_" ǰ׺�ĺ�����
//		����ֱ��ʹ��ԭ�� EasyX �������л���Ҳ���ԡ�
//
// ʹ��ʾ����
/*
	// ׼������͸��ͼ�Σ�����ͼ�εĿ�ߣ�
	DRAW_TNS_INIT_GRAPHICS(201, 201);
	{
		// �ڴ������ʹ�� Canvas ����ͨ��ͼ�������л��Ƽ���
		graphics.SetLineThickness(5);
		graphics.FillRoundRect(0, 0, 200, 200, 20, 20, true, GREEN, PURPLE);

		// ������ʹ�� EasyX ԭ����������Ҳ����
		line(20, 20, 50, 50);
	}
	// ���ѡ�����͸��ͼ�λ��Ƶ���������û���͸����
	DRAW_TNS_RENDER_TO(120, 120, yourImagePointer, 100);
*/
//
#define DRAW_TNS_INIT_GRAPHICS(nGraphW, nGraphH) \
	{\
		hiex::Canvas graphics(nGraphW, nGraphH);\
		graphics.BeginBatchDrawing();(0)

//
// ��ɻ���͸��ͼ�Σ���������Ƶ�ͼ��
// nRenderX		���λ�� X ����
// nRenderY		���λ�� Y ����
// pDstImg		͸��ͼ�������Ŀ�껭����IMAGE*��
// alpha		���ͼ��ʱʹ�õ�͸���ȣ���ȫ͸�� 0 ~ 255 ��͸����
//
// ע�⣺��Ҫ��� DRAW_TNS_INIT_GRAPHICS ��ʹ�ã������÷��� DRAW_TNS_INIT_GRAPHICS ���ע��
//
#define DRAW_TNS_RENDER_TO(nRenderX, nRenderY, pDstImg, alpha) \
		graphics.EndBatchDrawing();\
		ReverseAlpha(graphics.GetBuffer(), graphics.GetBufferSize());\
		graphics.RenderTo(nRenderX, nRenderY, pDstImg, { 0 }, alpha, true);\
	}(0)
// End of file
