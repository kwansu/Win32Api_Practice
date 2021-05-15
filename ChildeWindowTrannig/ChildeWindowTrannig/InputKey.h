#ifndef __INPUT_TEXT_H__
#define __INPUT_TEXT_H__

#include "BaseWindow.h"

HRESULT CALLBACK InputKeyProc(HWND, UINT, WPARAM, LPARAM);

/*void InputKeyRegusterWindow(WNDCLASS* InputKeyWnd)
{
	InputKeyWnd->hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	InputKeyWnd->lpfnWndProc = InputKeyProc;
	InputKeyWnd->lpszClassName = TEXT("InputText");

	RegisterClass(InputKeyWnd);
}*/

#endif