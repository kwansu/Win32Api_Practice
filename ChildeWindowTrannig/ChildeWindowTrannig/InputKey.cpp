/*#include "InputKey.h"

HRESULT CALLBACK InputKeyProc(HWND hInputKeyWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	static INT num;
	TCHAR str[10] = { NULL };
	HDC hdc;
	PAINTSTRUCT ps;

	switch (Msg)
	{
	case WM_CREATE:
		CreateWindow(TEXT("edit"), TEXT("¿¡µðÆ®"), WS_CHILD | WS_VISIBLE
			, 100, 100, 100, 30, hInputKeyWnd, (HMENU)1, )
		break;
	case WM_KEYDOWN:
		num++;
		InvalidateRect(hInputKeyWnd, NULL, TRUE);
		break;
	case WM_LBUTTONDOWN:
		num++;
		InvalidateRect(hInputKeyWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hInputKeyWnd, &ps);
		wsprintf(str, TEXT("%d"), num);
		TextOut(hdc, 0, 0, str, lstrlen(str));
		EndPaint(hInputKeyWnd, &ps);
		break;
	default:
		return (DefWindowProc(hInputKeyWnd, Msg, wParam, lParam));
	}
	return 0;
}*/