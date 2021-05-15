#include <Windows.h>

HRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Test");
HWND hMainWnd;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	WNDCLASS WndClass;
	MSG msg;

	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
	WndClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_VREDRAW | CS_HREDRAW;
	RegisterClass(&WndClass);

	hMainWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL
		, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT
		, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hMainWnd, nCmdShow);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

#define WM_ENTERSIZE WM_USER+1
WNDPROC OldEditProc;

HRESULT CALLBACK EditSubProc(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp)
{
	switch (Msg)
	{
	case WM_KEYDOWN:
		if (wp == VK_RETURN)
		{
			WPARAM ID = (WPARAM)GetWindowLongPtr(hWnd, GWLP_ID);
			SendMessage(hMainWnd, WM_ENTERSIZE, ID, 0);
			SetFocus(hMainWnd);
		}
		break;
	}
	return CallWindowProc(OldEditProc, hWnd, Msg, wp, lp);
}

struct Pos
{
	INT x, y;
	BOOL bStart = FALSE;
};

HRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp)
{
	static HWND hVEdit, hHEdit;
	static INT iWidth, iHeight;
	static INT iPosX = 0, iPosY = 0;
	static SCROLLINFO siV, siH;
	static Pos p[10000];
	static INT len = 0;
	static BOOL bDraw = FALSE;
	static INT x, y;
	RECT rt;
	INT i;

	HDC hdc;
	PAINTSTRUCT ps;

	switch (Msg)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &rt);
		hVEdit = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER
			| ES_NUMBER, 100, 100, 100, 20, hWnd, (HMENU)1, g_hInst, NULL);
		hHEdit = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER
			| ES_NUMBER, 100, 200, 100, 20, hWnd, (HMENU)2, g_hInst, NULL);
		iWidth = rt.right, iHeight = rt.bottom;

		OldEditProc = (WNDPROC)SetWindowLongPtr(hVEdit, GWLP_WNDPROC, (LONG_PTR)EditSubProc);
		SetWindowLongPtr(hHEdit, GWLP_WNDPROC, (LONG_PTR)EditSubProc);

		siV.cbSize = sizeof(SCROLLINFO);
		siV.fMask = SIF_ALL;
		siV.nPage = rt.right;
		siV.nMin = 0;
		siV.nMax = iWidth - siV.nPage;
		siV.nPos = 0;

		siH.cbSize = sizeof(SCROLLINFO);
		siH.fMask = SIF_ALL;
		siH.nPage = rt.bottom;
		siH.nMin = 0;
		siH.nMax = iHeight - siH.nPage;
		siH.nPos = 0;

		SetScrollInfo(hWnd, SB_VERT, &siV, TRUE);
		SetScrollInfo(hWnd, SB_HORZ, &siH, TRUE);
		break;
	case WM_SIZE:
		GetClientRect(hWnd, &rt);
		siV.nPage = rt.right;
		siH.nPage = rt.bottom;
		if (iWidth < rt.right)
			iWidth = rt.right;
		if (iHeight < rt.bottom)
			iHeight = rt.bottom;
		SetScrollInfo(hWnd, SB_VERT, &siV, TRUE);
		SetScrollInfo(hWnd, SB_HORZ, &siH, TRUE);
		break;
	case WM_LBUTTONDOWN:
		bDraw = TRUE;
		x = LOWORD(lp);
		y = HIWORD(lp);
		p[len].x = x - iPosX;
		p[len].y = y - iPosY;
		p[len].bStart = TRUE;
		len++;
		break;
	case WM_LBUTTONUP:
		bDraw = FALSE;
		break;
	case WM_MOUSEMOVE:
		if (bDraw)
		{
			hdc = GetDC(hWnd);
			MoveToEx(hdc, x, y, NULL);
			x = LOWORD(lp);
			y = HIWORD(lp);
			LineTo(hdc, x, y);
			p[len].x = x - iPosX;
			p[len].y = y - iPosY;
			len++;
			ReleaseDC(hWnd, hdc);
		}
		break;
	case WM_VSCROLL:
		i = 0;
		switch (LOWORD(wp))
		{
		case SB_LINEUP:
			i = -1;
			break;
		case SB_LINEDOWN:
			i = 1;
			break;
		case SB_PAGEUP:
			i = -20;
			break;
		case SB_PAGEDOWN:
			i = 20;
			break;
		case SB_THUMBTRACK:
			i = HIWORD(wp) - iPosX;
		}
		iPosX += i;
		siV.nPos = iPosX;
		ScrollWindow(hWnd, -i, 0, NULL, NULL);
		SetScrollInfo(hWnd, SB_VERT, &siV, TRUE);
	case WM_ENTERSIZE:
	{
		RECT rt;
		GetClientRect(hWnd, &rt);
		INT  size;
		if (wp == 1)
		{
			size = GetDlgItemInt(hWnd, 1, NULL, TRUE);
			if (size < rt.right)
			{
				iWidth = rt.right;
				SetDlgItemInt(hWnd, 1, iWidth, TRUE);
			}
			else
			{
				iWidth = size;
				siV.nMax = iWidth;
			}
			SetScrollInfo(hWnd, SB_VERT, &siV, TRUE);
		}
		if (wp == 2)
		{
			size = GetDlgItemInt(hWnd, 2, NULL, TRUE);
			if (size < rt.bottom)
			{
				iHeight = rt.bottom;
				SetDlgItemInt(hWnd, 2, iHeight, TRUE);
			}
			else
			{
				iHeight = size;
				siH.nMax = iHeight;
			}
			SetScrollInfo(hWnd, SB_HORZ, &siH, TRUE);
		}
	}
	break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		for (i = 0; i < len; i++)
		{
			if (!p[i].bStart)
			{
				MoveToEx(hdc, p[i - 1].x, p[i - 1].y, NULL);
				LineTo(hdc, p[i].x, p[i].y);
			}
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, Msg, wp, lp);
	}
	return 0;
}