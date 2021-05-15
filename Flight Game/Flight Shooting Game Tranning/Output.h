#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include "BaseWnd.h"

class Output_Wnd : public BaseWnd
{
	LPCTSTR lpszClass = TEXT("Output");
public:
	Output_Wnd(HWND);
	HRESULT WndProc(HWND, UINT, WPARAM, LPARAM);

};

#endif