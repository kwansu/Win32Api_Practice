#ifndef __MULTY_WINODW_H__
#define __MULTY_WINDOW_H__

#include "BaseWindow.h"
#include "ChildWindow.h"

class Wnd_MultyWindow : public Wnd_Base
{
	LPCTSTR class_name = TEXT("Wnd_MultyWindow");
	Wnd_ChildWindow* pStartChild;
	Wnd_ChildWindow** childs;
	INT len;

public:
	Wnd_MultyWindow(int x = CW_USEDEFAULT, int y = CW_USEDEFAULT
		, int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT);
	HRESULT WndProc(HWND, UINT, WPARAM, LPARAM);
	INT AddChild();
	LPCTSTR GetClassName() const
	{
		return class_name;
	}
};

#endif