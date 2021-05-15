#include "ToolBox.h"

HRESULT CALLBACK ToolBoxProc(HWND hToolBoxWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
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
		InvalidateRect(hToolBoxWnd, NULL, TRUE);
		break;
	case WM_LBUTTONDOWN:
		num++;
		InvalidateRect(hToolBoxWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hToolBoxWnd, &ps);
		wsprintf(str, TEXT("%d"), num);
		TextOut(hdc, 0, 0, str, lstrlen(str));
		EndPaint(hToolBoxWnd, &ps);
		break;
	default:
		return (DefWindowProc(hToolBoxWnd, Msg, wParam, lParam));
	}
	return 0;
}