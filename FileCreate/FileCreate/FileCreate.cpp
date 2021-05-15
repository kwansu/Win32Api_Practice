#include <Windows.h>

struct abc
{
	INT a[15];
	TCHAR s;
	void Set()
	{
		for (int i = 0; i < 15; i++)
			a[i] = i;
	}
};

class BaseWnd
{
	LPCTSTR lpszClass = TEXT("BaseWnd");
protected:
	HINSTANCE hInstance;
	HWND hWnd;
	WNDCLASS WndClass;
	HDC hdc;
	PAINTSTRUCT ps;

	abc tem;
	TCHAR *str = TEXT("12345678901234567890");
	INT num = 1234579;
	INT arr[15];
	INT Rnum;
	LPVOID a = &tem;
	TCHAR strResult[50];
	TCHAR str2[50];
	HANDLE hFile;
	DWORD nBytesWritten;
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
	HRESULT WndProc(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp)
	{
		switch (Msg)
		{
		case WM_CREATE:
			OnCreate(hWnd);
			break;
		case WM_LBUTTONDOWN:
			OnLButtonDown(lp);
			break;
		case WM_RBUTTONDOWN:
			OnRButtonDown(lp);
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
	HRESULT OnLButtonDown(LPARAM lp)
	{
		tem.Set();
		tem.s = 'g';
		hFile = CreateFile(TEXT("Text.txt"), GENERIC_WRITE, 0
			, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, TEXT("파일 저장에 실패했습니다.")
				, TEXT("알림"), MB_OK);
			return 0;
		}
		WriteFile(hFile, a, sizeof(tem), &nBytesWritten, NULL);
		CloseHandle(hFile);
		wsprintf(strResult, TEXT("%d, 저장 완료."), nBytesWritten);
		MessageBox(hWnd, strResult, TEXT("알림"), MB_OK);
		return 0;
	}
	HRESULT OnRButtonDown(LPARAM lp)
	{
		hFile = CreateFile(TEXT("Text.txt"), GENERIC_READ, 0
			, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, TEXT("파일 불러오기에 실패했습니다.")
				, TEXT("알림"), MB_OK);
			return 0;
		}
		for (int i = 0; i < 15; i++)
		{
			ReadFile(hFile, &tem.a[i], sizeof(INT), &nBytesWritten, NULL);
		}

		CloseHandle(hFile);
		//wsprintf(str2, TEXT("%d : %d"), nBytesWritten, sizeof(tem));
		//SetWindowText(hWnd, str2);

		wsprintf(strResult, TEXT("%d, %d, %d, %d, %d"), tem.a[0], tem.a[1], tem.a[2]
			, tem.a[3], tem.a[4], tem.a[5]);
		MessageBox(hWnd, strResult, TEXT("알림"), MB_OK);
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