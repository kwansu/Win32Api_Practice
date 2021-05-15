#include "Paint.h"

PaintWnd::PaintWnd()
{
	WndClass.lpszClassName = lpszClass;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW
		, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT
		, NULL, (HMENU)NULL, hInstance, (LPVOID)this);
}
HRESULT PaintWnd::OnCreate(HWND hWnd)
{
	return 0;
}
HRESULT PaintWnd::OnPaint()
{
	hdc = BeginPaint(hWnd, &ps);
	SetROP2(hdc, R2_NOT);
	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	INT len = drawings.GetLen();
	//TCHAR str[10];
	//wsprintf(str, TEXT("%d"), len);
	//SetWindowText(hWnd, str);
	for (INT n = 0; n < len; n++)
	{
		switch (drawings[n].tTools)
		{
		case PAN:
			if (drawings[n].bStart)
				MoveToEx(hdc, drawings[n].x, drawings[n].y, NULL);
			else
			{
				LineTo(hdc, drawings[n].x, drawings[n].y);
				MoveToEx(hdc, drawings[n].x, drawings[n].y, NULL);
			}
			break;
		case STRAIGHT:
			if (drawings[n].bStart)
				MoveToEx(hdc, drawings[n].x, drawings[n].y, NULL);
			else
				LineTo(hdc, drawings[n].x, drawings[n].y);
			break;
		case RECTANGLE:
			if (drawings[n].bStart)
			{
				beforePos.x = drawings[n].x;
				beforePos.y = drawings[n].y;
			}
			else
				Rectangle(hdc, beforePos.x, beforePos.y, drawings[n].x, drawings[n].y);
			break;
		case ELLIPSE:
			if (drawings[n].bStart)
			{
				beforePos.x = drawings[n].x;
				beforePos.y = drawings[n].y;
			}
			else
				Ellipse(hdc, beforePos.x, beforePos.y, drawings[n].x, drawings[n].y);
			break;
		}
	}
	EndPaint(hWnd, &ps);
	return 0;
}
HRESULT PaintWnd::OnKeyDown(WPARAM wp)
{
	if (wp == VK_RETURN)
	{
		if (!IsWindow(hDlg))
		{
			hDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1)
				, hWnd, OptionProc);
			SendMessage(hDlg, WM_TOOLCHANGE, (WPARAM)tTool, 0);
			ShowWindow(hDlg, SW_SHOW);
		}
	}
	return 0;
}
HRESULT PaintWnd::OnLButtonDown(LPARAM lp)
{
	bDraw = TRUE;
	mousePos.x = LOWORD(lp);
	mousePos.y = HIWORD(lp);
	beforePos.x = LOWORD(lp);
	beforePos.y = HIWORD(lp);
	Drawing tem = { mousePos.x, mousePos.y, TRUE, tTool };
	drawings = tem;
	return 0;
}
HRESULT PaintWnd::OnLButtonUp(LPARAM lp)
{
	bDraw = FALSE;
	hdc = GetDC(hWnd);
	Drawing tem;
	switch (tTool)
	{
	case STRAIGHT:
		SetROP2(hdc, R2_MERGEPEN);
		MoveToEx(hdc, mousePos.x, mousePos.y, NULL);
		LineTo(hdc, LOWORD(lp), HIWORD(lp));
		tem = { LOWORD(lp), HIWORD(lp), FALSE, STRAIGHT };
		drawings = tem;
		break;
	case RECTANGLE:
		SelectObject(hdc, GetStockObject(NULL_BRUSH));
		SetROP2(hdc, R2_MERGEPEN);
		Rectangle(hdc, mousePos.x, mousePos.y, LOWORD(lp), HIWORD(lp));
		tem = { LOWORD(lp), HIWORD(lp), FALSE, RECTANGLE };
		drawings = tem;
		break;
	case ELLIPSE :
		SelectObject(hdc, GetStockObject(NULL_BRUSH));
		SetROP2(hdc, R2_MERGEPEN);
		Ellipse(hdc, mousePos.x, mousePos.y, LOWORD(lp), HIWORD(lp));
		tem = { LOWORD(lp), HIWORD(lp), FALSE, ELLIPSE };
		drawings = tem;
		break;
	}
	//InvalidateRect(hWnd, NULL, TRUE);
	ReleaseDC(hWnd, hdc);
	return 0;
}
HRESULT PaintWnd::OnMouseMove(LPARAM lp)
{
	if (bDraw)
	{
		hdc = GetDC(hWnd);
		switch (tTool)
		{
		case PAN:
		{
			MoveToEx(hdc, beforePos.x, beforePos.y, NULL);
			LineTo(hdc, LOWORD(lp), HIWORD(lp));
			beforePos.x = LOWORD(lp);
			beforePos.y = HIWORD(lp);
			Drawing tem = { LOWORD(lp), HIWORD(lp), FALSE, PAN };
			drawings = tem;
		}
		break;
		case STRAIGHT:
			SetROP2(hdc, R2_NOT);
			MoveToEx(hdc, mousePos.x, mousePos.y, NULL);
			LineTo(hdc, beforePos.x, beforePos.y);
			beforePos.x = LOWORD(lp);
			beforePos.y = HIWORD(lp);
			MoveToEx(hdc, mousePos.x, mousePos.y, NULL);
			LineTo(hdc, beforePos.x, beforePos.y);
			break;
		case RECTANGLE:
			SetROP2(hdc, R2_NOT);
			SelectObject(hdc, GetStockObject(NULL_BRUSH));
			Rectangle(hdc, mousePos.x, mousePos.y, beforePos.x, beforePos.y);
			beforePos.x = LOWORD(lp);
			beforePos.y = HIWORD(lp);
			Rectangle(hdc, mousePos.x, mousePos.y, beforePos.x, beforePos.y);
			break;
		case ELLIPSE:
			SetROP2(hdc, R2_NOT);
			SelectObject(hdc, GetStockObject(NULL_BRUSH));
			Ellipse(hdc, mousePos.x, mousePos.y, beforePos.x, beforePos.y);
			beforePos.x = LOWORD(lp);
			beforePos.y = HIWORD(lp);
			Ellipse(hdc, mousePos.x, mousePos.y, beforePos.x, beforePos.y);
			break;
		}
		ReleaseDC(hWnd, hdc);
	}
	return 0;
}
HRESULT PaintWnd::OnToolChange(WPARAM wp)
{
	switch (wp)
	{
	case PAN:
		tTool = PAN;
		break;
	case STRAIGHT:
		tTool = STRAIGHT;
		break;
	case RECTANGLE:
		tTool = RECTANGLE;
		break;
	case ELLIPSE:
		tTool = ELLIPSE;
		break;
	}
	return 0;
}
HRESULT PaintWnd::OnSave()
{
	hFile = CreateFile(TEXT("SaveFile.txt"), GENERIC_WRITE, 0
		, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(hWnd, TEXT("저장 실패."), TEXT("알림"), MB_OK);
		return 0;
	}
	int i;
	for (i = 0; i < drawings.GetLen(); i++)
		WriteFile(hFile, &drawings[i], sizeof(Drawing), &cbWritten, NULL);

	CloseHandle(hFile);
	wsprintf(chNotify, TEXT("%d : 저장 성공."), i);
	MessageBox(hWnd, chNotify, TEXT("알림."), MB_OK);
	return 0;
}
HRESULT PaintWnd::OnLoad()
{
	hFile = CreateFile(TEXT("SaveFile.txt"), GENERIC_READ, 0
		, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(hWnd, TEXT("불러오기 실패."), TEXT("알림"), MB_OK);
		return 0;
	}
	Drawing tem;
	do
	{
		ReadFile(hFile, &tem, sizeof(Drawing), &cbWritten, NULL);
		drawings = tem;
	} while (cbWritten == sizeof(Drawing));

	CloseHandle(hFile);
	wsprintf(chNotify, TEXT("%d : 불러오기 성공."), drawings.GetLen());
	MessageBox(hWnd, chNotify, TEXT("알림."), MB_OK);
	InvalidateRect(hWnd, NULL, TRUE);
	return 0;
}
HRESULT PaintWnd::WndProc(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp)
{
	switch (Msg)
	{
	case WM_CREATE:
		OnCreate(hWnd);
		break;
	case WM_PAINT:
		OnPaint();
		break;
	case WM_KEYDOWN:
		OnKeyDown(wp);
		break;
	case WM_LBUTTONDOWN:
		OnLButtonDown(lp);
		break;
	case WM_LBUTTONUP:
		OnLButtonUp(lp);
		break;
	case WM_MOUSEMOVE:
		OnMouseMove(lp);
		break;
	case WM_TOOLCHANGE:
		OnToolChange(wp);
		break;
	case WM_SAVE:
		OnSave();
		break;
	case WM_LOAD:
		OnLoad();
		break;
	case WM_DESTROY:
		OnDestroy();
		break;
	}
	return DefWindowProc(hWnd, Msg, wp, lp);
}

