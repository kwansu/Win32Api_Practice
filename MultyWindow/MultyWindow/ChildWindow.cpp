#include "ChildWindow.h"

Wnd_ChildWindow::Wnd_ChildWindow(HWND hWndParent, INT id, int x, int y, int nWidth, int nHeigh)
{
	ID = id;
	hParent = hWndParent;
	hNeighborLeft = NULL;
	hNeighborRight = NULL;
	hPublicLeftDownLine = NULL;
	hPublicLeftLine = NULL;
	hPublicRightDownLine = NULL;
	hPublicRightLine = NULL;

	WndClass.lpszClassName = lpchClassName;
	//WndClass.style = NULL;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpchClassName, lpchClassName, WS_CHILD | WS_VISIBLE | WS_THICKFRAME | WS_CAPTION
		, x, y, nWidth, nHeigh, hWndParent, (HMENU)NULL, hInstance, (LPVOID)this);
}
HRESULT Wnd_ChildWindow::WndProc(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp)
{
	switch (Msg)
	{
	case WM_CREATE:
		break;
	case WM_SIZING:
		switch (wp)
		{
		case WMSZ_LEFT:
			lpRect = (LPRECT)lp;

			GetWindowRect(hNeighborLeft, &rect);
			ScreenToClient(hNeighborLeft, (LPPOINT)&rect);
			ScreenToClient(hNeighborLeft, (LPPOINT)&rect.right);
			MoveWindow(hNeighborLeft, rect.left, rect.top
				, lpRect->left - rect.left, rect.bottom - rect.top, TRUE);

			GetWindowRect(hPublicLeftDownLine, &rect);
			ScreenToClient(hPublicLeftDownLine, (LPPOINT)&rect);
			ScreenToClient(hPublicLeftDownLine, (LPPOINT)&rect.right);
			MoveWindow(hPublicLeftDownLine, rect.left, rect.top
				, lpRect->left - rect.left, rect.bottom - rect.top, TRUE);

			GetWindowRect(hPublicLeftLine, &rect);
			ScreenToClient(hPublicLeftLine, (LPPOINT)&rect);
			ScreenToClient(hPublicLeftLine, (LPPOINT)&rect.right);
			MoveWindow(hPublicLeftLine, lpRect->left, rect.top
				, rect.right - lpRect->left, rect.bottom - rect.top, TRUE);
			break;
		case WMSZ_RIGHT:
			lpRect = (LPRECT)lp;

			GetWindowRect(hNeighborRight, &rect);
			ScreenToClient(hNeighborRight, (LPPOINT)&rect);
			ScreenToClient(hNeighborRight, (LPPOINT)&rect.right);
			MoveWindow(hNeighborRight, lpRect->right, rect.top
				, rect.right - lpRect->right, rect.bottom - rect.top, TRUE);

			GetWindowRect(hPublicRightDownLine, &rect);
			ScreenToClient(hPublicRightDownLine, (LPPOINT)&rect);
			ScreenToClient(hPublicRightDownLine, (LPPOINT)&rect.right);
			MoveWindow(hPublicRightDownLine, lpRect->right, rect.top
				, rect.right - lpRect->right, rect.bottom - rect.top, TRUE);

			GetWindowRect(hPublicRightLine, &rect);
			ScreenToClient(hPublicRightLine, (LPPOINT)&rect);
			ScreenToClient(hPublicRightLine, (LPPOINT)&rect.right);
			MoveWindow(hPublicRightLine, rect.left, rect.top
				, lpRect->right - rect.left, rect.bottom - rect.top, TRUE);
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetRect(&rect, 0, 0, 200, 300);
		wsprintf(str, TEXT("ID:%d\n1:%d\n2:%d\n3:%d\n4:%d\n5:%d\n6:%d\nleft:%d\ntop:%d\nright:%d\nbottom:%d")
			, ID, hNeighborLeft, hNeighborRight, hPublicLeftLine
			, hPublicRightLine, hPublicLeftDownLine, hPublicRightDownLine
			, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
		DrawText(hdc, str, -1, &rect, DT_LEFT | DT_WORDBREAK);
		EndPaint(hWnd, &ps);
		break;
	default:
		return DefWindowProc(hWnd, Msg, wp, lp);
	}
	return 0;
}
BOOL Wnd_ChildWindow::SetDivideWindow()
{
	Wnd_ChildWindow* pDividedObj;
	RECT temRect;
	GetCursorPos(&pos);
	HWND hPointWnd = WindowFromPoint(pos);
	pDividedObj = (Wnd_ChildWindow*)GetWindowLong(hPointWnd, 0);
	if (!pDividedObj)
		return FALSE;

	GetWindowRect(hPointWnd, &temRect);
	if (pos.y >= (temRect.bottom - temRect.top) / 2)
	{
		hPublicLeftLine = pDividedObj->hPublicLeftLine;
		hPublicRightLine = pDividedObj->hPublicRightLine;
		hPublicLeftDownLine = pDividedObj->hPublicLeftDownLine;
		hPublicRightDownLine = pDividedObj->hPublicRightDownLine;

		pDividedObj->hPublicLeftLine = hWnd;
		pDividedObj->hPublicRightLine = hWnd;
		pDividedObj->hPublicLeftDownLine = NULL;
		pDividedObj->hPublicRightDownLine = NULL;

		GetClientRect(hPointWnd, &rect);
		SetWindowPos(hPointWnd, NULL, NULL, NULL
			, rect.right, rect.bottom / 2, SWP_NOMOVE | SWP_NOZORDER);
		GetWindowRect(hPointWnd, &temRect);
		SetWindowPos(hWnd, NULL, temRect.left, temRect.bottom
			, rect.right, rect.bottom / 2, SWP_NOZORDER);
	}
	else if (pos.x <= (temRect.right - temRect.left) / 2)
	{
		hNeighborLeft = pDividedObj->hNeighborLeft;
		hPublicLeftLine = pDividedObj->hPublicLeftLine;
		hPublicLeftDownLine = pDividedObj->hPublicLeftDownLine;
		hNeighborRight = hPointWnd;
		hPublicRightDownLine = hPointWnd;

		pDividedObj->hNeighborLeft = hWnd;
		pDividedObj->hPublicLeftLine = NULL;
		pDividedObj->hPublicLeftDownLine = hWnd;

		GetClientRect(hPointWnd, &rect);
		SetWindowPos(hPointWnd, NULL, (temRect.right - temRect.left) / 2, temRect.top
			, rect.right / 2, rect.bottom, SWP_NOZORDER);
		SetWindowPos(hWnd, NULL, temRect.left, temRect.bottom
			, rect.right / 2, rect.bottom, SWP_NOZORDER);
	}
	else
	{
		hNeighborRight = pDividedObj->hNeighborRight;
		hPublicRightLine = pDividedObj->hPublicRightLine;
		hPublicRightDownLine = pDividedObj->hPublicRightDownLine;
		hNeighborLeft = hPointWnd;
		hPublicLeftDownLine = hPointWnd;

		pDividedObj->hNeighborRight = hWnd;
		pDividedObj->hPublicRightLine = NULL;
		pDividedObj->hPublicRightDownLine = hWnd;

		GetClientRect(hPointWnd, &rect);
		SetWindowPos(hPointWnd, NULL, temRect.left, temRect.bottom
			, rect.right / 2, rect.bottom, SWP_NOZORDER);
		SetWindowPos(hWnd, NULL, (temRect.right - temRect.left) / 2, temRect.top
			, rect.right / 2, rect.bottom, SWP_NOZORDER);
	}
	return TRUE;
}
void Wnd_ChildWindow::WindowInitialize(Wnd_ChildWindow* pDividedObj)
{
	RECT temRect;
	HWND hDividedWnd = pDividedObj->GetHWND();

	hNeighborRight = pDividedObj->hNeighborRight;
	hPublicRightLine = pDividedObj->hPublicRightLine;
	hPublicRightDownLine = pDividedObj->hPublicRightDownLine;
	hNeighborLeft = hDividedWnd;
	hPublicLeftDownLine = hDividedWnd;

	pDividedObj->hNeighborRight = hWnd;
	pDividedObj->hPublicRightLine = NULL;
	pDividedObj->hPublicRightDownLine = hWnd;

	GetWindowRect(hDividedWnd, &temRect);
	ScreenToClient(hParent, (LPPOINT)&temRect);
	ScreenToClient(hParent, (LPPOINT)&temRect.right);
	SetWindowPos(hWnd, NULL, (temRect.right - temRect.left) / 2, rect.top
		, (temRect.right - temRect.left) / 2, temRect.bottom, SWP_NOZORDER);
	SetWindowPos(hDividedWnd, NULL, temRect.left, temRect.top
		, (temRect.right - temRect.left) / 2, temRect.bottom, SWP_NOZORDER);
}