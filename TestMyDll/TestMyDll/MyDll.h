#ifdef EXPORT_MYDLL
#define MYDLLTYPE __declspec(dllexport)
#else
#define MYDLLTYPE __declspec(dllimport)
#endif
#include <Windows.h>

extern "C" MYDLLTYPE BOOL RectInRect(const RECT*, const RECT*);