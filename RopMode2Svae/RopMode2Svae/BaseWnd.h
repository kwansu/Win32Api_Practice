#ifndef __BASE_WND_H__
#define __BASE_WND_H__

#include <Windows.h>

class BaseWnd
{
	LPCTSTR lpszClass = TEXT("BaseWnd");
protected:
	HINSTANCE hInstance;
	HWND hWnd;
	WNDCLASS WndClass;
	HDC hdc;
	PAINTSTRUCT ps;
public:
	BaseWnd();
	static HRESULT CALLBACK BaseWndProc(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp)
	{
		BaseWnd* pThis = NULL;
		if (Msg == WM_NCCREATE)
		{
			DefWindowProc(hWnd, Msg, wp, lp);
			CREATESTRUCT cs = *(LPCREATESTRUCT)lp;
			SetWindowLongPtr(hWnd, 0, (LONG)(cs.lpCreateParams));
		}
		pThis = (BaseWnd*)GetWindowLongPtr(hWnd, 0);
		if (pThis != NULL)
			return pThis->WndProc(hWnd, Msg, wp, lp);

		return DefWindowProc(hWnd, Msg, wp, lp);
	}
	virtual HRESULT WndProc(HWND, UINT, WPARAM, LPARAM);
	HRESULT OnCreate(HWND);
	HRESULT OnPaint();
	HRESULT OnDestroy();
	HWND GetHWND() const;
};

#endif