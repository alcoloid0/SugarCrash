#include "sugarcrash.h"

#include <wingdi.h>
#include <math.h>

VOID FILTER HueRotate(RGBQUAD *prgbPixel, LPVOID lpAngle)
{
    HSVQUAD hsv;

    ConvertRGBtoHSV(prgbPixel, &hsv);

    hsv.fHue += (lpAngle != NULL) ? *((PFLOAT) lpAngle) : 0;

    if (hsv.fHue > 360) hsv.fHue -= 360;
    else if (hsv.fHue < 0) hsv.fHue += 360;
    
    ConvertHSVtoRGB(&hsv, prgbPixel);
}

VOID Sugar_ScreenPixelFilter(SGRPIXELFILTERPROC filter, LPVOID lpParam)
{
    HDC             hScreenDc = GetDC(NULL);
    RGBQUAD        *prgbDest = NULL;
    BITMAPINFO      bmi = {0};
    HDC             hDcTemp = NULL;
    HBITMAP         hbmTemp = NULL;
    ULONG           uWidth, uHeight;
    SIZE_T          i;

    uWidth = GetSystemMetrics(SM_CXSCREEN);
    uHeight = GetSystemMetrics(SM_CYSCREEN);

    bmi.bmiHeader.biSize = sizeof bmi;
    bmi.bmiHeader.biWidth = uWidth;
    bmi.bmiHeader.biHeight = uHeight;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biPlanes = 1;

    hDcTemp = CreateCompatibleDC(hScreenDc);
    
    hbmTemp = CreateDIBSection(
        hScreenDc, &bmi, 0, (VOID**) &prgbDest, NULL, 0);

    SelectObject(hDcTemp, hbmTemp);

    BitBlt(hDcTemp, 0, 0, uWidth, uHeight, hScreenDc, 0, 0, SRCCOPY);

    for (i = 0; i < (uWidth * uHeight); i++)
        filter(&prgbDest[i], lpParam);

    BitBlt(hScreenDc, 0, 0, uWidth, uHeight, hDcTemp, 0, 0, SRCCOPY);

    DeleteObject(hbmTemp);
    DeleteDC(hDcTemp);

    ReleaseDC(NULL, hScreenDc);
}

VOID Sugar_ScreenDrawText(const PPOINT pPoint, LPCTSTR lpText)
{
    RECT    rect = {0};
    HDC     hScreenDc = GetDC(NULL);

    if (pPoint == NULL || lpText == NULL)
        return;

    rect.left = pPoint->x;
    rect.top  = pPoint->y;

    DrawText(hScreenDc, lpText, -1, &rect, DT_NOCLIP | DT_LEFT);

    ReleaseDC(NULL, hScreenDc);
}

VOID Sugar_ScreenGetRandomPoint(PPOINT pPoint)
{
    if (pPoint == NULL)
        return;
    
    pPoint->x = Sugar_Random(0, GetSystemMetrics(SM_CXSCREEN));
    pPoint->y = Sugar_Random(0, GetSystemMetrics(SM_CYSCREEN));
}

DWORD Sugar_ScreenGetFrequency(VOID)
{
    DEVMODE mode = { .dmSize = sizeof mode };
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &mode);
    return mode.dmDisplayFrequency;
}