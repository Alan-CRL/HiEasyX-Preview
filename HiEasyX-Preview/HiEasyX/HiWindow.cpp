#include "EasyX/EasyX.h"
#include "HiWindow.h"

#include "HiMacro.h"
#include "HiIcon.h"
#include "HiStart.h"
#include "HiGdiplus.h"
#include "HiCanvas.h"
#include "HiSysGUI/SysControlBase.h"

// Ԥ����Ϣ�ռ�
#define MSG_RESERVE_SIZE		100

// Ԥ���ؼ��ռ�
#define SYSCTRL_RESERVE_SIZE	100

namespace HiEasyX
{
	////////////****** ȫ�ֱ��� ******////////////

	WNDCLASSEX				g_WndClassEx;								///< ������
	TCHAR					g_lpszClassName[] = _T("HiEasyX");			///< ��������
	ScreenSize				g_screenSize;								///< ��ʾ����Ϣ
	HWND					g_hConsole;									///< ����̨���
	HINSTANCE				g_hInstance = GetModuleHandle(0);			///< ����ʵ��

	std::vector<EasyWindow>	g_vecWindows;								///< ���ڱ�����ര�ڣ�
	int						g_nFocusWindowIndex = NO_WINDOW_INDEX;		///< ��ǰ�������㴰������

	bool					g_isInTask = false;							///< ��Ǵ���������

	HICON					g_hIconDefault;								///< Ĭ�ϳ���ͼ��
	LPCTSTR					g_lpszCustomIcon = nullptr;					///< �Զ������ͼ����Դ��Ϊ�ձ�ʾ��ʹ��
	LPCTSTR					g_lpszCustomIconSm = nullptr;
	HICON					g_hCustomIcon;								///< �Զ������ͼ��
	HICON					g_hCustomIconSm;

	bool					g_isPreStyle = false;						///< �Ƿ�Ԥ�贰����ʽ
	bool					g_isPreStyleEx = false;						///< �Ƿ�Ԥ�贰����չ��ʽ
	bool					g_isPrePos = false;							///< �Ƿ�Ԥ�贰��λ��
	bool					g_isPreShowState = false;					///< �Ƿ�Ԥ�贰����ʾ״̬
	long					g_lPreStyle;								///< ��������ǰ��Ԥ����ʽ
	long					g_lPreStyleEx;								///< ��������ǰ��Ԥ����չ��ʽ
	POINT					g_pPrePos;									///< ��������ǰ��Ԥ�贰��λ��
	int						g_nPreCmdShow;								///< ��������ǰ��Ԥ����ʾ״̬

	DrawMode				g_fDrawMode = DM_Normal;					///< ȫ�ֻ���ģʽ
	bool					g_bAutoFlush = true;						///< �Ƿ��Զ�ˢ��˫����

	UINT					g_uWM_TASKBARCREATED;						///< ϵͳ��������Ϣ����

	////////////****** �������� ******////////////

	// ���鴰�������Ƿ�Ϸ�
	bool IsValidWindowIndex(int index)
	{
		return index >= 0 && index < (int)g_vecWindows.size();
	}

	// ��ǰ�Ƿ���ڲ������㴰�ڣ������ڣ���һ���ǻ�ڣ�
	bool IsFocusWindowExisted()
	{
		return IsValidWindowIndex(g_nFocusWindowIndex);
	}

	// ��ȡ��ǰ�������㴰��
	EasyWindow& GetFocusWindow()
	{
		static EasyWindow wndEmpty;
		if (IsFocusWindowExisted())
		{
			return g_vecWindows[g_nFocusWindowIndex];
		}
		else
		{
			wndEmpty = {};
			return wndEmpty;
		}
	}

	// ͨ�������ô˴����ڴ��ڼ�¼���е�����
	// ���� nullptr ����ǰ�����
	// δ�ҵ����� NO_WINDOW_INDEX
	int GetWindowIndex(HWND hWnd)
	{
		if (hWnd == nullptr)
		{
			return g_nFocusWindowIndex;
		}
		int index = NO_WINDOW_INDEX;
		for (int i = 0; i < (int)g_vecWindows.size(); i++)
		{
			if (hWnd == g_vecWindows[i].hWnd)
			{
				index = i;
				break;
			}
		}
		return index;
	}

	bool IsAnyWindow()
	{
		for (auto& i : g_vecWindows)
			if (i.isAlive)
				return true;
		return false;
	}

