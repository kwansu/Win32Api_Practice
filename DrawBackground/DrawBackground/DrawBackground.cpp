#include <Windows.h>
#include <cmath>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hinst;
LPCTSTR lpszClass = TEXT("Draw Background");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hinst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
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

int x = 100, y = 100;
float pi = 3.14;

void CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	static int dir = 0;
	int r = rand() % 5;
	switch (r)
	{
	case 0:
		break;
	case 1:
		dir -= 2;
		break;
	case 2:
		dir += 2;
		break;
	case 3:
		dir -= 4;
		break;
	case 4:
		dir += 4;
		break;
	}

	if (x + cos(dir*pi / 180) * 5 < 0 || x + cos(dir*pi / 180) * 5 > 600
		|| y + sin(dir*pi / 180) * 5 < 0 || y + sin(dir*pi / 180) * 5 > 600)
		dir += 180;
	else
	{
		x += cos(dir*pi / 180) * 5;
		y += sin(dir*pi / 180) * 5;
	}

	RECT a = { x - 10, y - 10, x + 30, y + 30 };
	InvalidateRect(hWnd, &a, TRUE);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rt;
	static int nowX, nowY;
	static int xpos, ypos;
	static bool bLButton = false;
	switch (iMessage)
	{
	case WM_CREATE:
		SetTimer(hWnd, 1, 20, TimerProc);
		return 0;
	case WM_LBUTTONDOWN:
		xpos = LOWORD(lParam);
		ypos = HIWORD(lParam);
		bLButton = true;
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_LBUTTONUP:
		bLButton = false;
		return 0;
	case WM_MOUSEMOVE:
		nowX = LOWORD(lParam);
		nowY = HIWORD(lParam);
		rt = { xpos, ypos, nowX, nowY };
		InvalidateRect(hWnd, &rt, TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		Ellipse(hdc, x, y, x + 20, y + 20);
		if (bLButton)
			Rectangle(hdc, xpos, ypos, nowX, nowY);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}