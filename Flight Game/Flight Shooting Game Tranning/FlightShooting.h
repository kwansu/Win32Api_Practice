#ifndef __FLIGHT_SHOOTING_H__
#define __FLIGHT_SHOOTING_H__

#include "BaseWnd.h"
#include "Output.h"
#include "Objects.h"

class Flight_Shooting_Wnd : public BaseWnd
{
	LPCTSTR lpszClass = TEXT("FlightShooting");
	Output_Wnd *pChild;
	RECT rect;
	HDC memDC;

	BOOL bGame;
	Player_AirPlane player;
	HBITMAP hBit_PlayerAirplane, hOldBit;
public:
	Flight_Shooting_Wnd();
	HRESULT WndProc(HWND, UINT, WPARAM, LPARAM);
	HRESULT OnKeyDown(WPARAM, LPARAM);
	HRESULT OnTimer(WPARAM, LPARAM);
	HRESULT OnPaint();
	HRESULT OnCreate(HWND);
	HRESULT OnDestroy();
};

#endif