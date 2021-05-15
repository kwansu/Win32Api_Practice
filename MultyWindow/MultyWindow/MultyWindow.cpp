#include "MultyWindow.h"

Wnd_MultyWindow::Wnd_MultyWindow(int x, int y, int nWidth, int nHeigh)
{
	WndClass.lpszClassName = class_name;
	//WndClass.hbrBackground = NULL;
	RegisterClass(&WndClass);
	
	hWnd = CreateWindow(class_name, class_name, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN
		, x, y, nWidth, nHeigh, NULL, (HMENU)NULL, hInstance, (LPVOID)this);

	GetClientRect(hWnd, &rect);

	len = 1;
	childs = new Wnd_ChildWindow*[len];
	childs[0] = new Wnd_ChildWindow(hWnd, 0, rect.left, rect.top, rect.right, rect.bottom);
	pStartChild = childs[0];
}
HRESULT Wnd_MultyWindow::WndProc(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp)
{
	switch (Msg)
	{
	case WM_CREATE:
		break;
	case WM_KEYDOWN:
		if (wp == VK_RETURN)
			AddChild();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, Msg, wp, lp);
	}
	return 0;
}
INT Wnd_MultyWindow::AddChild()
{
	if (len == 0)
	{
		len++;
		childs = new Wnd_ChildWindow*[len];
		childs[0] = new Wnd_ChildWindow(hWnd, 0);
		childs[0]->WindowInitialize(pStartChild);
		pStartChild = childs[0];
		return len;
	}
	Wnd_ChildWindow** tem = new Wnd_ChildWindow*[len];
	for (int n = 0; n < len; n++)
		tem[n] = childs[n];

	delete[] childs;
	childs = new Wnd_ChildWindow*[++len];

	for (int n = 0; n < len - 1; n++)
		childs[n] = tem[n];

	childs[len - 1] = new Wnd_ChildWindow(hWnd, len - 1);
	childs[len - 1]->WindowInitialize(pStartChild);
	pStartChild = childs[len - 1];
	return len;
}