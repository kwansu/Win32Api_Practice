#ifndef __DRAWING_AREA_H__
#define __DRAWING_AREA_H__

#include "BaseWindow.h"

HRESULT CALLBACK DrawingProc(HWND, UINT, WPARAM, LPARAM);

/*void DrawingRegisterWindow(WNDCLASS &DrawingWnd)
{
	DrawingWnd.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	DrawingWnd.lpfnWndProc = DrawingProc;
	DrawingWnd.lpszClassName = TEXT("DrawingArea");

	RegisterClass(&DrawingWnd);
}*/

#endif