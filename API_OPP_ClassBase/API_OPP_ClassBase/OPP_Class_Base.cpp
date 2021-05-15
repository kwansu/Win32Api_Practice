#include <Windows.h>

class BaseWnd
{
	LPCTSTR lpszClass = TEXT("BaseWnd");
protected:
	HINSTANCE hInstance;
	HWND hWnd;
	WNDCLASS WndClass;
	HDC hdc;
	PAINTSTRUCT ps;
public:
	BaseWnd()
	{
		hInstance = GetModuleHandle(NULL);

		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = sizeof(LPVOID);
		WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		WndClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
		WndClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
		WndClass.lpfnWndProc = BaseWndProc;
		WndClass.lpszClassName = lpszClass;
		WndClass.lpszMenuName = NULL;
		WndClass.style = CS_VREDRAW | CS_HREDRAW;
		RegisterClass(&WndClass);

		hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT
			, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, static_cast<LPVOID>(this));
	}
	static HRESULT CALLBACK BaseWndProc(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp)
	{
		BaseWnd* pThis = NULL;
		if (Msg == WM_NCCREATE)
		{
			CREATESTRUCT cs = *(LPCREATESTRUCT)lp;
			SetWindowLongPtr(hWnd, 0, (LONG)(cs.lpCreateParams));
			DefWindowProc(hWnd, Msg, wp, lp);
		}
		pThis = (BaseWnd*)GetWindowLongPtr(hWnd, 0);
		if (pThis != NULL)
			return pThis->WndProc(hWnd, Msg, wp, lp);

		return DefWindowProc(hWnd, Msg, wp, lp);
	}
	HRESULT WndProc(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp)
	{
		switch (Msg)
		{
		case WM_CREATE:
			OnCreate(hWnd);
			break;
		case WM_PAINT:
			OnPaint();
			break;
		case WM_DESTROY:
			OnDestroy();
			break;
		}
		return DefWindowProc(hWnd, Msg, wp, lp);
	}
	HRESULT OnCreate(HWND hWnd)
	{
		return 0;
	}
	HRESULT OnPaint()
	{
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	}
	HRESULT OnDestroy()
	{
		PostQuitMessage(0);
		return 0;
	}

	HWND GetHWND() const
	{
		return hWnd;
	}
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	MSG Message;

	BaseWnd wnd;
	ShowWindow(wnd.GetHWND(), nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}