#include "FlightShooting.h"
#include "resource.h"

Flight_Shooting_Wnd::Flight_Shooting_Wnd()
{
	WndClass.lpszClassName = lpszClass;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_CAPTION | WS_CLIPCHILDREN
		, CW_USEDEFAULT, CW_USEDEFAULT, 600, 600
		, NULL, (HMENU)NULL, hInstance, static_cast<LPVOID>(this));
}
HRESULT Flight_Shooting_Wnd::WndProc(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp)
{
	switch (Msg)
	{
	case WM_CREATE:
		OnCreate(hWnd);
		break;
	case WM_KEYDOWN:
		OnKeyDown(wp, lp);
		break;
	case WM_TIMER:
		OnTimer(wp, lp);
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
HRESULT Flight_Shooting_Wnd::OnDestroy()
{
	if (bGame)
		KillTimer(hWnd, 1);
	
	DeleteObject(hBit_PlayerAirplane);
	PostQuitMessage(0);
	return 0;
}
HRESULT Flight_Shooting_Wnd::OnCreate(HWND hWnd)
{
	pChild = new Output_Wnd(hWnd);
	SetRect(&rect, 0, 0, 600, 600);
	AdjustWindowRect(&rect, WS_CAPTION, FALSE);
	SetWindowPos(hWnd, NULL, 0, 0, rect.right - rect.left
		, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER);

	bGame = FALSE;
	player.x = 200;
	player.y = 400;
	player.frame_d = 10;

	hBit_PlayerAirplane = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_PLAYER_AIRPLANE));
	return 0;
}
HRESULT Flight_Shooting_Wnd::OnKeyDown(WPARAM wp, LPARAM lp)
{
	switch (wp)
	{
	case VK_RETURN:
		if (!bGame)
		{
			bGame = TRUE;
			SetTimer(hWnd, 1, 20, NULL);
		}
		break;
	}
	return 0;
}
HRESULT Flight_Shooting_Wnd::OnPaint()
{
	hdc = BeginPaint(hWnd, &ps);
	if (bGame)
	{
		memDC = CreateCompatibleDC(hdc);
		hOldBit = (HBITMAP)SelectObject(memDC, hBit_PlayerAirplane);
		BitBlt(hdc, player.x - 10, player.y - 10, 21, 20
			, memDC, 0, 0, SRCCOPY);
		SelectObject(memDC, hOldBit);
		DeleteDC(memDC);
	}
	else
	{
		SetRect(&rect, 0, 0, 400, 600);
		DrawText(hdc, TEXT("Press ENTER To Start!"), -1
			, &rect, DT_CENTER | DT_VCENTER);
	}
	EndPaint(hWnd, &ps);
	return 0;
}
HRESULT Flight_Shooting_Wnd::OnTimer(WPARAM wp, LPARAM lp)
{
	switch (wp)
	{
	case 1:
		if (GetKeyState(VK_LEFT) & 0x8000)
			if (player.x > 10 + player.frame_d)
				player.x -= player.frame_d;

		if (GetKeyState(VK_RIGHT) & 0x8000)
			if (player.x < 390 - player.frame_d)
				player.x += player.frame_d;

		if (GetKeyState(VK_UP) & 0x8000)
			if (player.y > 10 + player.frame_d)
				player.y -= player.frame_d;

		if (GetKeyState(VK_DOWN) & 0x8000)
			if (player.y < 590 - player.frame_d)
				player.y += player.frame_d;

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	return 0;
}