#ifndef __TOOLBOX_H__
#define __TOOLBOX_H__

#include "BaseWindow.h"

HRESULT CALLBACK ToolBoxProc(HWND, UINT, WPARAM, LPARAM);

/*void ToolBoxRegusterWindow(WNDCLASS &ToolBoxWnd)
{
	ToolBoxWnd.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	ToolBoxWnd.lpfnWndProc = ToolBoxProc;
	ToolBoxWnd.lpszClassName = TEXT("ToolBox");

	RegisterClass(&ToolBoxWnd);
}*/

#endif