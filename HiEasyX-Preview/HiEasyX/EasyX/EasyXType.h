#pragma once

#ifndef WINVER
#define WINVER 0x0500			// Specifies that the minimum required platform is Windows 95 and Windows NT 4.0.
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500		// Specifies that the minimum required platform is Windows 2000
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410	// Specifies that the minimum required platform is Windows 98
#endif

#ifndef __cplusplus
#error EasyX is only for C++
#endif

#include <windows.h>
#include <tchar.h>

#include <windef.h>

// EasyX Window Properties
#define EX_SHOWCONSOLE		1		// Maintain the console window when creating a graphics window
#define EX_NOCLOSE			2		// Disable the close button
#define EX_NOMINIMIZE		4		// Disable the minimize button
#define EX_DBLCLKS			8		// Support double-click events

// Color constant
#define	BLACK			0
#define	BLUE			0xAA0000
#define	GREEN			0x00AA00
#define	CYAN			0xAAAA00
#define	RED				0x0000AA
#define	MAGENTA			0xAA00AA
#define	BROWN			0x0055AA
#define	LIGHTGRAY		0xAAAAAA
#define	DARKGRAY		0x555555
#define	LIGHTBLUE		0xFF5555
#define	LIGHTGREEN		0x55FF55
#define	LIGHTCYAN		0xFFFF55
#define	LIGHTRED		0x5555FF
#define	LIGHTMAGENTA	0xFF55FF
#define	YELLOW			0x55FFFF
#define	WHITE			0xFFFFFF

// Color conversion macro
#define BGR(color)	( (((color) & 0xFF) << 16) | ((color) & 0xFF00FF00) | (((color) & 0xFF0000) >> 16) )

class IMAGE;

// Line style class
class LINESTYLE
{
public:
	LINESTYLE();
	LINESTYLE(const LINESTYLE& style);
	LINESTYLE& operator = (const LINESTYLE& style);
	virtual ~LINESTYLE();

	DWORD	style;
	DWORD	thickness;
	DWORD* puserstyle;
	DWORD	userstylecount;
};

// Fill style class
class FILLSTYLE
{
public:
	FILLSTYLE();
	FILLSTYLE(const FILLSTYLE& style);
	FILLSTYLE& operator = (const FILLSTYLE& style);
	virtual ~FILLSTYLE();

	int			style;				// Fill style
	long		hatch;				// Hatch pattern
	IMAGE* ppattern;			// Fill image
};

// Image class
class IMAGE
{
public:
	int getwidth() const;			// Get the width of the image
	int getheight() const;			// Get the height of the image

private:
	int			width, height;		// Width and height of the image
	HBITMAP		m_hBmp;
	HDC			m_hMemDC;
	float		m_data[6];
	COLORREF	m_LineColor;		// Current line color
	COLORREF	m_FillColor;		// Current fill color
	COLORREF	m_TextColor;		// Current text color
	COLORREF	m_BkColor;			// Current background color
	DWORD* m_pBuffer;			// Memory buffer of the image

	LINESTYLE	m_LineStyle;		// Current line style
	FILLSTYLE	m_FillStyle;		// Current fill style

	virtual void SetDefault();		// Set the graphics environment as default

public:
	IMAGE(int _width = 0, int _height = 0);
	IMAGE(const IMAGE& img);
	IMAGE& operator = (const IMAGE& img);
	virtual ~IMAGE();
	virtual void Resize(int _width, int _height);			// Resize image
};

// Message Category
#define EX_MOUSE	1
#define EX_KEY		2
#define EX_CHAR		4
#define EX_WINDOW	8

// Message Structure
struct ExMessage
{
	USHORT message;					// The message identifier
	union
	{
		// Data of the mouse message
		struct
		{
			bool ctrl : 1;		// Indicates whether the CTRL key is pressed
			bool shift : 1;		// Indicates whether the SHIFT key is pressed
			bool lbutton : 1;		// Indicates whether the left mouse button is pressed
			bool mbutton : 1;		// Indicates whether the middle mouse button is pressed
			bool rbutton : 1;		// Indicates whether the right mouse button is pressed
			short x;				// The x-coordinate of the cursor
			short y;				// The y-coordinate of the cursor
			short wheel;			// The distance the wheel is rotated, expressed in multiples or divisions of 120
		};

		// Data of the key message
		struct
		{
			BYTE vkcode;			// The virtual-key code of the key
			BYTE scancode;			// The scan code of the key. The value depends on the OEM
			bool extended : 1;		// Indicates whether the key is an extended key, such as a function key or a key on the numeric keypad. The value is true if the key is an extended key; otherwise, it is false.
			bool prevdown : 1;		// Indicates whether the key is previously up or down
		};

		// Data of the char message
		TCHAR ch;

		// Data of the window message
		struct
		{
			WPARAM wParam;
			LPARAM lParam;
		};
	};
};

