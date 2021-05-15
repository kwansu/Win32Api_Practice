#ifndef __PAINT_H__
#define __PAINT_H__

#define WM_TOOLCHANGE WM_USER+1
#define WM_DLGEND WM_USER+2
#define WM_SAVE	WM_USER+3
#define WM_LOAD WM_USER+4

#include "BaseWnd.h"
#include "resource.h"
#include "List.h"

enum TOOL
{
	PAN, STRAIGHT, RECTANGLE, ELLIPSE
};

struct Drawing
{
	INT x, y;
	BOOL bStart;
	TOOL tTools;
};

class PaintWnd : public BaseWnd
{
	LPCTSTR lpszClass = TEXT("Paint");
	HWND hDlg = NULL;
	HANDLE hFile;
	DWORD cbWritten;
	TCHAR chNotify[50];

	BOOL bDraw = FALSE;
	TOOL tTool = PAN;
	List<Drawing> drawings;
	POINT beforePos;
	POINT mousePos;
public:
	PaintWnd();
	HRESULT WndProc(HWND, UINT, WPARAM, LPARAM);
	HRESULT OnCreate(HWND);
	HRESULT OnPaint();
	HRESULT OnKeyDown(WPARAM);
	HRESULT OnLButtonDown(LPARAM);
	HRESULT OnLButtonUp(LPARAM);
	HRESULT OnMouseMove(LPARAM);
	//HRESULT OnRButtonDown(LPARAM);
	HRESULT OnToolChange(WPARAM);
	HRESULT OnSave();
	HRESULT OnLoad();
};

BOOL CALLBACK OptionProc(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp);

#endif