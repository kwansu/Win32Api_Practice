#include <Windows.h>
#include "resource.h"

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);
	
	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

HRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hInst;
LPCTSTR lpszClassName = TEXT("Dodge");
HWND hMainWnd;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	MSG msg;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
	WndClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClassName;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hMainWnd = CreateWindow(lpszClassName, lpszClassName, WS_CAPTION | WS_SYSMENU
		, CW_USEDEFAULT, CW_USEDEFAULT, 400, 400
		, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hMainWnd, nCmdShow);

	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

BOOL bGamming = FALSE;
LPCTSTR str = TEXT("시작하려면 Enter를 눌러주세요.");

struct Plate
{
	int x, y;
};

struct Misile
{
	int x, y;
	int vx, vy;
};

Plate plate;
Misile misile[50];

VOID FrameAction();
VOID StartGame();
VOID GameOver();

HBITMAP hBitPlate;
HBITMAP hBitMisile;

HRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT clientRect;

	switch (msg)
	{
	case WM_CREATE:
		hBitPlate = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_PLATE));
		hBitMisile = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MISILE));
		break;
	case WM_TIMER:
		if (bGamming)
			FrameAction();
		break;
	case WM_KEYDOWN:
		if (wp == VK_RETURN)
		{
			if (!bGamming)
			{
				bGamming = TRUE;
				StartGame();
			}
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (bGamming)
		{
			DrawBitmap(hdc, plate.x, plate.y, hBitPlate);
			for (int i = 0; i < 50; i++)
				DrawBitmap(hdc, misile[i].x, misile[i].y, hBitMisile);
		}
		else
		{
			GetClientRect(hWnd, &clientRect);
			SetTextAlign(hdc, TA_CENTER);
			TextOut(hdc, clientRect.right / 2, clientRect.bottom / 2, str, lstrlen(str));
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
	return 0;
}

VOID StartGame()
{
	int i, r;

	SetTimer(hMainWnd, 1, 20, NULL);
	plate.x = 200;
	plate.y = 200;

	for (i = 0; i < 50; i++)
	{
		r = rand() % 1600;
		if (r < 400)
		{
			misile[i].x = 5;
			misile[i].y = r;
		}
		else if (r < 800)
		{
			misile[i].x = r;
			misile[i].y = 395;
		}
		else if (r < 1200)
		{
			misile[i].x = 395;
			misile[i].y = r;
		}
		else
		{
			misile[i].x = r;
			misile[i].y = 5;
		}

		misile[i].vx = rand() % 51 / 10.0 - 2.5;
		misile[i].vy = rand() % 51 / 10.0 - 2.5;
	}
}

VOID FrameAction()
{
	RECT rt;
	POINT pt;

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		if (plate.x > 7.5)
			plate.x += -2.3;
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		if (plate.x < 392.5)
			plate.x += 2.3;
	}
	if (GetKeyState(VK_UP) & 0x8000)
	{
		if (plate.y > 7.5)
			plate.y += -2.3;
	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		if (plate.y < 392.5)
			plate.y += 2.3;
	}

	rt.bottom = plate.y + 5;
	rt.left = plate.x - 5;
	rt.right = plate.x + 5;
	rt.top = plate.y - 5;

	for (int i = 0; i < 50; i++)
	{
		misile[i].x += misile[i].vx;
		misile[i].y += misile[i].vy;

		if (misile[i].x < 0) misile[i].x += 400;
		if (misile[i].x > 400) misile[i].x -= 400;
		if (misile[i].y < 0) misile[i].y += 400;
		if (misile[i].y > 400) misile[i].y -= 400;
		
		pt.x = misile[i].x;
		pt.y = misile[i].y;

		if (PtInRect(&rt, pt))
		{
			GameOver();
			break;
		}
	}
	InvalidateRect(hMainWnd, NULL, TRUE);
}

VOID GameOver()
{
	KillTimer(hMainWnd, 1);
	bGamming = FALSE;
}