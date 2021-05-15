#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("FPS_Check");

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

	hWnd = CreateWindow(lpszClass, lpszClass, WS_CAPTION | WS_SYSMENU
		, CW_USEDEFAULT, CW_USEDEFAULT, 500, 300
		, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

#define FPS_SETTING 20
#define MAX_FPS 100

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int r;
	static DWORD count;
	static int n;
	static DWORD FDT[100];
	static int arr[1000];
	static BOOL bFull = FALSE;
	TCHAR szNowFPS[100], szAverFPS[100];

	switch (iMessage)
	{
	case WM_CREATE:
		SetTimer(hWnd, 0, 1000 / FPS_SETTING, NULL);
		count = GetTickCount();
		n = 0;
		FDT[0] = 10;
		break;
	case WM_TIMER:
		n++;
		if (n > 99)
		{
			n -= 100;
			bFull = TRUE;
		}
		FDT[n] = GetTickCount() - count;
		count = GetTickCount();
		r = rand() % 10;
		r *= r * 100;
		for (int i = 0; i <= r; i++)
		{
			arr[i % 1000] = i % 1000 * 10;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		wsprintf(szNowFPS, TEXT("ÇöÀç FPS : %d"), 1000 / FDT[n]);
		TextOut(hdc, 50, 30, szNowFPS, lstrlen(szNowFPS));
		Rectangle(hdc, 40, 50, 444, 250);
		if (bFull)
			r = 99;
		else
			r = n;
		for (int i = 0; i <= r; i++)
		{
			MoveToEx(hdc, 40 + 4 * i, 250, NULL);
			LineTo(hdc, 40 + 4 * i, 2000 / FDT[i]);
		}
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