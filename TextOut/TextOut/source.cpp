#include <Windows.h>

int StrLen(LPCTSTR str)
{
	int len = 0;
	while (str[len])
		len++;

	return len;
}
int StrLen(TCHAR* str)
{
	int len = 0;
	while (str[len])
		len++;

	return len;
}
void MyTextOut(HDC hdc, int x, int y, TCHAR* str)
{
	TextOut(hdc, x, y, str, StrLen(str));
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("TextOut");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS wndClass;
	g_hInst = hInstance;

	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = lpszClass;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wndClass);

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

	switch (iMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		MyTextOut(hdc, 100, 100, TEXT("가나다라"));
		SetTextAlign(hdc, TA_CENTER);
		MyTextOut(hdc, 100, 150, TEXT("마바사"));
		MyTextOut(hdc, 100, 200, TEXT("아자차카타파하"));
		SetTextAlign(hdc, TA_UPDATECP);
		MyTextOut(hdc, 100, 300, TEXT(" 이글은 끊기지 안고 계속 이어집니다."));
		MyTextOut(hdc, 100, 400, TEXT(" 보세요. 안 끊기지요?"));
		EndPaint(hWnd, &ps);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}