#include <Windows.h>
#include "GraphicButton.h"

struct GrpBtnData
{
	HBITMAP Bitmap[4];
	eState State;
};

LRESULT CALLBACK GrpBtnProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
static void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);
static void ChangeState(HWND hWnd, eState State);
static void DeleteAllBitmap(GrpBtnData *pData);
static BOOL lsPtOnMe(HWND hWnd, LPARAM lParam);

class GrpBtnRegister
{
public:
	GrpBtnRegister()
	{
		WNDCLASS WndClass;

		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = sizeof(LONG_PTR);
		WndClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
		WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		WndClass.hIcon = NULL;
		WndClass.hInstance = GetModuleHandle(NULL);
		WndClass.lpfnWndProc = GrpBtnProc;
		WndClass.lpszClassName = TEXT("GrpBtn");
		WndClass.lpszMenuName = NULL;
		WndClass.style = 0;

		RegisterClass(&WndClass);
	}
};

LRESULT CALLBACK GrpBtnProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	GrpBtnData *pData;
	RECT crt;
	POINT pt;
	static eState CheckState;
	HWND hParent;

	pData = (GrpBtnData*)GetWindowLongPtr(hWnd, 0);
	switch (iMsg)
	{
	case WM_CREATE:
		pData = new GrpBtnData;
		SetWindowLongPtr(hWnd, 0, (LONG_PTR)pData);
		return 0;
	case GBM_SETIMAGE:
		DeleteAllBitmap(pData);
		pData->Bitmap[0] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(LOWORD(wParam)));
		pData->Bitmap[1] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(HIWORD(wParam)));
		pData->Bitmap[2] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(LOWORD(lParam)));
		pData->Bitmap[3] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(HIWORD(lParam)));
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case GBM_GETSTATE:
		return pData->State;
	case GBM_SETSTATE:
		if ((GetWindowLongPtr(hWnd, GWL_STYLE) & GBS_CHECK) != 0)
		{
			ChangeState(hWnd, (eState)wParam);
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (pData->Bitmap[pData->State])
		{
			DrawBitmap(hdc, 0, 0, pData->Bitmap[pData->State]);
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		if ((GetWindowLongPtr(hWnd, GWL_STYLE) & GBS_CHECK) == 0)
		{
			ChangeState(hWnd, GB_DOWN);
		}
		else
		{
			CheckState = pData->State;
			ChangeState(hWnd, CheckState == GB_NOMAL ? GB_DOWN : GB_NOMAL);
		}
		SetCapture(hWnd);
		return 0;
	case WM_MOUSEMOVE:
		if (GetCapture() == hWnd)
		{
			if ((GetWindowLongPtr(hWnd, GWL_STYLE) & GBS_CHECK) == 0)
			{
				if (lsPtOnMe(hWnd, lParam))
				{
					ChangeState(hWnd, GB_DOWN);
				}
				else
				{
					ChangeState(hWnd, GB_NOMAL);
				}
			}
			else
			{
				if (lsPtOnMe(hWnd, lParam))
				{
					ChangeState(hWnd, CheckState == GB_NOMAL ? GB_DOWN : GB_NOMAL);
				}
				else
				{
					ChangeState(hWnd, CheckState);
				}
			}
		}
		else
		{
			for (hParent = hWnd; GetParent(hParent); hParent = GetParent(hParent)) { ; }
			if (GetForegroundWindow() != hParent) return 0;
			if ((GetWindowLongPtr(hWnd, GWL_STYLE) & GBS_CHECK) == 0)
			{
				ChangeState(hWnd, GB_HOT);
				SetTimer(hWnd, 0, 50, NULL);
			}
		}
		return 0;
	case WM_LBUTTONUP:
		if (GetCapture() == hWnd)
		{
			ReleaseCapture();
			if ((GetWindowLongPtr(hWnd, GWL_STYLE) & GBS_CHECK) == 0)
			{
				ChangeState(hWnd, GB_NOMAL);
			}
			if (lsPtOnMe(hWnd, lParam))
			{
				SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(
					GetWindowLongPtr(hWnd, GWLP_ID), GBN_CLICKED), (LPARAM)hWnd);
			}
		}
		return 0;
	case WM_TIMER:
		GetCursorPos(&pt);
		GetWindowRect(hWnd, &crt);
		if (PtInRect(&crt, pt) == FALSE)
		{
			KillTimer(hWnd, 0);
			ChangeState(hWnd, GB_NOMAL);
		}
		return 0;
	case WM_ENABLE:
		if (wParam == TRUE)
		{
			ChangeState(hWnd, GB_NOMAL);
		}
		else
		{
			ChangeState(hWnd, GB_DISIBLE);
			KillTimer(hWnd, 0);
		}
		return 0;
	case WM_DESTROY:
		DeleteAllBitmap(pData);
		delete pData;
		return 0;
	}
	return (DefWindowProc(hWnd, iMsg, wParam, lParam));
}

static void ChangeState(HWND hWnd, eState State)
{
	GrpBtnData *pData;

	pData = (GrpBtnData*)GetWindowLongPtr(hWnd, 0);
	if (pData->State == State) return;

	pData->State = State;
	InvalidateRect(hWnd, NULL, FALSE);
}

static void DeleteAllBitmap(GrpBtnData *pData)
{
	int i;

	for (i = 0; i < 4; i++)
	{
		if (pData->Bitmap[i])
		{
			DeleteObject(pData->Bitmap[i]);
		}
	}
}

static void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	if (hBit == NULL) return;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

static BOOL lsPtOnMe(HWND hWnd, LPARAM lParam)
{
	POINT pt;
	RECT wrt;

	pt.x = (int)(short)LOWORD(lParam);
	pt.y = (int)(short)HIWORD(lParam);
	ClientToScreen(hWnd, &pt);
	GetWindowRect(hWnd, &wrt);
	return PtInRect(&wrt, pt);
}