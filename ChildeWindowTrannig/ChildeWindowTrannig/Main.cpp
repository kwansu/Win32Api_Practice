#include "DrawingArea.h"
#include "InputKey.h"
#include "ToolBox.h"

HRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_Instance;
HWND hMainWnd, hToolBoxWnd, hInputTextWnd, hDrawingAreaWnd;
LPCTSTR lpszClass = TEXT("ChildeWindowTranning");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	g_Instance = hInstance;
	MSG msg;
	WNDCLASS WndClass;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = NULL;
	WndClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
	WndClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.lpfnWndProc = DrawingProc;
	WndClass.lpszClassName = TEXT("Drawing");
	RegisterClass(&WndClass);

	WndClass.lpfnWndProc = InputKeyProc;
	WndClass.lpszClassName = TEXT("InputKey");
	RegisterClass(&WndClass);

	WndClass.lpfnWndProc = ToolBoxProc;
	WndClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	WndClass.lpszClassName = TEXT("ToolBox");
	RegisterClass(&WndClass);

	hMainWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW
		, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT
		, NULL, NULL, hInstance, NULL);
	ShowWindow(hMainWnd, nCmdShow);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

HRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT MainRt, ToolBoxRt, DrawingRt, InputRt;
	static INT num;
	TCHAR str[10];

	switch (Msg)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &MainRt);
		hToolBoxWnd = CreateWindow(TEXT("ToolBox"), NULL, WS_CHILD | WS_THICKFRAME | WS_VISIBLE
			, 0, 0, MainRt.right / 5, MainRt.bottom, hWnd, NULL, g_Instance, NULL);
		hDrawingAreaWnd = CreateWindow(TEXT("Drawing"), NULL, WS_CHILD | WS_THICKFRAME | WS_VISIBLE
			, MainRt.right / 5, 0, MainRt.right * 4 / 5, MainRt.bottom * 4 / 5, hWnd, NULL, g_Instance, NULL);
		hInputTextWnd = CreateWindow(TEXT("InputKey"), NULL, WS_CHILD | WS_THICKFRAME | WS_VISIBLE
			, MainRt.right / 5, MainRt.bottom * 4 / 5, MainRt.right * 4 / 5, MainRt.bottom, hWnd, NULL, g_Instance, NULL);
		break;
	case WM_SIZE:
		GetClientRect(hWnd, &MainRt);
		MoveWindow(hToolBoxWnd, 0, 0, MainRt.right / 5, MainRt.bottom, TRUE);
		MoveWindow(hDrawingAreaWnd, MainRt.right / 5, 0, MainRt.right * 4 / 5, MainRt.bottom * 4 / 5, TRUE);
		MoveWindow(hInputTextWnd, MainRt.right / 5, MainRt.bottom * 4 / 5, MainRt.right * 4 / 5, MainRt.bottom, TRUE);
		break;
	case WM_LBUTTONDOWN:
		num++;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_KEYDOWN:
		num++;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		wsprintf(str, TEXT("%d"), num);
		TextOut(hdc, 100, 100, str, lstrlen(str));
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return (DefWindowProc(hWnd, Msg, wParam, lParam));
	}
	return 0;
}

HRESULT CALLBACK InputKeyProc(HWND hInputKeyWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	static INT num;
	TCHAR str[10] = { NULL };
	HDC hdc;
	PAINTSTRUCT ps;

	switch (Msg)
	{
	case WM_CREATE:
		CreateWindow(TEXT("edit"), TEXT("¿¡µðÆ®"), WS_CHILD | WS_VISIBLE
			, 100, 100, 100, 30, hInputKeyWnd, (HMENU)1, g_Instance, NULL);
			break;
	case WM_KEYDOWN:
		num++;
		InvalidateRect(hInputKeyWnd, NULL, TRUE);
		break;
	case WM_LBUTTONDOWN:
		num++;
		InvalidateRect(hInputKeyWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hInputKeyWnd, &ps);
		wsprintf(str, TEXT("%d"), num);
		TextOut(hdc, 0, 0, str, lstrlen(str));
		EndPaint(hInputKeyWnd, &ps);
		break;
	default:
		return (DefWindowProc(hInputKeyWnd, Msg, wParam, lParam));
	}
	return 0;
}