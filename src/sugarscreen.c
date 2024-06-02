#include "sugarcrash.h"

#include <wingdi.h>
#include <math.h>

/* 
 * Alghoritm:
 * https://learn.microsoft.com/en-us/windows/win32/direct2d/hue-rotate
 */
static VOID CalculateHueRotationMatrix(FLOAT fAngle, FLOAT afMatrix[9])
{
    FLOAT fCosValue = cosf(fAngle);
    FLOAT fSinValue = sinf(fAngle);

    /* Red Component */
    afMatrix[0] = 0.213f + fCosValue * 0.787f - fSinValue * 0.213f;
    afMatrix[1] = 0.715f - fCosValue * 0.715f - fSinValue * 0.715f;
    afMatrix[2] = 0.072f - fCosValue * 0.072f + fSinValue * 0.928f;

    /* Green Component */
    afMatrix[3] = 0.213f - fCosValue * 0.213f + fSinValue * 0.143f;
    afMatrix[4] = 0.715f + fCosValue * 0.285f + fSinValue * 0.140f;
    afMatrix[5] = 0.072f - fCosValue * 0.072f - fSinValue * 0.283f;

    /* Blue Component */
    afMatrix[6] = 0.213f - fCosValue * 0.213f - fSinValue * 0.787f;
    afMatrix[7] = 0.715f - fCosValue * 0.715f + fSinValue * 0.715f;
    afMatrix[8] = 0.072f + fCosValue * 0.928f + fSinValue * 0.072f;
}

VOID FILTER HueRotate(
    RGBQUAD        *prgbPixels,
    UINT            uWidth,
    UINT            uHeight,
    LPVOID          lpAngle)
{
    FLOAT       afMatrix[9];
    FLOAT       fRed, fGreen, fBlue;
    SIZE_T      i;

    CalculateHueRotationMatrix(*((FLOAT*) lpAngle), afMatrix);

    for (i = 0; i < (uWidth * uHeight); ++i) {
        fRed                = prgbPixels[i].rgbRed   * afMatrix[0]
                            + prgbPixels[i].rgbGreen * afMatrix[1]
                            + prgbPixels[i].rgbBlue  * afMatrix[2];

        fGreen              = prgbPixels[i].rgbRed   * afMatrix[3]
                            + prgbPixels[i].rgbGreen * afMatrix[4]
                            + prgbPixels[i].rgbBlue  * afMatrix[5];

        fBlue               = prgbPixels[i].rgbRed   * afMatrix[6]
                            + prgbPixels[i].rgbGreen * afMatrix[7]
                            + prgbPixels[i].rgbBlue  * afMatrix[8];
        
        prgbPixels[i].rgbRed    = (BYTE) fRed;
        prgbPixels[i].rgbGreen  = (BYTE) fGreen;
        prgbPixels[i].rgbBlue   = (BYTE) fBlue;
    }
}

VOID Sugar_ScreenPixelFilter(SGRPIXELFILTERPROC filter, LPVOID lpParam)
{
    HDC             hScreenDc = GetDC(NULL);
    RGBQUAD        *prgbPixels = NULL;
    BITMAPINFO      bmi = {0};
    HDC             hDcTemp = NULL;
    HBITMAP         hbmTemp = NULL;
    ULONG           uWidth = GetSystemMetrics(SM_CXSCREEN);
    ULONG           uHeight = GetSystemMetrics(SM_CYSCREEN);
    SIZE_T          i;

    bmi.bmiHeader.biSize        = sizeof bmi;
    bmi.bmiHeader.biWidth       = uWidth;
    bmi.bmiHeader.biHeight      = uHeight;
    bmi.bmiHeader.biBitCount    = 32;
    bmi.bmiHeader.biPlanes      = 1;

    hDcTemp = CreateCompatibleDC(hScreenDc);
    
    hbmTemp = CreateDIBSection(
        hScreenDc, &bmi, 0, (VOID**) &prgbPixels, NULL, 0);

    SelectObject(hDcTemp, hbmTemp);

    BitBlt(hDcTemp, 0, 0, uWidth, uHeight, hScreenDc, 0, 0, SRCCOPY);

    filter(prgbPixels, uWidth, uHeight, lpParam);

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