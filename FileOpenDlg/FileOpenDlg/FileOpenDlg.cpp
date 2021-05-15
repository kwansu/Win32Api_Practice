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
	OPENFILENAME op;
	TCHAR str[MAX_PATH];
public:
	BaseWnd()
	{
		hInstance = GetModuleHandle(NULL);

		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = 4;
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
			DefWindowProc(hWnd, Msg, wp, lp);
			CREATESTRUCT cs = *(LPCREATESTRUCT)lp;
			SetWindowLongPtr(hWnd, 0, (LONG)(cs.lpCreateParams));
		}
		pThis = (BaseWnd*)GetWindowLongPtr(hWnd, 0);
		if (pThis != NULL)
			return pThis->WndProc(hWnd, Msg, wp, lp);

		return DefWindowProc(hWnd, Msg, wp, lp);
	}
	virtual HRESULT WndProc(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp)
	{
		switch (Msg)
		{
		case WM_CREATE:
			OnCreate(hWnd);
			break;
		case WM_LBUTTONDOWN:
			OnLButtonDown();
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
	HRESULT OnLButtonDown()
	{
		memset(&op, 0, sizeof(OPENFILENAME));
		op.lStructSize = sizeof(OPENFILENAME);
		op.hwndOwner = hWnd;
		op.lpstrFilter = TEXT("모든파일\0*.*\0");
		op.lpstrFile = str;
		op.nMaxFile = MAX_PATH;
		if (GetOpenFileName(&op) != 0)
			MessageBox(hWnd, str, TEXT("경로"), MB_OK);
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
	HWND hWnd;
	MSG msg;

	BaseWnd wnd;
	hWnd = wnd.GetHWND();

	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}