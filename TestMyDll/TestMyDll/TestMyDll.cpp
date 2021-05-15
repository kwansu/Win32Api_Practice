#include <Windows.h>
#pragma comment(lib, "MyDll.lib")
#include "MyDll.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Class");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW
		, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT
		, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

HBITMAP hBit, hOldBit;
POINT mousePos;

struct Object
{
	int x, y;
	int r = 20;
	int dx, dy;
	RECT GetRect() const
	{
		RECT rt = { x - r, y - r, x + r, y + r };
		return rt;
	}
};

Object target;

LPCTSTR str = TEXT("Collide!");

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, hMemDC;
	PAINTSTRUCT ps;
	RECT crt, rt1, rt2;
	int tem;

	switch (iMessage)
	{
	case WM_CREATE:
		SetTimer(hWnd, 0, 20, NULL);
		target.x = 100;
		target.y = 100;
		target.dx = 1;
		target.dy = -2;
		break;
	case WM_TIMER:
		GetClientRect(hWnd, &crt);
		tem = target.x + target.dx;
		if (tem < target.r)
		{
			target.dx *= -1;
			target.x = -tem;
		}
		else if (tem > crt.right - target.r)
		{
			target.dx *= -1;
			target.x = 2 * crt.right - tem;
		}
		else
		{
			target.x = tem;
		}

		tem = target.y + target.dy;
		if (tem < target.r)
		{
			target.dy *= -1;
			target.y = -tem;
		}
		else if (tem > crt.bottom - target.r)
		{
			target.dy *= -1;
			target.y = 2 * crt.bottom - tem;
		}
		else
		{
			target.y = tem;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		hMemDC = CreateCompatibleDC(hdc);
		GetCursorPos(&mousePos);
		ScreenToClient(hWnd, &mousePos);
		GetClientRect(hWnd, &crt);

		if (hBit == NULL)
			hBit = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);

		hOldBit = (HBITMAP)SelectObject(hMemDC, hBit);

		Rectangle(hMemDC, 0, 0, crt.right, crt.bottom);

		rt1 = target.GetRect();
		Rectangle(hMemDC, rt1.left, rt1.top, rt1.right, rt1.bottom);

		SetRect(&rt2, mousePos.x - 20, mousePos.y - 20, mousePos.x + 20, mousePos.y + 20);
		Rectangle(hMemDC, rt2.left, rt2.top, rt2.right, rt2.bottom);

		if (RectInRect(&rt1, &rt2))
			TextOut(hMemDC, 20, 20, str, lstrlen(str));

		BitBlt(hdc, 0, 0, crt.right, crt.bottom, hMemDC, 0, 0, SRCCOPY);

		SelectObject(hMemDC, hOldBit);
		DeleteDC(hMemDC);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 0);
		PostQuitMessage(0);
		break;
	default:
		return (DefWindowProc(hWnd, iMessage, wParam, lParam));
	}
	return 0;
}