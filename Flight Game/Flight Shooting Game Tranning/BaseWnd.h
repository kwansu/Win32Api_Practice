#ifndef __BASE_WND_H__
#define __BASE_WND_H__

#include <Windows.h>

class BaseWnd
{
protected:
	HINSTANCE hInstance;
	HWND hWnd;
	WNDCLASS WndClass;
	HDC hdc;
	PAINTSTRUCT ps;
public:
	BaseWnd()
	{
		hInstance = GetModuleHandle(NULL);

		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = 4;
		WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		WndClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
		WndClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
		WndClass.hInstance = hInstance;
		WndClass.lpfnWndProc = BaseWndProc;
		WndClass.lpszClassName = TEXT("BaseWnd");
		WndClass.lpszMenuName = NULL;
		WndClass.style = CS_HREDRAW | CS_VREDRAW;
	}
	static HRESULT CALLBACK BaseWndProc(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp)
	{
		BaseWnd* pThis = NULL;
		if (Msg == WM_NCCREATE)
		{
			CREATESTRUCT cs = *(LPCREATESTRUCT)lp;
			SetWindowLongPtr(hWnd, 0, (LONG)cs.lpCreateParams);
		}
		pThis = (BaseWnd*)GetWindowLongPtr(hWnd, 0);
		if (pThis != NULL)
			return pThis->WndProc(hWnd, Msg, wp, lp);

		return DefWindowProc(hWnd, Msg, wp, lp);
	}
	virtual HRESULT WndProc(HWND, UINT, WPARAM, LPARAM) = 0;
	HWND GetHWND() const
	{
		return hWnd;
	}
};

#endif