	bool IsAliveWindow(HWND hWnd)
	{
		if (hWnd)
		{
			int index = GetWindowIndex(hWnd);
			if (IsValidWindowIndex(index))
			{
				return g_vecWindows[index].isAlive;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return IsFocusWindowExisted();
		}
	}

	bool IsAliveWindow(int index)
	{
		return IsValidWindowIndex(index) && g_vecWindows[index].isAlive;
	}

	// �ȴ������ڲ���Ϣ�������
	void WaitForProcessing(int index)
	{
		// �����ڿ����������٣��ʲ��� isAliveWindow
		if (IsValidWindowIndex(index))
		{
			while (g_vecWindows[index].isBusyProcessing)
			{
				HpSleep(1);
			}
		}
	}

	// �� IMAGE ���ݸ��Ƶ� HDC ��
	// pImg		ԭͼ��
	// hdc		���Ƶ� HDC
	// rct		�� HDC �ϵĻ�������
	void CopyImageToHDC(IMAGE* pImg, HDC hdc, RECT rct)
	{
		//HDC hdc = GetDC(hWnd);
		HDC hdcImg = GetImageHDC(pImg);
		BitBlt(hdc, rct.left, rct.top, rct.right, rct.bottom, hdcImg, 0, 0, SRCCOPY);
		//ReleaseDC(hWnd, hdc);
	}

	void WaitForTask(HWND hWnd)
	{
		// δ���þ��ʱֻ��Ҫ�ȴ���������������Ҫ�жϸþ���Ƿ��Ӧ�����
		if (!hWnd || (IsFocusWindowExisted() && GetFocusWindow().hWnd == hWnd))
		{
			while (g_isInTask)
			{
				HpSleep(1);
			}
		}
	}

	// �ͷŴ����ڴ�
	void FreeWindow(int index)
	{
		if (!IsValidWindowIndex(index))
		{
			return;
		}

		// �ͷŻ�ͼ����
		if (g_vecWindows[index].pImg)
		{
			delete g_vecWindows[index].pImg;
			g_vecWindows[index].pImg = nullptr;
		}
		if (g_vecWindows[index].pBufferImg)
		{
			delete g_vecWindows[index].pBufferImg;
			g_vecWindows[index].pBufferImg = nullptr;
		}

		// �ͷ���Ϣ�б��ڴ�
		std::vector<ExMessage>().swap(g_vecWindows[index].vecMessage);

		//DestroyWindow(g_vecWindows[index].hWnd);
		//PostQuitMessage(0);
	}

	// �˺��������ڲ����ã�������������ǹرմ��ڡ��ͷ��ڴ�
	// ��Ҫ���˺��������� WndProc �߳��е��ã������޷��رմ���
	void closegraph_win32(int index)
	{
		if (!IsAliveWindow(index))
		{
			return;
		}

		// �����ô����������ٱ�ʶæµ���ȴ��������
		g_vecWindows[index].isAlive = false;
		g_vecWindows[index].isBusyProcessing = true;
		WaitForTask(g_vecWindows[index].hWnd);

		// �������ø�����Ϊģ̬���ڣ�����Ҫ�������ڻָ�����
		if (g_vecWindows[index].hParent != nullptr)
		{
			EnableWindow(g_vecWindows[index].hParent, true);
			SetForegroundWindow(g_vecWindows[index].hParent);
		}

		// ж������
		DeleteTray(g_vecWindows[index].hWnd);

		// �������ڱ����٣�����Ҫ���û��������
		if (index == g_nFocusWindowIndex)
		{
			g_nFocusWindowIndex = NO_WINDOW_INDEX;
		}

		// �ͷŴ����ڴ�
		FreeWindow(index);

		// �ر�æµ��ʶ
		g_vecWindows[index].isBusyProcessing = false;

		// ����رմ˴��ں󲻴����κδ���
		if (!IsAnyWindow())
		{
			// �ر� GDI+ ��ͼ����
			Gdiplus_Shutdown();
		}
	}

	// �˺��������ⲿ���ã�ֻ����Ŀ�괰���̷߳��͹رմ�����Ϣ
	void closegraph_win32(HWND hWnd)
	{
		// �ر�ȫ��
		if (hWnd == nullptr)
		{
			for (int i = 0; i < (int)g_vecWindows.size(); i++)
			{
				if (g_vecWindows[i].isAlive)
				{
					// ���뽻��ԭ�߳� DestroyWindow
					// ���� WM_DESTROY ʱ������ wParam Ϊ 1����ʾ�����������ٴ���
					SendMessage(g_vecWindows[i].hWnd, WM_DESTROY, 1, 0);
				}
			}
		}
		else if (IsAliveWindow(hWnd))
		{
			SendMessage(hWnd, WM_DESTROY, 1, 0);
		}
	}

	void SetWndProcFunc(HWND hWnd, WNDPROC WindowProcess)
	{
		int index = GetWindowIndex(hWnd);
		if (IsAliveWindow(index))
		{
			g_vecWindows[index].funcWndProc = WindowProcess;
		}
	}

	IMAGE* GetWindowImage(HWND hWnd)
	{
		int index = GetWindowIndex(hWnd);
		if (IsAliveWindow(index))
		{
			return g_vecWindows[index].pBufferImg;
		}
		else
		{
			return nullptr;
		}
	}

	Canvas* GetWindowCanvas(HWND hWnd)
	{
		int index = GetWindowIndex(hWnd);
		if (IsAliveWindow(index))
		{
			return g_vecWindows[index].pBufferImgCanvas;
		}
		else
		{
			return nullptr;
		}
	}

	void BindWindowCanvas(Canvas* pCanvas, HWND hWnd)
	{
		int index = GetWindowIndex(hWnd);
		if (IsAliveWindow(index))
		{
			g_vecWindows[index].pBufferImgCanvas = pCanvas;
			pCanvas->BindToWindow(g_vecWindows[index].hWnd, g_vecWindows[index].pBufferImg);
		}
	}

	void init_end(HWND hWnd)
	{
		if (hWnd)
		{
			int index = GetWindowIndex(hWnd);
			while (IsAliveWindow(index))
				Sleep(100);
		}
		else
			while (IsAnyWindow())
				Sleep(100);
	}

	void AutoExit()
	{
		std::thread([]() {
			init_end();
			exit(0);
			}).detach();
	}

	HWND GetHWnd_win32()
	{
		return IsFocusWindowExisted() ? GetFocusWindow().hWnd : nullptr;
	}

	EasyWindow GetWorkingWindow()
	{
		return GetFocusWindow();
	}

	bool SetWorkingWindow(HWND hWnd)
	{
		if (!hWnd || GetFocusWindow().hWnd == hWnd)
		{
			if (GetWorkingImage() != GetFocusWindow().pBufferImg)
			{
				SetWorkingImage(GetFocusWindow().pBufferImg);
			}
			return true;
		}

		int index = GetWindowIndex(hWnd);
		if (IsAliveWindow(index))
		{
			WaitForTask();
			WaitForProcessing(index);
			g_nFocusWindowIndex = index;

			SetWorkingImage(GetFocusWindow().pBufferImg);
			return true;
		}
		else
		{
			return false;
		}
	}

	void QuickDraw(UINT nSkipPixels, HWND hWnd)
	{
		int index = GetWindowIndex(hWnd);
		if (IsAliveWindow(index))
			g_vecWindows[index].nSkipPixels = nSkipPixels;
	}

	DrawMode GetDrawMode()
	{
		return g_fDrawMode;
	}

	void SetDrawMode(DrawMode mode)
	{
		g_fDrawMode = mode;
	}

	// �ڲ�������ֱ�ӷ����û��ػ���Ϣ
	void SendUserRedrawMsg(HWND hWnd)
	{
		SendMessage(hWnd, WM_USER_REDRAW, 0, 0);
	}

	void RedrawWindow(HWND hWnd)
	{
		if (!hWnd)
			hWnd = GetFocusWindow().hWnd;

		switch (g_fDrawMode)
		{
		case DM_Real:
			SendUserRedrawMsg(hWnd);
			break;

		case DM_Normal:
			// ���̫����
			//InvalidateRect(hWnd, nullptr, false);
			SendUserRedrawMsg(hWnd);
			break;

		case DM_Fast:
			if (!(clock() % 2))
				SendUserRedrawMsg(hWnd);
			break;

		case DM_VeryFast:
			if (!(clock() % 5))
				SendUserRedrawMsg(hWnd);
			break;

		case DM_Fastest:
			if (!(clock() % 9))
				SendUserRedrawMsg(hWnd);
			break;
		}
	}

	// ���´��ڻ�����˫����
	// rct �����������궼Ϊ 0 ��ʾȫ������
	void FlushDrawing(int index, RECT rct = { 0 })
	{
		if (!IsAliveWindow(index))
		{
			return;
		}

		int w = g_vecWindows[index].pImg->getwidth();
		int h = g_vecWindows[index].pImg->getheight();

		// �Ƿ�ȫ������
		bool isAllFlush = !(rct.left && rct.top && rct.right && rct.bottom);

		// ˫��������㻭��
		DWORD* dst = GetImageBuffer(g_vecWindows[index].pImg);
		DWORD* src = GetImageBuffer(g_vecWindows[index].pBufferImg);

		// �����ػ�ʱ�������ػ�����
		RECT rctCorrected = rct;
		if (!isAllFlush)
		{
			if (rct.left < 0)		rctCorrected.left = 0;
			if (rct.top < 0)		rctCorrected.top = 0;
			if (rct.right > w)		rctCorrected.right = w;
			if (rct.bottom > h)		rctCorrected.bottom = h;
		}

		// ���������ص�ģʽ
		if (g_vecWindows[index].nSkipPixels == 0)
		{
			// ȫ������
			if (isAllFlush)
			{
				// fastest
				memcpy(dst, src, sizeof(DWORD) * w * h);
			}
			// ���ָ���
			else
			{
				for (int x = rctCorrected.left; x < rctCorrected.right; x++)
				{
					for (int y = rctCorrected.top; y < rctCorrected.bottom; y++)
					{
						int index = x + y * w;
						dst[index] = src[index];
					}
				}
			}
		}
		// �������ص�ģʽ
		else
		{
			// ȫ������
			if (isAllFlush)
			{
				int len = w * h;
				for (int i = 0; i < len; i++)		// ���Ա�������
				{
					if (dst[i] == src[i])			// ��������ĳλ��ɫ���ص����������������� n �����ص�
					{
						i += g_vecWindows[index].nSkipPixels;
						continue;
					}
					dst[i] = src[i];
				}
			}
			// ���ָ���
			else
			{
				for (int y = rctCorrected.top; y < rctCorrected.bottom; y++)	// �ھ��������ڱ�������
				{
					for (int x = rctCorrected.left; x < rctCorrected.right; x++)
					{
						int index = x + y * w;
						if (dst[index] == src[index])	// ��������ĳλ��ɫ���ص������� x ������������������ n �����ص�
						{
							x += g_vecWindows[index].nSkipPixels;
							continue;
						}
						dst[index] = src[index];
					}
				}
			}
		}
	}// FlushDrawing

	// �ṩ���û��Ľӿ�
	void FlushDrawing(RECT rct)
	{
		// Ϊ�˷�ֹ�û�����˫����ʱ�������쵼�»�����ͻ�������ڴ��������ڵ��ô˺���
		if (IsInTask())
		{
			FlushDrawing(g_nFocusWindowIndex, rct);
		}
	}

	void EnableAutoFlush(bool enable)
	{
		g_bAutoFlush = enable;
	}

	bool BeginTask()
	{
		// ��������ƥ���жϣ�ֻ�����Ƿ���������
		if (!g_isInTask && IsFocusWindowExisted())
		{
			WaitForProcessing(g_nFocusWindowIndex);
			g_isInTask = true;
		}
		return g_isInTask;
	}

	void EndTask(bool flush)
	{
		if (g_isInTask)
		{
			if (flush && IsFocusWindowExisted())
			{
				GetFocusWindow().isNeedFlush = true;
				//FlushDrawing(g_nFocusWindowIndex);
			}

			g_isInTask = false;
		}
	}

	bool IsInTask(HWND hWnd)
	{
		return g_isInTask && (hWnd ? GetFocusWindow().hWnd == hWnd : true);
	}

	// ���µ������ڻ�����С
	void ResizeWindowImage(int index, RECT rct)
	{
		if (IsAliveWindow(index))
		{
			g_vecWindows[index].pImg->Resize(rct.right, rct.bottom);
			g_vecWindows[index].pBufferImg->Resize(rct.right, rct.bottom);
			g_vecWindows[index].isNewSize = true;
		}
	}

	void ShowTray(NOTIFYICONDATA* nid)
	{
		Shell_NotifyIcon(NIM_ADD, nid);
	}

	void CreateTray(LPCTSTR lpszTrayName, HWND hWnd)
	{
		static int id = 0;

		int index = GetWindowIndex(hWnd);
		if (IsAliveWindow(index))
		{
			HICON hIcon = g_hIconDefault;
			if (g_lpszCustomIconSm)
				hIcon = g_hCustomIconSm;
			else if (g_lpszCustomIcon)
				hIcon = g_hCustomIcon;

			g_vecWindows[index].isUseTray = true;
			g_vecWindows[index].nid.cbSize = sizeof(NOTIFYICONDATA);
			g_vecWindows[index].nid.hWnd = g_vecWindows[index].hWnd;
			g_vecWindows[index].nid.uID = id++;
			g_vecWindows[index].nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
			g_vecWindows[index].nid.uCallbackMessage = WM_TRAY;
			g_vecWindows[index].nid.hIcon = hIcon;
			lstrcpy(g_vecWindows[index].nid.szTip, lpszTrayName);
			ShowTray(&g_vecWindows[index].nid);
		}
	}

	void DeleteTray(HWND hWnd)
	{
		int index = GetWindowIndex(hWnd);

		// ������ɾ��ʱ����øú��������Բ��жϴ������ֻ��Ҫ�жϴ����Ƿ����
		if (IsValidWindowIndex(index))
		{
			if (g_vecWindows[index].isUseTray)
			{
				g_vecWindows[index].isUseTray = false;
				Shell_NotifyIcon(NIM_DELETE, &g_vecWindows[index].nid);
			}
		}
	}

	void SetTrayMenu(HMENU hMenu, HWND hWnd)
	{
		int index = GetWindowIndex(hWnd);
		if (IsAliveWindow(index))
		{
			g_vecWindows[index].isUseTrayMenu = true;
			g_vecWindows[index].hTrayMenu = hMenu;
		}
	}

	void SetTrayMenuProcFunc(void(*pFunc)(UINT), HWND hWnd)
	{
		int index = GetWindowIndex(hWnd);
		if (IsAliveWindow(index))
		{
			g_vecWindows[index].funcTrayMenuProc = pFunc;
		}
	}

	bool IsWindowSizeChanged(HWND hWnd)
	{
		int index = GetWindowIndex(hWnd);
		if (IsValidWindowIndex(index))
		{
			bool b = g_vecWindows[index].isNewSize;
			g_vecWindows[index].isNewSize = false;
			return b;
		}
		else
		{
			return false;
		}
	}

	bool GetCustomIconState()
	{
		return g_lpszCustomIcon;
	}

	void SetCustomIcon(LPCTSTR lpszIcon, LPCTSTR lpszIconSm)
	{
		g_lpszCustomIcon = lpszIcon;
		g_lpszCustomIconSm = lpszIconSm;
		g_hCustomIcon = LoadIcon(g_hInstance, lpszIcon);
		g_hCustomIconSm = LoadIcon(g_hInstance, lpszIconSm);
	}

	// ��ȡ��Ϣ����
	std::vector<ExMessage>& GetMsgVector(HWND hWnd)
	{
		static std::vector<ExMessage> vec;
		int index = GetWindowIndex(hWnd);
		if (IsAliveWindow(index))
		{
			return g_vecWindows[index].vecMessage;
		}
		else
		{
			vec.clear();
			return vec;
		}
	}

	// �Ƴ���ǰ��Ϣ
	void RemoveMessage(HWND hWnd)
	{
		if (GetMsgVector(hWnd).size())
		{
			GetMsgVector(hWnd).erase(GetMsgVector(hWnd).begin());
		}
	}

	// �����Ϣ
	// ֧�ֻ����Ϣ����
	void ClearMessage(BYTE filter, HWND hWnd)
	{
		for (size_t i = 0; i < GetMsgVector(hWnd).size(); i++)
			if (filter & GetExMessageType(GetMsgVector(hWnd)[i]))
				GetMsgVector(hWnd).erase(GetMsgVector(hWnd).begin() + i--);
	}

	// �Ƿ�������Ϣ
	// ֧�ֻ����Ϣ����
	bool IsNewMessage(BYTE filter, HWND hWnd)
	{
		for (auto& element : GetMsgVector(hWnd))
			if (filter & GetExMessageType(element))
				return true;
		return false;
	}

	// �����Ϣ��ֱ����ȡ���������͵���Ϣ
	// ֧�ֻ����Ϣ����
	ExMessage GetNextMessage(BYTE filter, HWND hWnd)
	{
		if (IsNewMessage(filter, hWnd))
		{
			for (size_t i = 0; i < GetMsgVector(hWnd).size(); i++)
			{
				if (filter & GetExMessageType(GetMsgVector(hWnd)[i]))
				{
					for (size_t j = 0; j < i; j++)
					{
						RemoveMessage(hWnd);
					}
					return GetMsgVector(hWnd)[0];
				}
			}
		}
		return {};
	}

	ExMessage getmessage_win32(BYTE filter, HWND hWnd)
	{
		while (!IsNewMessage(filter, hWnd))	HpSleep(1);
		ExMessage msg = GetNextMessage(filter, hWnd);
		RemoveMessage(hWnd);
		return msg;
	}

	void getmessage_win32(ExMessage* msg, BYTE filter, HWND hWnd)
	{
		ExMessage msgEx = getmessage_win32(filter, hWnd);
		if (msg)	*msg = msgEx;
	}

	bool peekmessage_win32(ExMessage* msg, BYTE filter, bool removemsg, HWND hWnd)
	{
		if (IsNewMessage(filter, hWnd))
		{
			if (msg)		*msg = GetNextMessage(filter, hWnd);
			if (removemsg)	RemoveMessage(hWnd);
			return true;
		}
		return false;
	}

	void flushmessage_win32(BYTE filter, HWND hWnd)
	{
		ClearMessage(filter, hWnd);
	}

	bool MouseHit_win32(HWND hWnd)
	{
		return IsNewMessage(EM_MOUSE, hWnd);
	}

	MOUSEMSG GetMouseMsg_win32(HWND hWnd)
	{
		ExMessage msgEx = getmessage_win32(EM_MOUSE, hWnd);
		return To_MouseMsg(msgEx);
	}

	bool PeekMouseMsg_win32(MOUSEMSG* pMsg, bool bRemoveMsg, HWND hWnd)
	{
		ExMessage msgEx;
		bool r = peekmessage_win32(&msgEx, EM_MOUSE, bRemoveMsg, hWnd);
		*pMsg = To_MouseMsg(msgEx);
		return r;
	}

	void FlushMouseMsgBuffer_win32(HWND hWnd)
	{
		ClearMessage(EM_MOUSE, hWnd);
	}

	ExMessage To_ExMessage(MOUSEMSG msg)
	{
		ExMessage msgEx = {};
		msgEx.message = msg.uMsg;
		msgEx.ctrl = msg.mkCtrl;
		msgEx.shift = msg.mkShift;
		msgEx.lbutton = msg.mkLButton;
		msgEx.mbutton = msg.mkMButton;
		msgEx.rbutton = msg.mkRButton;
		msgEx.x = msg.x;
		msgEx.y = msg.y;
		msgEx.wheel = msg.wheel;
		return msgEx;
	}

	MOUSEMSG To_MouseMsg(ExMessage msgEx)
	{
		MOUSEMSG msg = {};
		if (GetExMessageType(msgEx) == EM_MOUSE)
		{
			msg.uMsg = msgEx.message;
			msg.mkCtrl = msgEx.ctrl;
			msg.mkShift = msgEx.shift;
			msg.mkLButton = msgEx.lbutton;
			msg.mkMButton = msgEx.mbutton;
			msg.mkRButton = msgEx.rbutton;
			msg.x = msgEx.x;
			msg.y = msgEx.y;
			msg.wheel = msgEx.wheel;
		}
		return msg;
	}

	void PreSetWindowStyle(long lStyle)
	{
		g_isPreStyle = true;
		g_lPreStyle = lStyle;
	}

	void PreSetWindowStyleEx(long lStyleEx)
	{
		g_isPreStyleEx = true;
		g_lPreStyleEx = lStyleEx;
	}

	void PreSetWindowPos(int x, int y)
	{
		g_isPrePos = true;
		g_pPrePos = { x,y };
	}

	void PreSetWindowShowState(int nCmdShow)
	{
		g_isPreShowState = true;
		g_nPreCmdShow = nCmdShow;
	}

	int SetWindowStyle(long lNewStyle, HWND hWnd)
	{
		if (hWnd == nullptr)	hWnd = GetFocusWindow().hWnd;
		return SetWindowLong(hWnd, GWL_STYLE, lNewStyle);
	}

	int SetWindowExStyle(long lNewExStyle, HWND hWnd)
	{
		if (hWnd == nullptr)	hWnd = GetFocusWindow().hWnd;
		return SetWindowLong(hWnd, GWL_EXSTYLE, lNewExStyle);
	}

	POINT GetWindowPos(HWND hWnd)
	{
		if (!hWnd)	hWnd = GetFocusWindow().hWnd;
		RECT rct;
		GetWindowRect(hWnd, &rct);
		return { rct.left, rct.top };
	}

	SIZE GetWindowSize(HWND hWnd)
	{
		if (!hWnd)	hWnd = GetFocusWindow().hWnd;
		RECT rct;
		GetWindowRect(hWnd, &rct);
		return { rct.right - rct.left, rct.bottom - rct.top };
	}

	void MoveWindow(int x, int y, HWND hWnd)
	{
		if (!hWnd)	hWnd = GetFocusWindow().hWnd;
		SetWindowPos(hWnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE);
	}

	void MoveWindowRel(int dx, int dy, HWND hWnd)
	{
		if (!hWnd)	hWnd = GetFocusWindow().hWnd;
		POINT pos = GetWindowPos(hWnd);
		SetWindowPos(hWnd, HWND_TOP, pos.x + dx, pos.y + dy, 0, 0, SWP_NOSIZE);
	}

	void ResizeWindow(int w, int h, HWND hWnd)
	{
		if (!hWnd)	hWnd = GetFocusWindow().hWnd;
		SetWindowPos(hWnd, HWND_TOP, 0, 0, w, h, SWP_NOMOVE);
	}

	void SetWindowTitle(LPCTSTR lpszTitle, HWND hWnd)
	{
		if (!hWnd)	hWnd = GetFocusWindow().hWnd;
		SetWindowText(hWnd, lpszTitle);
	}

	// ��ȡĬ�ϴ���ͼ��
	HICON GetDefaultAppIcon()
	{
		static HBITMAP hBmp = Image2Bitmap(GetIconImage(), true);
		static HICON hIcon = Bitmap2Icon(hBmp);
		static bool init = false;
		if (!init)
		{
			DeleteObject(hBmp);
			init = true;
		}
		return hIcon;
	}

	void OnSize(int indexWnd)
	{
		RECT rctWnd;
		GetClientRect(g_vecWindows[indexWnd].hWnd, &rctWnd);

		WaitForProcessing(indexWnd);
		g_vecWindows[indexWnd].isBusyProcessing = true;		// ��������������
		WaitForTask(g_vecWindows[indexWnd].hWnd);			// �ȴ����һ���������

		ResizeWindowImage(indexWnd, rctWnd);
		if (g_vecWindows[indexWnd].pBufferImgCanvas)
		{
			g_vecWindows[indexWnd].pBufferImgCanvas->UpdateSizeInfo();
		}

		g_vecWindows[indexWnd].isBusyProcessing = false;
	}

	void OnTray(int indexWnd, LPARAM lParam)
	{
		if (g_vecWindows[indexWnd].isUseTray)
		{
			HWND hWnd = g_vecWindows[indexWnd].hWnd;
			POINT ptMouse;
			GetCursorPos(&ptMouse);

			switch (lParam)
			{
				// ��������
			case WM_LBUTTONDOWN:
				SetForegroundWindow(hWnd);
				break;

				// �Ҽ��򿪲˵�
			case WM_RBUTTONDOWN:
				if (g_vecWindows[indexWnd].isUseTrayMenu)
				{
					SetForegroundWindow(hWnd);	// ����һ�´��ڣ���ֹ�˵�����ʧ

					// ��ʾ�˵�������
					int nMenuId = TrackPopupMenu(g_vecWindows[indexWnd].hTrayMenu, TPM_RETURNCMD, ptMouse.x, ptMouse.y, 0, hWnd, nullptr);
					if (nMenuId == 0) PostMessage(hWnd, WM_LBUTTONDOWN, 0, 0);
					if (g_vecWindows[indexWnd].funcTrayMenuProc)
					{
						g_vecWindows[indexWnd].funcTrayMenuProc(nMenuId);
					}
				}
				break;

			default:
				break;
			}
		}
	}

	void OnTaskBarCreated(int indexWnd)
	{
		if (g_vecWindows[indexWnd].isUseTray)
		{
			ShowTray(&g_vecWindows[indexWnd].nid);
		}
	}

	// �Ǽ���Ϣ��ExMessage��
	void RegisterExMessage(int indexWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// ��¼��Ϣ�¼�
		switch (msg)
		{
			// EM_MOUSE
		case WM_MOUSEMOVE:
		case WM_MOUSEWHEEL:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
		{
			ExMessage msgMouse = {};
			msgMouse.message = msg;
			msgMouse.x = GET_X_LPARAM(lParam);
			msgMouse.y = GET_Y_LPARAM(lParam);
			msgMouse.wheel = GET_WHEEL_DELTA_WPARAM(wParam);
			msgMouse.shift = LOWORD(wParam) & 0x04 ? true : false;
			msgMouse.ctrl = LOWORD(wParam) & 0x08 ? true : false;
			msgMouse.lbutton = LOWORD(wParam) & 0x01 ? true : false;
			msgMouse.mbutton = LOWORD(wParam) & 0x10 ? true : false;
			msgMouse.rbutton = LOWORD(wParam) & 0x02 ? true : false;

			// �й�����Ϣʱ���õ�����������Ļ���꣬��Ҫת��
			if (msgMouse.wheel)
			{
				POINT p = { msgMouse.x ,msgMouse.y };
				ScreenToClient(g_vecWindows[indexWnd].hWnd, &p);
				msgMouse.x = (short)p.x;
				msgMouse.y = (short)p.y;
			}

			g_vecWindows[indexWnd].vecMessage.push_back(msgMouse);
		}
		break;

		// EM_KEY
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		{
			// code from MSDN
			WORD vkCode = LOWORD(wParam);                                 // virtual-key code
			WORD keyFlags = HIWORD(lParam);
			WORD scanCode = LOBYTE(keyFlags);                             // scan code
			BOOL isExtendedKey = (keyFlags & KF_EXTENDED) == KF_EXTENDED; // extended-key flag, 1 if scancode has 0xE0 prefix

			if (isExtendedKey)
				scanCode = MAKEWORD(scanCode, 0xE0);

			BOOL repeatFlag = (keyFlags & KF_REPEAT) == KF_REPEAT;        // previous key-state flag, 1 on autorepeat
			WORD repeatCount = LOWORD(lParam);                            // repeat count, > 0 if several keydown messages was combined into one message
			BOOL upFlag = (keyFlags & KF_UP) == KF_UP;                    // transition-state flag, 1 on keyup

			// ���ܼ�������������
			// if we want to distinguish these keys:
			//switch (vkCode)
			//{
			//case VK_SHIFT:   // converts to VK_LSHIFT or VK_RSHIFT
			//case VK_CONTROL: // converts to VK_LCONTROL or VK_RCONTROL
			//case VK_MENU:    // converts to VK_LMENU or VK_RMENU
			//	vkCode = LOWORD(MapVirtualKeyW(scanCode, MAPVK_VSC_TO_VK_EX));
			//	break;
			//}

			ExMessage msgKey = {};
			msgKey.message = msg;
			msgKey.vkcode = (BYTE)vkCode;
			msgKey.scancode = (BYTE)scanCode;
			msgKey.extended = isExtendedKey;
			msgKey.prevdown = repeatFlag;

			g_vecWindows[indexWnd].vecMessage.push_back(msgKey);

			// ������̨��һ�ݣ�֧�� _getch() ϵ�к���
			PostMessage(g_hConsole, msg, wParam, lParam);
		}
		break;

		// EM_CHAR
		case WM_CHAR:
		{
			ExMessage msgChar = {};
			msgChar.message = msg;
			msgChar.ch = (TCHAR)wParam;
			g_vecWindows[indexWnd].vecMessage.push_back(msgChar);

			// ֪ͨ����̨
			PostMessage(g_hConsole, msg, wParam, lParam);
		}
		break;

		// EM_WINDOW
		case WM_ACTIVATE:
		case WM_MOVE:
		case WM_SIZE:
		{
			ExMessage msgWindow = {};
			msgWindow.message = msg;
			msgWindow.wParam = wParam;
			msgWindow.lParam = lParam;
			g_vecWindows[indexWnd].vecMessage.push_back(msgWindow);
		}
		break;
		}
	}

	// �����û�����
	void OnPaint(int indexWnd, HDC hdc)
	{
		// �ڿ����Զ�ˢ��˫���������£�����˫�����ˢ������
		if (g_bAutoFlush && g_vecWindows[indexWnd].isNeedFlush)
		{
			WaitForProcessing(indexWnd);
			g_vecWindows[indexWnd].isBusyProcessing = true;		// ��������������
			WaitForTask(g_vecWindows[indexWnd].hWnd);			// �ȴ����һ���������

			// ����˫����
			FlushDrawing(indexWnd);
			g_vecWindows[indexWnd].isNeedFlush = false;

			g_vecWindows[indexWnd].isBusyProcessing = false;
		}

		// ����ͼ������������� HDC
		RECT rctWnd;
		GetClientRect(g_vecWindows[indexWnd].hWnd, &rctWnd);
		CopyImageToHDC(g_vecWindows[indexWnd].pImg, hdc, rctWnd);
	}

	void OnMove(HWND hWnd)
	{
		//RECT rctWnd;
		//GetWindowRect(hWnd, &rctWnd);

		//// �ƶ����ڳ�����Ļʱ���ܵ����Ӵ�����ʾ�����⣬���Դ�ʱ��Ҫ�����ػ�
		//// ����û�����һֱ��ǿ���ػ棬��˲������ࡣ
		//if (rctWnd.left < g_screenSize.left || rctWnd.top < g_screenSize.top
		//	|| rctWnd.right > g_screenSize.left + g_screenSize.w
		//	|| rctWnd.bottom > g_screenSize.top + g_screenSize.h)
		//{
		//	EnforceRedraw(hWnd);
		//}
	}

	void OnDestroy(int indexWnd, WPARAM wParam)
	{
		closegraph_win32(indexWnd);

		// ���ڲ�������ζ�������û����� closegraph_win32 ���ٴ���
		// ���ٵ��� DestroyWindow
		if (wParam)
		{
			DestroyWindow(g_vecWindows[indexWnd].hWnd);
		}
	}

	HWND OnSysCtrlCreate(int indexWnd, WPARAM wParam, LPARAM lParam)
	{
		CREATESTRUCT* c = (CREATESTRUCT*)lParam;
		HWND hWnd = CreateWindow(
			c->lpszClass,
			c->lpszName,
			c->style,
			c->x, c->y,
			c->cx, c->cy,
			c->hwndParent,
			c->hMenu,
			GetModuleHandle(0),
			c->lpCreateParams
		);

		// ��¼
		g_vecWindows[indexWnd].vecSysCtrl.push_back((SysControlBase*)wParam);
		return hWnd;
	}

	// ����ϵͳ�ؼ���Ϣ
	// bRet ����������Ƿ�ֱ�ӷ���
	LRESULT SysCtrlProc(int indexWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool& bRet)
	{
		switch (msg)
		{
			// ����ϵͳ�ؼ�
		case WM_SYSCTRL_CREATE:
		{
			g_vecWindows[indexWnd].bHasCtrl = true;
			bRet = true;
			return (LRESULT)OnSysCtrlCreate(indexWnd, wParam, lParam);
			break;
		}

		// ����ϵͳ�ؼ�
		case WM_SYSCTRL_DELETE:
		{
			// �������Ŀؼ�ָ����Ϊ��
			for (size_t i = 0; i < g_vecWindows[indexWnd].vecSysCtrl.size(); i++)
			{
				if (g_vecWindows[indexWnd].vecSysCtrl[i] == (SysControlBase*)wParam)
				{
					g_vecWindows[indexWnd].vecSysCtrl[i] = nullptr;
				}
			}

			bRet = true;
			return 0;
			break;
		}
		}

		// ���ڿؼ�ʱ���ɷ���Ϣ
		if (g_vecWindows[indexWnd].bHasCtrl)
		{
			bool bCtrlRet = false;
			LRESULT lr = 0;
			for (auto& pCtrl : g_vecWindows[indexWnd].vecSysCtrl)
			{
				if (pCtrl)
				{
					LRESULT lr = pCtrl->UpdateMessage(msg, wParam, lParam, bCtrlRet);
					if (bCtrlRet)
					{
						bRet = true;
						return lr;
					}
				}
			}
		}

		bRet = false;
		return 0;
	}

	void OnCreate(int indexWnd, HWND hWnd, LPARAM lParam)
	{
	}

	// ���ڹ��̺���
	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// ���ھ�����Ϣ
		LRESULT resultUserProc = HIWINDOW_DEFAULT_PROC;		// ��¼�û����ڹ��̺�������ֵ
		int indexWnd = GetWindowIndex(hWnd);				// �ô������Ѽ�¼�б��е�����

		// ���ô��ڲ��ڴ����б��ڣ���ʹ��Ĭ�Ϸ������д��������鴰�����
		if (!IsValidWindowIndex(indexWnd))
		{
			// Ҳ�п������ڽ��� WM_CREATE ��Ϣ����ʱ���ڻ�δ�����б�������û����̺���
			if (msg == WM_CREATE)
			{
				// ��ʱ��Ҫ���� index
				int indexReal = (int)g_vecWindows.size() - 1;
				OnCreate(indexReal, hWnd, lParam);
				WNDPROC proc = g_vecWindows[indexReal].funcWndProc;
				if (proc)
				{
					proc(hWnd, msg, wParam, lParam);
				}
			}

			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		//** ��ʼ��������Ϣ **//

		// Ԥ�ȴ�������Ϣ
		switch (msg)
		{
		case WM_SIZE:
			OnSize(indexWnd);
			break;

			// ������Ϣ
		case WM_TRAY:
			OnTray(indexWnd, lParam);
			break;

		default:
			// ϵͳ���������´�������ʱ������Ҫ���´�������
			if (msg == g_uWM_TASKBARCREATED)
			{
				OnTaskBarCreated(indexWnd);
			}
			break;
		}

		// ��ڵ�һ���¼�����
		if (IsAliveWindow(indexWnd))
		{
			// �Ǽ���Ϣ
			RegisterExMessage(indexWnd, msg, wParam, lParam);

			// ����ϵͳ�ؼ���Ϣ
			bool bRetSysCtrl = false;
			LRESULT lrSysCtrl = SysCtrlProc(indexWnd, msg, wParam, lParam, bRetSysCtrl);
			if (bRetSysCtrl)
				return lrSysCtrl;
		}

		// �����û���Ϣ������
		if (g_vecWindows[indexWnd].funcWndProc)
		{
			resultUserProc = g_vecWindows[indexWnd].funcWndProc(hWnd, msg, wParam, lParam);
		}

		// �ƺ���
		switch (msg)
		{
			// �û��ػ���Ϣ��������ֱ�ӷ���
			// Ҳ�������ϵͳ�ػ淽��
			// ������Ϊ�����û�Ҳ�ܴ��������Ϣ
		case WM_USER_REDRAW:
		{
			HDC hdc = GetDC(hWnd);
			OnPaint(indexWnd, hdc);
			ReleaseDC(hWnd, hdc);
			return 0;
			break;
		}

		// ��Ϊ�û������ڹ��̺����л�ͼ��Ҫ����֮���������
		case WM_PAINT:
		{
			HDC			hdc;
			PAINTSTRUCT	ps;
			hdc = BeginPaint(hWnd, &ps);
			OnPaint(indexWnd, hdc);
			EndPaint(hWnd, &ps);

			// WM_PAINT ��Ϣ����Ҫ����ϵͳ���Ʒ���
			DefWindowProc(hWnd, WM_PAINT, 0, 0);
			break;
		}

		case WM_MOVE:
			OnMove(hWnd);
			break;

			// �رմ��ڣ��ͷ��ڴ�
		case WM_DESTROY:
			OnDestroy(indexWnd, wParam);
			break;
		}

		// ����ֵ
		LRESULT lResult = 0;

		// �˴�ͳһ�ں���ĩβ����

		// �û�δ�������Ϣ
		if (!g_vecWindows[indexWnd].funcWndProc || resultUserProc == HIWINDOW_DEFAULT_PROC)
		{
			switch (msg)
			{
			case WM_CLOSE:
				DestroyWindow(g_vecWindows[indexWnd].hWnd);
				break;

			case WM_DESTROY:
				PostQuitMessage(0);
				break;

				// WM_PAINT ��Ϣ�����ظ�����Ĭ�Ϸ���
			case WM_PAINT:
				break;

			default:
				lResult = DefWindowProc(hWnd, msg, wParam, lParam);
				break;
			}
		}

		// �û��Ѵ������Ϣ
		else
		{
			switch (msg)
			{
			case WM_CLOSE:
				break;

			case WM_DESTROY:
				break;
			}

			lResult = resultUserProc;
		}

		return lResult;
	}

	void RegisterWndClass()
	{
		HICON hIcon = g_hIconDefault;
		HICON hIconSm = g_hIconDefault;
		if (g_lpszCustomIcon)
			hIcon = g_hCustomIcon;
		if (g_lpszCustomIconSm)
			hIconSm = g_hCustomIconSm;

		g_WndClassEx.cbSize = sizeof(WNDCLASSEX);
		g_WndClassEx.style = CS_VREDRAW | CS_HREDRAW;
		g_WndClassEx.lpfnWndProc = WndProc;
		g_WndClassEx.cbClsExtra = 0;
		g_WndClassEx.cbWndExtra = 0;
		g_WndClassEx.hInstance = g_hInstance;
		g_WndClassEx.hIcon = hIcon;
		g_WndClassEx.hIconSm = hIconSm;
		g_WndClassEx.hCursor = LoadCursor(nullptr, IDC_ARROW);
		g_WndClassEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		g_WndClassEx.lpszMenuName = nullptr;
		g_WndClassEx.lpszClassName = g_lpszClassName;

		// ע�ᴰ����
		if (!RegisterClassEx(&g_WndClassEx))
		{
#ifdef UNICODE
			std::wstring str = std::to_wstring(GetLastError());
#else
			std::string str = std::to_string(GetLastError());
#endif
			MessageBox(nullptr, (_T("Error registing window class: ") + str).c_str(), _T("[Error]"), MB_OK | MB_ICONERROR);
			exit(-1);
		}
	}

	// ��ʼ�����ڽṹ��
	EasyWindow& InitWindowStruct(EasyWindow& wnd, HWND hParent, int w, int h, WNDPROC WindowProcess)
	{
		wnd.isAlive = true;
		wnd.hWnd = nullptr;
		wnd.hParent = hParent;
		wnd.pImg = new IMAGE(w, h);
		wnd.pBufferImg = new IMAGE(w, h);
		wnd.pBufferImgCanvas = nullptr;
		wnd.isNeedFlush = false;
		wnd.funcWndProc = WindowProcess;
		wnd.vecMessage.reserve(MSG_RESERVE_SIZE);
		wnd.isUseTray = false;
		wnd.nid = { 0 };
		wnd.isUseTrayMenu = false;
		wnd.hTrayMenu = nullptr;
		wnd.funcTrayMenuProc = nullptr;
		wnd.isNewSize = false;
		wnd.isBusyProcessing = false;
		wnd.nSkipPixels = 0;
		wnd.vecSysCtrl.reserve(SYSCTRL_RESERVE_SIZE);
		return wnd;
	}

	void InitRenderStartScene(HWND hWnd, int w, int h, int nPreCmdShow, bool& nStartAnimation)
	{
		RenderStartScene(hWnd, w, h, nPreCmdShow);
		nStartAnimation = true;
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}

	// �����������ڵĺ�����������
	void InitWindow(int w, int h, int flag, LPCTSTR lpszWndTitle, WNDPROC WindowProcess, HWND hParent, int* nDoneFlag, bool* nStartAnimation, HWND* hWnd)
	{
		static int nWndCount = 0;	// �Ѵ������ڼ������������ɴ��ڱ��⣩

#ifdef UNICODE
		std::wstring wstrTitle;		// ���ڱ���
#else
		std::string strTitle;		// ���ڱ���
#endif

		EasyWindow wnd;				// ������Ϣ
		int nFrameW, nFrameH;		// ���ڱ�������ߣ��������ڿ��ܲ�ͬ��
		int nIndexWnd = nWndCount;	// ��¼������ڵ� id

		// ���ܶ������ͬʱ�ڴ�����Ϊ�˷�ֹԤ�贰�����Խ��棬�ȱ������ݣ��ó�ȫ�ֱ���
		bool isPreStyle = g_isPreStyle;
		bool isPreStyleEx = g_isPreStyleEx;
		bool isPrePos = g_isPrePos;
		bool isPreShowState = g_isPreShowState;
		long lPreStyle = g_lPreStyle;
		long lPreStyleEx = g_lPreStyleEx;
		POINT pPrePos = g_pPrePos;
		int nPreCmdShow = g_nPreCmdShow;

		bool start_animation = false;

		g_isPreStyle = false;
		g_isPreStyleEx = false;
		g_isPrePos = false;
		g_isPreShowState = false;

		// δ���ñ���
		if (lstrlen(lpszWndTitle) == 0)
		{
#ifdef UNICODE
			wstrTitle = L"EasyX_" + (std::wstring)GetEasyXVer() + L" HiEasyX (" _HIEASYX_VER_STR_ + L")";
			if (nWndCount != 0)
			{
				wstrTitle += L" ( WindowID: " + std::to_wstring(nWndCount) + L" )";
			}
#else
			strTitle = "EasyX_" + (std::string)GetEasyXVer() + " HiEasyX (" _HIEASYX_VER_STR_ + ")";
			if (nWndCount != 0)
			{
				strTitle += " ( WindowID: " + std::to_string(nWndCount) + " )";
			}
#endif
		}
		else
		{
#ifdef UNICODE
			wstrTitle = lpszWndTitle;
#else
			strTitle = lpszWndTitle;
#endif
		}

		// ��һ�δ������� --- ��ʼ����������
		if (nWndCount == 0)
		{
			// ��ȡ�ֱ���
			g_screenSize = GetScreenSize();

			// Ĭ�ϳ���ͼ��
			g_hIconDefault = GetDefaultAppIcon();

			// ע�ᴰ����
			RegisterWndClass();
			g_hConsole = GetConsoleWindow();

			// ���ؿ���̨
			if (g_hConsole)
			{
				ShowWindow(g_hConsole, SW_HIDE);
			}

			// ��ȡϵͳ�������Զ������Ϣ����
			g_uWM_TASKBARCREATED = RegisterWindowMessage(TEXT("TaskbarCreated"));

#ifndef _DEBUG
#ifndef __DEBUG__
#ifndef DEBUG
#ifndef _NO_START_ANIMATION_

			if (!(isPreShowState && nPreCmdShow == SW_HIDE) && w >= 640 && h >= 480)
				start_animation = true;

#endif
#endif
#endif
#endif
		}

		// ������ڲ������κδ���
		if (!IsAnyWindow())
		{
			// ��ʼ�� GDI+ ��ͼ����
			Gdiplus_Try_Starup();
		}

		// ����̨
		if (g_hConsole && flag & EW_SHOWCONSOLE)
		{
			ShowWindow(g_hConsole, flag & SW_NORMAL);
		}

		// �û��ڴ�������ʱ���õĴ�������
		long user_style = WS_OVERLAPPEDWINDOW;
		if (flag & EW_NOMINIMIZE)	// �޳���С����ť
		{
			user_style &= ~WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX;
		}
		// �˷������У������洦�������
		/*if (flag & EW_NOCLOSE)
		{
			user_style &= ~WS_SYSMENU;
		}*/
		if (flag & EW_DBLCLKS)		// ֧��˫��
		{
			user_style |= CS_DBLCLKS;
		}

		// �ڴ�������ǰ�����ڼ���������Ԥ����Ϊ�գ�������̺������� WM_CREATE ��Ϣ
		InitWindowStruct(wnd, hParent, w, h, WindowProcess);
		g_vecWindows.push_back(wnd);

		// ��������
		for (int i = 0;; i++)
		{
			// ����ȷ��ʹ�õĴ�����ʽ
			long final_style = user_style;
			if (isPreStyle)
				final_style = lPreStyle;
			final_style |= WS_CLIPCHILDREN;	// ����������ʽ

			// ����ȷ��ʹ�õĴ�����չ��ʽ
			long final_style_ex = WS_EX_WINDOWEDGE;
			if (isPreStyleEx)
				final_style_ex = lPreStyleEx;

#ifdef UNICODE
			wnd.hWnd = CreateWindowEx(
				final_style_ex,
				g_lpszClassName,
				wstrTitle.c_str(),
				final_style,
				CW_USEDEFAULT, CW_USEDEFAULT,
				w, h,	// ��������������ã��Ժ��ȡ�߿��С���ٵ���
				hParent,
				nullptr,
				g_hInstance,
				nullptr
			);
#else
			wnd.hWnd = CreateWindowEx(
				final_style_ex,
				g_lpszClassName,
				strTitle.c_str(),
				final_style,
				CW_USEDEFAULT, CW_USEDEFAULT,
				w, h,	// ��������������ã��Ժ��ȡ�߿��С���ٵ���
				hParent,
				nullptr,
				g_hInstance,
				nullptr
			);
#endif

			if (wnd.hWnd)
			{
				// �������ڳɹ����ٽ������¼
				g_vecWindows[g_vecWindows.size() - 1].hWnd = wnd.hWnd;
				break;
			}

			// ���δ�������ʧ�ܣ����ٳ���
			else if (i == 2)
			{
#ifdef UNICODE
				std::wstring str = std::to_wstring(GetLastError());
#else
				std::string str = std::to_string(GetLastError());
#endif
				MessageBox(nullptr, (_T("Error creating window: ") + str).c_str(), _T("[Error]"), MB_OK | MB_ICONERROR);
				*nDoneFlag = -1;
				return;
			}
		}

		// �޳��رհ�ť
		if (flag & EW_NOCLOSE)
		{
			HMENU hmenu = GetSystemMenu(wnd.hWnd, false);
			RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
		}

		// ���ᴰ�ڽ���
		SetWorkingWindow(wnd.hWnd);

		*hWnd = wnd.hWnd;

		// ���ڴ������
		nWndCount++;

		// ע�⣺
		//	��������ʾ����ǰ����Ѿ���ɴ������ڡ�
		//	��Ϊ�������Զ�����̺����д����Ӵ��ڣ����ǲ�����ʾ����ǰ��Ǵ��ڴ�����ɣ�
		//	�ͻᵼ�¸����ڹ��̺�����������������ʾ���ھͻ����������������������ڼ�����
		*nDoneFlag = 1;
		if (!start_animation) *nStartAnimation = true;

		//** ��ʾ���ڵȺ������� **//

		// ��ȡ�߿��С�������ͼ����С
		RECT rcClient, rcWnd;
		GetClientRect(wnd.hWnd, &rcClient);
		GetWindowRect(wnd.hWnd, &rcWnd);
		nFrameW = (rcWnd.right - rcWnd.left) - rcClient.right;
		nFrameH = (rcWnd.bottom - rcWnd.top) - rcClient.bottom;

		int px = 0, py = 0;
		if (isPrePos)
		{
			px = pPrePos.x;
			py = pPrePos.y;
		}
		SetWindowPos(
			wnd.hWnd,
			HWND_TOP,
			px, py,
			w + nFrameW, h + nFrameH,
			isPrePos ? 0 : SWP_NOMOVE
		);

		if (!start_animation)
		{
			ShowWindow(wnd.hWnd, isPreShowState ? nPreCmdShow : SW_SHOWNORMAL);
			UpdateWindow(wnd.hWnd);
		}
		// ����ģʽ����Ⱦ��������
		if (start_animation == true)
		{
			// ��Ⱦ��������
			std::thread([&]() {
				InitRenderStartScene(wnd.hWnd, w, h, isPreShowState ? nPreCmdShow : SW_SHOWNORMAL, *nStartAnimation);
				}).detach();
		}

		// ��Ϣ�ɷ�������
		// �������ٺ���Զ��˳�
		MSG Msg;
		while (GetMessage(&Msg, 0, 0, 0) > 0)
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	}

	HWND initgraph_win32(int w, int h, int flag, LPCTSTR lpszWndTitle, WNDPROC WindowProcess, HWND hParent)
	{
		// ����Ƿ��Ѿ���ɴ��ڴ�������
		int nDoneFlag = 0;
		HWND hWnd = nullptr;

		bool nStartAnimation = false;

		// ���ڸ�����ʱ��ʵ��ģ̬����
		if (hParent)
		{
			// ���ø����ڣ��ô��ڱ����ٺ󣬸����ڽ���ָ�������
			EnableWindow(hParent, false);
		}

		std::thread(InitWindow, w, h, flag, lpszWndTitle, WindowProcess, hParent, &nDoneFlag, &nStartAnimation, &hWnd).detach();

		while (nDoneFlag == 0)	Sleep(50);		// �ȴ����ڴ������
		if (nDoneFlag == -1)
		{
			if (hParent)						// �����Ӵ���ʧ�ܣ���ʹ�����ڻָ�����
			{
				EnableWindow(hParent, true);
			}
			return nullptr;
		}
		else
		{
			while (nStartAnimation == false)	Sleep(50);		// �ȴ���ʼ�������
			// Ԥ�豳��ɫ
			if (SetWorkingWindow(hWnd) && BeginTask())
			{
				setbkcolor(CLASSICGRAY);
				settextcolor(BLACK);
				setlinecolor(BLACK);
				setfillcolor(BLACK);
				cleardevice();
				EndTask();
				RedrawWindow();
			}

			return hWnd;
		}
	}

	bool init_console()
	{
		if (GetConsoleWindow() == NULL) AllocConsole();
		if (GetConsoleWindow() != NULL)
		{
			ShowWindow(GetConsoleWindow(), SW_SHOW);
			return true;
		}
		return false;
	}

	bool hide_console()
	{
		if (GetConsoleWindow() != NULL)
		{
			ShowWindow(GetConsoleWindow(), SW_HIDE);
			return true;
		}
		return false;
	}

	bool close_console()
	{
		if (GetConsoleWindow() != NULL)
		{
			ShowWindow(GetConsoleWindow(), SW_HIDE);
			FreeConsole();
			return true;
		}
		return false;
	}

	Window::Window()
	{
	}

	Window::Window(int w, int h, int flag, LPCTSTR lpszWndTitle, WNDPROC WindowProcess, HWND hParent)
	{
		InitWindow(w, h, flag, lpszWndTitle, WindowProcess, hParent);
	}

	Window::~Window()
	{
	}

	HWND Window::InitWindow(int w, int h, int flag, LPCTSTR lpszWndTitle, WNDPROC WindowProcess, HWND hParent)
	{
		if (!m_isCreated)
		{
			// Ԥ�贰������
			if (m_isPreStyle)		PreSetWindowStyle(m_lPreStyle);
			if (m_isPreStyleEx)		PreSetWindowStyleEx(m_lPreStyleEx);
			if (m_isPrePos)			PreSetWindowPos(m_pPrePos.x, m_pPrePos.y);
			if (m_isPreShowState)	PreSetWindowShowState(m_nPreCmdShow);

			HWND hwnd = initgraph_win32(w, h, flag, lpszWndTitle, WindowProcess, hParent);
			int index = GetWindowIndex(hwnd);
			m_nWindowIndex = index;
			m_isCreated = true;
			return hwnd;
		}
		return nullptr;
	}

	HWND Window::Create(int w, int h, int flag, LPCTSTR lpszWndTitle, WNDPROC WindowProcess, HWND hParent)
	{
		return InitWindow(w, h, flag, lpszWndTitle, WindowProcess, hParent);
	}

	void Window::CloseWindow()
	{
		closegraph_win32(g_vecWindows[m_nWindowIndex].hWnd);
	}

	void Window::Destroy()
	{
		CloseWindow();
	}

	void Window::SetProcFunc(WNDPROC WindowProcess)
	{
		SetWndProcFunc(g_vecWindows[m_nWindowIndex].hWnd, WindowProcess);
	}

	HWND Window::GetHandle()
	{
		return g_vecWindows[m_nWindowIndex].hWnd;
	}

	EasyWindow Window::GetInfo()
	{
		return g_vecWindows[m_nWindowIndex];
	}

	bool Window::IsAlive()
	{
		return IsAliveWindow(m_nWindowIndex);
	}

	IMAGE* Window::GetImage()
	{
		return g_vecWindows[m_nWindowIndex].pBufferImg;
	}

	Canvas* Window::GetCanvas()
	{
		return g_vecWindows[m_nWindowIndex].pBufferImgCanvas;
	}

	void Window::BindCanvas(Canvas* pCanvas)
	{
		BindWindowCanvas(pCanvas, g_vecWindows[m_nWindowIndex].hWnd);
	}

	void Window::WaitMyTask()
	{
		WaitForTask(g_vecWindows[m_nWindowIndex].hWnd);
	}

	bool Window::SetWorkingWindow()
	{
		return HiEasyX::SetWorkingWindow(g_vecWindows[m_nWindowIndex].hWnd);
	}

	void Window::FlushDrawing(RECT rct)
	{
		if (IsInTask())
		{
			HiEasyX::FlushDrawing(rct);
		}
	}

	bool Window::BeginTask()
	{
		if (SetWorkingWindow())
		{
			return HiEasyX::BeginTask();
		}
		else
		{
			return false;
		}
	}

	void Window::EndTask(bool flush)
	{
		HiEasyX::EndTask(flush);
	}

	bool Window::IsInTask()
	{
		return HiEasyX::IsInTask(g_vecWindows[m_nWindowIndex].hWnd);
	}

	bool Window::IsSizeChanged()
	{
		return IsWindowSizeChanged(g_vecWindows[m_nWindowIndex].hWnd);
	}

	void Window::CreateTray(LPCTSTR lpszTrayName)
	{
		HiEasyX::CreateTray(lpszTrayName, g_vecWindows[m_nWindowIndex].hWnd);
	}

	void Window::DeleteTray()
	{
		HiEasyX::DeleteTray(g_vecWindows[m_nWindowIndex].hWnd);
	}

	void Window::SetTrayMenu(HMENU hMenu)
	{
		HiEasyX::SetTrayMenu(hMenu, g_vecWindows[m_nWindowIndex].hWnd);
	}

	void Window::SetTrayMenuProcFunc(void(*pFunc)(UINT))
	{
		HiEasyX::SetTrayMenuProcFunc(pFunc, g_vecWindows[m_nWindowIndex].hWnd);
	}

	void Window::PreSetStyle(long lStyle)
	{
		m_isPreStyle = true;
		m_lPreStyle = lStyle;
	}

	void Window::PreSetStyleEx(long lStyleEx)
	{
		m_isPreStyleEx = true;
		m_lPreStyleEx = lStyleEx;
	}

	void Window::PreSetPos(int x, int y)
	{
		m_isPrePos = true;
		m_pPrePos = { x,y };
	}

	void Window::PreSetShowState(int nCmdShow)
	{
		m_isPreShowState = true;
		m_nPreCmdShow = nCmdShow;
	}

	void Window::SetQuickDraw(UINT nSkipPixels)
	{
		QuickDraw(nSkipPixels, g_vecWindows[m_nWindowIndex].hWnd);
	}

	void Window::Redraw()
	{
		RedrawWindow(g_vecWindows[m_nWindowIndex].hWnd);
	}

	long Window::GetStyle()
	{
		return GetWindowStyle(g_vecWindows[m_nWindowIndex].hWnd);
	}

	int Window::SetStyle(long lNewStyle)
	{
		return SetWindowStyle(lNewStyle, g_vecWindows[m_nWindowIndex].hWnd);
	}

	long Window::GetExStyle()
	{
		return GetWindowExStyle(g_vecWindows[m_nWindowIndex].hWnd);
	}

	int Window::SetExStyle(long lNewExStyle)
	{
		return SetWindowExStyle(lNewExStyle, g_vecWindows[m_nWindowIndex].hWnd);
	}

	POINT Window::GetPos()
	{
		return GetWindowPos(g_vecWindows[m_nWindowIndex].hWnd);
	}

	SIZE Window::GetWindowSize()
	{
		return HiEasyX::GetWindowSize(g_vecWindows[m_nWindowIndex].hWnd);
	}

	void Window::Move(int x, int y)
	{
		MoveWindow(x, y, g_vecWindows[m_nWindowIndex].hWnd);
	}

	int Window::GetWindowWidth()
	{
		return GetWindowSize().cx;
	}

	int Window::GetWindowHeight()
	{
		return  GetWindowSize().cy;
	}

	void Window::MoveRel(int dx, int dy)
	{
		MoveWindowRel(dx, dy, g_vecWindows[m_nWindowIndex].hWnd);
	}

	void Window::Resize(int w, int h)
	{
		ResizeWindow(w, h, g_vecWindows[m_nWindowIndex].hWnd);
	}

	void Window::SetTransparent(bool enable, int alpha)
	{
		//���Խ��ۣ��˺��������� UpdateLayeredWindow һͬʹ��

		LONG nRet = ::GetWindowLong(g_vecWindows[m_nWindowIndex].hWnd, GWL_EXSTYLE);
		nRet |= WS_EX_LAYERED;
		::SetWindowLong(g_vecWindows[m_nWindowIndex].hWnd, GWL_EXSTYLE, nRet);

		if (!enable) alpha = 0xFF;
		SetLayeredWindowAttributes(g_vecWindows[m_nWindowIndex].hWnd, 0, alpha, LWA_ALPHA);
	}

	void Window::SetTitle(LPCTSTR lpszTitle)
	{
		SetWindowTitle(lpszTitle, g_vecWindows[m_nWindowIndex].hWnd);
	}

	bool Window::IsForegroundWindow()
	{
		return GetForegroundWindow() == g_vecWindows[m_nWindowIndex].hWnd;
	}

	int Window::GetClientWidth()
	{
		return g_vecWindows[m_nWindowIndex].pBufferImg->getwidth();
	}

	int Window::GetClientHeight()
	{
		return g_vecWindows[m_nWindowIndex].pBufferImg->getheight();
	}

	ExMessage Window::Get_Message(BYTE filter)
	{
		return getmessage_win32(filter, g_vecWindows[m_nWindowIndex].hWnd);
	}

	void Window::Get_Message(ExMessage* msg, BYTE filter)
	{
		return getmessage_win32(msg, filter, g_vecWindows[m_nWindowIndex].hWnd);
	}

	bool Window::Peek_Message(ExMessage* msg, BYTE filter, bool removemsg)
	{
		return peekmessage_win32(msg, filter, removemsg, g_vecWindows[m_nWindowIndex].hWnd);
	}

	void Window::Flush_Message(BYTE filter)
	{
		flushmessage_win32(filter, g_vecWindows[m_nWindowIndex].hWnd);
	}
}