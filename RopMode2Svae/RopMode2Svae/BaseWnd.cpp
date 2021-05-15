#include "BaseWnd.h"

BaseWnd::BaseWnd()
{
	hInstance = GetModuleHandle(NULL);

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 4;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
	WndClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	WndClass.lpfnWndProc = BaseWndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_VREDRAW | CS_HREDRAW;
}
HRESULT BaseWnd::WndProc(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp)
{
	switch (Msg)
	{
	case WM_CREATE:
		OnCreate(hWnd);
		break;
	case WM_PAINT:
		OnPaint();
		break;
	case WM_DESTROY:
		OnDestroy();
		break;
	}
	return DefWindowProc(hWnd, Msg, wp, lp);
}
HRESULT BaseWnd::OnCreate(HWND hWnd)
{
	return 0;
}
HRESULT BaseWnd::OnPaint()
{
	hdc = BeginPaint(hWnd, &ps);
	EndPaint(hWnd, &ps);
	return 0;
}
HRESULT BaseWnd::OnDestroy()
{
	PostQuitMessage(0);
	return 0;
}
HWND BaseWnd::GetHWND() const
{
	return hWnd;
}