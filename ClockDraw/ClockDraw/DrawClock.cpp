#include <Windows.h>
#include <cmath>

TCHAR* IntToStr(int num)
{
	TCHAR tem[20] = { NULL };
	int len = 0;
	while (num)
	{
		tem[len++] = (num % 10) + '0';
		num /= 10;
	}
	TCHAR* str = new TCHAR[len-- + 1];
	for (int i = 0; i <= len; i++)
		str[i] = tem[len - i];

	str[len + 1] = NULL;
	return str;
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hinst;
LPCTSTR lpszClass = TEXT("Draw Clock");

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	SYSTEMTIME st;
	static float second;
	static float minute;
	static float hour;
	static float pi = 3.14159;

	switch (iMessage)
	{
	case WM_CREATE:
		SetTimer(hWnd, 1, 1000, NULL);
		return 0;
	case WM_TIMER:
		GetLocalTime(&st);
		second = st.wSecond;
		minute = st.wMinute;
		hour = st.wHour;
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		Ellipse(hdc, 200, 200, 600, 600);
		MoveToEx(hdc, 400, 400, NULL);
		LineTo(hdc, 400 + (cos((second * 6 - 90) * pi / 180) * 200), 400 + (sin((second * 6 - 90) * pi / 180) * 200));
		MoveToEx(hdc, 400, 400, NULL);
		LineTo(hdc, 400 + (cos((minute * 6 - 90) * pi / 180) * 180), 400 + (sin((minute * 6 - 90) * pi / 180) * 180));
		MoveToEx(hdc, 400, 400, NULL);
		LineTo(hdc, 400 + (cos((hour * 30 - 90) * pi / 180) * 150), 400 + (sin((hour * 30 - 90) * pi / 180) * 150));
		TextOut(hdc, 100, 100, IntToStr(second), lstrlen(IntToStr(second)));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}