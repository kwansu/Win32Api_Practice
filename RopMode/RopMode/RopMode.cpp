#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hinst;
LPCTSTR lpszClass = TEXT("Class");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hinst = hInstance;

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

struct Line
{
	int sx, sy;
	int nx, ny;
	bool bDrawLine;
};

Line drawLine = { false };

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage)
	{
	case WM_LBUTTONDOWN:
		drawLine.sx = LOWORD(lParam);
		drawLine.sy = HIWORD(lParam);
		drawLine.nx = LOWORD(lParam);
		drawLine.ny = HIWORD(lParam);
		drawLine.bDrawLine = true;
		break;
	case WM_LBUTTONUP:
		drawLine.bDrawLine = false;
		break;
	case WM_MOUSEMOVE:
		if (drawLine.bDrawLine)
		{
			hdc = GetDC(hWnd);
			SetROP2(hdc, R2_NOT);
			MoveToEx(hdc, drawLine.sx, drawLine.sy, NULL);
			LineTo(hdc, drawLine.nx, drawLine.ny);
			//SetROP2(hdc, R2_COPYPEN);
			drawLine.nx = LOWORD(lParam);
			drawLine.ny = HIWORD(lParam);
			MoveToEx(hdc, drawLine.sx, drawLine.sy, NULL);
			LineTo(hdc, drawLine.nx, drawLine.ny);
			ReleaseDC(hWnd, hdc);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return (DefWindowProc(hWnd, iMessage, wParam, lParam));
	}
	return 0;
}