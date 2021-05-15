#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MyProcedure(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("First");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = TEXT("MyWindow");
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	WNDCLASS WndClass2;

	WndClass2.cbClsExtra = 0;
	WndClass2.cbWndExtra = 0;
	WndClass2.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass2.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass2.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass2.hInstance = hInstance;
	WndClass2.lpfnWndProc = MyProcedure;
	WndClass2.lpszClassName = TEXT("tranning01");
	WndClass2.lpszMenuName = NULL;
	WndClass2.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass2);

	hWnd = CreateWindow(TEXT("tranning01"), TEXT("첫번째 윈도우 프로그램"), WS_SYSMENU | WS_THICKFRAME
		, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

LRESULT CALLBACK MyProcedure(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_LBUTTONDOWN:
		MessageBeep(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}