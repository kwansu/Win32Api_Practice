#ifdef EXPORT_MYDLL
#define MYDLLTYPE __declspec(dllexport)
#else
#define MYDLLTYPE __declspec(dllimport)
#endif
#include <Windows.h>

extern "C" MYDLLTYPE void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);