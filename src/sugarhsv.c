#include "sugarcrash.h"

#include <math.h>

/* https://en.wikipedia.org/wiki/HSL_and_HSV#Color_conversion_formulae */

VOID ConvertRGBtoHSV(const RGBQUAD *rgb, HSVQUAD *hsv)
{
    FLOAT fR, fG, fB;
    FLOAT fCMax, fCMin;
    FLOAT fDelta;

    if (rgb == NULL || hsv == NULL)
        return;

    fR = (FLOAT) rgb->rgbRed / 255.0f;
    fG = (FLOAT) rgb->rgbGreen / 255.0f;
    fB = (FLOAT) rgb->rgbBlue / 255.0f;

    fCMax = max(max(fR, fG), fB);
    fCMin = min(min(fR, fG), fB);
    fDelta = fCMax - fCMin;

    if (fDelta == 0) /* Hue */
        hsv->fHue = 0;
    else if (fCMax == fR)
        hsv->fHue = 60 * (fmod(((fG - fB) / fDelta), 6));
    else if (fCMax == fG)
        hsv->fHue = 60 * (((fB - fR) / fDelta) + 2);
    else if (fCMax == fB)
        hsv->fHue = 60 * (((fR - fG) / fDelta) + 4);

    if (fCMax == 0) /* Saturation */
        hsv->fSaturation = 0;
    else
        hsv->fSaturation = fDelta / fCMax;

    hsv->fValue = fCMax; /* Value */
    
    hsv->fReserved = (FLOAT) rgb->rgbReserved;
}

VOID ConvertHSVtoRGB(const HSVQUAD *hsv, RGBQUAD *rgb)
{
    FLOAT fC, fX, fM, fH, fRs, fGs, fBs;

    if (rgb == NULL || hsv == NULL)
        return;

    fC = hsv->fValue * hsv->fSaturation;
    fH = hsv->fHue / 60.0f;
    fX = fC * (1 - fabs(fmod(fH, 2) - 1));
    fM = hsv->fValue - fC;

    switch ((UINT) fH)
    {
    case 0:
        fRs = fC;
        fGs = fX;
        fBs = 0;
        break;
    
    case 1:
        fRs = fX;
        fGs = fC;
        fBs = 0;
        break;

    case 2:
        fRs = 0;
        fGs = fC;
        fBs = fX;
        break;
    
    case 3:
        fRs = 0;
        fGs = fX;
        fBs = fC;
        break;

    case 4:
        fRs = fX;
        fGs = 0;
        fBs = fC;
        break;
    
    default:
        fRs = fC;
        fGs = 0;
        fBs = fX;
        break;
    }

    rgb->rgbRed = (BYTE) ((fRs + fM) * 255.0f);
    rgb->rgbGreen = (BYTE) ((fGs + fM) * 255.0f);
    rgb->rgbBlue = (BYTE) ((fBs + fM) * 255.0f);

    rgb->rgbReserved = (BYTE) hsv->fReserved;
}