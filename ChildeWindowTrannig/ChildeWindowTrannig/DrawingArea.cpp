#include "DrawingArea.h"

HRESULT CALLBACK DrawingProc(HWND hDrawingWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	static INT num;
	TCHAR str[10] = { NULL };
	HDC hdc;
	PAINTSTRUCT ps;

	switch (Msg)
	{
	case WM_CREATE:
		break;
	case WM_KEYDOWN:
		num++;
		InvalidateRect(hDrawingWnd, NULL, TRUE);
		break;
	case WM_LBUTTONDOWN:
		num++;
		InvalidateRect(hDrawingWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hDrawingWnd, &ps);
		wsprintf(str, TEXT("%d"), num);
		TextOut(hdc, 0, 0, str, lstrlen(str));
		EndPaint(hDrawingWnd, &ps);
		break;
	default:
		return (DefWindowProc(hDrawingWnd, Msg, wParam, lParam));
	}
	return 0;
}