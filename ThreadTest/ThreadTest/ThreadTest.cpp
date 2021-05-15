#include <Windows.h>

HRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hInst;
HWND hMainWnd;
LPCTSTR lpszClassName = TEXT("ThreadTest");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS WndClass;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(hInstance, IDC_APPSTARTING);
	WndClass.hIcon = LoadIcon(hInstance, IDI_ERROR);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClassName;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | WS_VISIBLE;
	RegisterClass(&WndClass);

	hMainWnd = CreateWindow(lpszClassName, lpszClassName, WS_OVERLAPPEDWINDOW
		, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT
		, NULL, NULL, hInstance, NULL);
	ShowWindow(hMainWnd, nCmdShow);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		DispatchMessage(&msg);
	}
	
	return (int)msg.wParam;
}

struct BenuInfo
{
	int x, y, w, h;
	TCHAR* str[3];
	int sleepTime;
};

BenuInfo benu[3] = {
	{ 50, 50, 100, 100, TEXT("첫번째 배너"), TEXT("인터벌 2초"), TEXT("50, 50, 100, 100"), 2000 },
	{ 200, 50, 100, 100, TEXT("두번쨰 배너"), TEXT("인터벌 1초"), TEXT("200, 50, 100, 100"), 1000 },
	{ 350, 50, 100, 100, TEXT("세번째 배너"), TEXT("인터벌 0.5초"), TEXT("350, 50, 100, 100"), 500 }
};

struct ButtonInfo
{
	TCHAR* str[2];
	int sleepTime;
	HWND hWnd;
};

ButtonInfo button_stop = { TEXT("누르면"), TEXT("정지"), 1000, NULL};
ButtonInfo button_start = { TEXT("누르면"), TEXT("다시시작"), 1500, NULL};

DWORD WINAPI MyThreadFunc1(LPVOID Param)
{
	BenuInfo* pBenu = (BenuInfo*)Param;
	HDC hdc = GetDC(hMainWnd);
	RECT rt = { pBenu->x, pBenu->y, pBenu->x + pBenu->w, pBenu->y + pBenu->h };
	int i = 0;

	for (;;)
	{
		Rectangle(hdc, rt.left, rt.top, rt.right, rt.bottom);
		TextOut(hdc, pBenu->x + 30, pBenu->y + 45, pBenu->str[i % 3], lstrlen(pBenu->str[i % 3]));
		GdiFlush();
		Sleep(pBenu->sleepTime);
		i++;
	}

	ReleaseDC(hMainWnd, hdc);
	return 0;
}

DWORD WINAPI MyThreadFunc2(LPVOID Param)
{
	ButtonInfo* pInfo = (ButtonInfo*)Param;
	int i = 0;

	for (;;)
	{
		SetWindowText(pInfo->hWnd, *(pInfo->str));
		Sleep(pInfo->sleepTime);
		i++;
	}
	return 0;
}

#define ID_START 1
#define ID_STOP 2
#define ID_R1 3
#define ID_R2 4
#define ID_R3 5

HRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	static HANDLE hThread[5];
	static int now_thread = 0;
	HWND hButton;

	switch (msg)
	{
	case WM_CREATE:
		hButton = CreateWindow(TEXT("Button"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
			, 100, 200, 50, 20, hWnd, (HMENU)ID_START, g_hInst, NULL);
		button_start.hWnd = hButton;
		hButton = CreateWindow(TEXT("Button"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
			, 100, 250, 50, 20, hWnd, (HMENU)ID_STOP, g_hInst, NULL);
		button_stop.hWnd = hButton;
		CreateWindow(TEXT("Button"), TEXT("benu1"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON
			| WS_GROUP, 200, 200, 50, 20, hWnd, (HMENU)ID_R1, g_hInst, NULL);
		CreateWindow(TEXT("Button"), TEXT("benu2"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON
			, 200, 230, 50, 20, hWnd, (HMENU)ID_R2, g_hInst, NULL);
		CreateWindow(TEXT("Button"), TEXT("benu3"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON
			, 200, 260, 50, 20, hWnd, (HMENU)ID_R3, g_hInst, NULL);

		for (int i = 0; i < 3; i++)
			hThread[i] = CreateThread(NULL, 0, MyThreadFunc1, (LPVOID)&benu[i], 0, NULL);

		hThread[3] = CreateThread(NULL, 0, MyThreadFunc2, (LPVOID)&button_start, 0, NULL);
		hThread[4] = CreateThread(NULL, 0, MyThreadFunc2, (LPVOID)&button_stop, 0, NULL);
		break;
	case WM_COMMAND:
		switch (LOWORD(wp))
		{
		case ID_R1:
			now_thread = 0;
			break;
		case ID_R2:
			now_thread = 1;
			break;
		case ID_R3:
			now_thread = 2;
			break;
		case ID_START:
			ResumeThread(hThread[now_thread]);
			break;
		case ID_STOP:
			SuspendThread(hThread[now_thread]);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
	return 0;
}