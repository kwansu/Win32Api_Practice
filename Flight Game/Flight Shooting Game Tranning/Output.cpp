#include "Output.h"

Output_Wnd::Output_Wnd(HWND hParent)
{
	WndClass.lpszClassName = lpszClass;
	WndClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_CHILD | WS_VISIBLE
		, 400, 0, 200, 600, hParent, NULL, hInstance, (LPVOID)this);
}
HRESULT Output_Wnd::WndProc(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp)
{
	return DefWindowProc(hWnd, Msg, wp, lp);
}