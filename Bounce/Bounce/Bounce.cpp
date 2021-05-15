#include <Windows.h>

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

#define R 20
int x, y;
int xi, yi;
HBITMAP hBit;

void OnTimer(HWND hWnd)
{
	RECT crt;
	HDC hdc, hMemDC;
	HBITMAP OldBit;
	int i;

	GetClientRect(hWnd, &crt);
	hdc = GetDC(hWnd);

	if (hBit == NULL)
		hBit = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);

	hMemDC = CreateCompatibleDC(hdc);
	OldBit = (HBITMAP)SelectObject(hMemDC, hBit);

	if (x <= R || x >= crt.right - R)
		xi *= -1;

	if (y <= R || y >= crt.bottom - R)
		yi *= -1;

	x += xi;
	y += yi;

	for (i = 0; i < crt.right; i += 10)
	{
		MoveToEx(hMemDC, i, 0, NULL);
		LineTo(hMemDC, i, crt.bottom);
	}

	for (i = 0; i < crt.bottom; i += 10)
	{
		MoveToEx(hMemDC, 0, i, NULL);
		LineTo(hMemDC, crt.right, i);
	}

	Ellipse(hMemDC, x - R, y - R, x + R, y + R);
	
	SelectObject(hMemDC, OldBit);
	DeleteDC(hMemDC);
	ReleaseDC(hWnd, hMemDC);
	InvalidateRect(hWnd, NULL, FALSE);
}

void DrawBitmap(HDC hdc, HBITMAP hBit)
{
	HDC memDC;
	BITMAP sb;
	HBITMAP BeforeBit;

	memDC = CreateCompatibleDC(hdc);
	BeforeBit = (HBITMAP)SelectObject(memDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &sb);

	BitBlt(hdc, 0, 0, 1024, 768, memDC, 0, 0, SRCCOPY);
	SelectObject(memDC, BeforeBit);
	DeleteDC(memDC);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage)
	{
	case WM_CREATE:
		x = 50;
		y = 50;
		xi = 4;
		yi = 5;
		SetTimer(hWnd, 0, 25, NULL);
		break;
	case WM_TIMER:
		OnTimer(hWnd);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (hBit)
			DrawBitmap(hdc, hBit);
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