// Old Window Properties
#define SHOWCONSOLE		1		// Maintain the console window when creating a graphics window
#define NOCLOSE			2		// Disable the close button
#define NOMINIMIZE		4		// Disable the minimize button
#define EW_SHOWCONSOLE	1		// Maintain the console window when creating a graphics window
#define EW_NOCLOSE		2		// Disable the close button
#define EW_NOMINIMIZE	4		// Disable the minimize button
#define EW_DBLCLKS		8		// Support double-click events

// Old fill styles
#define	NULL_FILL			BS_NULL
#define	EMPTY_FILL			BS_NULL
#define	SOLID_FILL			BS_SOLID
// Old normal fill style
#define	BDIAGONAL_FILL		BS_HATCHED, HS_BDIAGONAL					// Fill with ///.
#define CROSS_FILL			BS_HATCHED, HS_CROSS						// Fill with +++.
#define DIAGCROSS_FILL		BS_HATCHED, HS_DIAGCROSS					// Fill with xxx (heavy cross hatch fill).
#define DOT_FILL			(BYTE*)"\x80\x00\x08\x00\x80\x00\x08\x00"	// Fill with xxx.
#define FDIAGONAL_FILL		BS_HATCHED, HS_FDIAGONAL					// Fill with \\\.
#define HORIZONTAL_FILL		BS_HATCHED, HS_HORIZONTAL					// Fill with ===.
#define VERTICAL_FILL		BS_HATCHED, HS_VERTICAL						// Fill with |||.
// Old dense fill style
#define BDIAGONAL2_FILL		(BYTE*)"\x44\x88\x11\x22\x44\x88\x11\x22"
#define CROSS2_FILL			(BYTE*)"\xff\x11\x11\x11\xff\x11\x11\x11"
#define DIAGCROSS2_FILL		(BYTE*)"\x55\x88\x55\x22\x55\x88\x55\x22"
#define DOT2_FILL			(BYTE*)"\x88\x00\x22\x00\x88\x00\x22\x00"
#define FDIAGONAL2_FILL		(BYTE*)"\x22\x11\x88\x44\x22\x11\x88\x44"
#define HORIZONTAL2_FILL	(BYTE*)"\x00\x00\xff\x00\x00\x00\xff\x00"
#define VERTICAL2_FILL		(BYTE*)"\x11\x11\x11\x11\x11\x11\x11\x11"
// Old heavy line fill style
#define BDIAGONAL3_FILL		(BYTE*)"\xe0\xc1\x83\x07\x0e\x1c\x38\x70"
#define CROSS3_FILL			(BYTE*)"\x30\x30\x30\x30\x30\x30\xff\xff"
#define DIAGCROSS3_FILL		(BYTE*)"\xc7\x83\xc7\xee\x7c\x38\x7c\xee"
#define DOT3_FILL			(BYTE*)"\xc0\xc0\x0c\x0c\xc0\xc0\x0c\x0c"
#define FDIAGONAL3_FILL		(BYTE*)"\x07\x83\xc1\xe0\x70\x38\x1c\x0e"
#define HORIZONTAL3_FILL	(BYTE*)"\xff\xff\x00\x00\xff\xff\x00\x00"
#define VERTICAL3_FILL		(BYTE*)"\x33\x33\x33\x33\x33\x33\x33\x33"
// Old other fill style
#define INTERLEAVE_FILL		(BYTE*)"\xcc\x33\xcc\x33\xcc\x33\xcc\x33"

//
#if _MSC_VER > 1200 && _MSC_VER < 1900
#define _EASYX_DEPRECATE					__declspec(deprecated("This function is deprecated."))
#define _EASYX_DEPRECATE_WITHNEW(_NewFunc)	__declspec(deprecated("This function is deprecated. Instead, use this new function: " #_NewFunc ". See https://docs.easyx.cn/" #_NewFunc " for details."))
#define _EASYX_DEPRECATE_OVERLOAD(_Func)	__declspec(deprecated("This overload is deprecated. See https://docs.easyx.cn/" #_Func " for details."))
#else
#define _EASYX_DEPRECATE
#define _EASYX_DEPRECATE_WITHNEW(_NewFunc)
#define _EASYX_DEPRECATE_OVERLOAD(_Func)
#endif

struct MOUSEMSG
{
	UINT uMsg;				// Mouse message
	bool mkCtrl : 1;		// Indicates whether the CTRL key is pressed
	bool mkShift : 1;		// Indicates whether the SHIFT key is pressed
	bool mkLButton : 1;		// Indicates whether the left mouse button is pressed
	bool mkMButton : 1;		// Indicates whether the middle mouse button is pressed
	bool mkRButton : 1;		// Indicates whether the right mouse button is pressed
	short x;				// The x-coordinate of the cursor
	short y;				// The y-coordinate of the cursor
	short wheel;			// The distance the wheel is rotated, expressed in multiples or divisions of 120
};

typedef ExMessage EASYXMSG;	// Old message structure

// Old message category
#define EM_MOUSE	1
#define EM_KEY		2
#define EM_CHAR		4
#define EM_WINDOW	8