BOOL CALLBACK OptionProc(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp)
{
	switch (Msg)
	{
	case WM_TOOLCHANGE:
		switch (wp)
		{
		case PAN:
			CheckDlgButton(hWnd, IDC_RADIO_PAN, BST_CHECKED);
			break;
		case STRAIGHT:
			CheckDlgButton(hWnd, IDC_RADIO_STRAIGHT, BST_CHECKED);
			break;
		case RECTANGLE:
			CheckDlgButton(hWnd, IDC_RADIO_RECTANGLE, BST_CHECKED);
			break;
		case ELLIPSE:
			CheckDlgButton(hWnd, IDC_RADIO_ELLIPSE, BST_CHECKED);
			break;
		}
		return TRUE;
	case WM_COMMAND:
		HWND hOwner = GetWindow(hWnd, GW_OWNER);
		switch (LOWORD(wp))
		{
		case IDC_RADIO_PAN:
			SendMessage(hOwner, WM_TOOLCHANGE, (WPARAM)PAN, 0);
			break;
		case IDC_RADIO_STRAIGHT:
			SendMessage(hOwner, WM_TOOLCHANGE, (WPARAM)STRAIGHT, 0);
			break;
		case IDC_RADIO_RECTANGLE:
			SendMessage(hOwner, WM_TOOLCHANGE, (WPARAM)RECTANGLE, 0);
			break;
		case IDC_RADIO_ELLIPSE:
			SendMessage(hOwner, WM_TOOLCHANGE, (WPARAM)ELLIPSE, 0);
			break;
		case IDC_BUTTON_SAVE:
			SendMessage(hOwner, WM_SAVE, 0, 0);
			break;
		case IDC_BUTTON_LOAD:
			SendMessage(hOwner, WM_LOAD, 0, 0);
			break;
		case IDCANCEL:
			SendMessage(hOwner, WM_DLGEND, 0, 0);
			DestroyWindow(hWnd);
			break;
		}
		return TRUE;
	}
	return FALSE;
}