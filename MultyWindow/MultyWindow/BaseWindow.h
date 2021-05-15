#ifndef __BASE_WINDOW_H__
#define __BASE_WINDOW_H__

#include <Windows.h>
#include "resource.h"

class Wnd_Base
{
protected:
	WNDCLASS WndClass;
	HINSTANCE hInstance;
	MSG msg;
	HWND hWnd;

	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
public:
	Wnd_Base()
	{
		hInstance = GetModuleHandle(NULL);

		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = 4;
		WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		WndClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
		WndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
		WndClass.hInstance = hInstance;
		WndClass.lpfnWndProc = Wnd_BaseProc;
		WndClass.lpszClassName = TEXT("Wnd_Base");
		WndClass.lpszMenuName = NULL;
		WndClass.style = CS_HREDRAW | CS_VREDRAW;
	}
	static HRESULT CALLBACK Wnd_BaseProc(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp)
	{
		Wnd_Base* pThis = NULL;
		if (Msg == WM_NCCREATE)
		{
			CREATESTRUCT cs;
			cs = *(LPCREATESTRUCT)lp;
			SetWindowLong(hWnd, 0, (LONG)cs.lpCreateParams);
		}
		pThis = (Wnd_Base*)GetWindowLong(hWnd, 0);
		if (pThis != NULL)
			return pThis->WndProc(hWnd, Msg, wp, lp);

		return DefWindowProc(hWnd, Msg, wp, lp);
	}
	virtual HRESULT WndProc(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp)
	{
		switch (Msg)
		{
		case WM_CREATE:
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, Msg, wp, lp);
		}
		return 0;
	}
	HWND GetHWND() const
	{
		return hWnd;
	}
};

#endif