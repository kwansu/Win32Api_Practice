#ifndef __CHILD_WINDOW_H__
#define __CHILD_WINDOW_H__

#include "BaseWindow.h"

class Wnd_ChildWindow : public Wnd_Base
{
	HWND hParent;
	POINT pos;
	LPRECT lpRect;
	INT nMinWidth, nMinHeight;
	INT nPublicMinRange;
	INT ID;
	LPCTSTR lpchClassName = TEXT("Wnd_ChildWindow");
	TCHAR str[50];
public:
	HWND hNeighborLeft, hNeighborRight, hPublicLeftLine
		, hPublicRightLine, hPublicLeftDownLine, hPublicRightDownLine;

	Wnd_ChildWindow(HWND, INT, int x = 0, int y = 0
		, int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT);
	HRESULT WndProc(HWND, UINT, WPARAM, LPARAM);
	LPCTSTR GetClassName() const
	{
		return lpchClassName;
	}
	BOOL SetDivideWindow();
	void WindowInitialize(Wnd_ChildWindow*);
};

#endif