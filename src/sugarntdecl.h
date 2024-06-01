#ifndef _SUGAR_NTDECL
#define _SUGAR_NTDECL

#include <Windows.h>

#define SE_SHUTDOWN_PRIVILEGE 19L

/********************************************************
 * Подробнее:                                           *
 * http://undocumented.ntinternals.net/index.html       *
 ********************************************************/

/* https://github.com/reactos/reactos/blob/master/sdk/include/reactos/wine/winternl.h#L2188 */
typedef enum _HARDERROR_RESPONSE_OPTION {
    OptionAbortRetryIgnore,
    OptionOk,
    OptionOkCancel,
    OptionRetryCancel,
    OptionYesNo,
    OptionYesNoCancel,
    OptionShutdownSystem
} HARDERROR_RESPONSE_OPTION, *PHARDERROR_RESPONSE_OPTION;

/* https://github.com/reactos/reactos/blob/master/sdk/include/ndk/extypes.h#L210 */
typedef enum _HARDERROR_RESPONSE {
    ResponseReturnToCaller,
    ResponseNotHandled,
    ResponseAbort,
    ResponseCancel,
    ResponseIgnore,
    ResponseNo,
    ResponseOk,
    ResponseRetry,
    ResponseYes,
    ResponseTryAgain,
    ResponseContinue
} HARDERROR_RESPONSE, *PHARDERROR_RESPONSE;

/* https://github.com/reactos/reactos/blob/master/sdk/include/reactos/wine/winternl.h#L2625 */
extern NTSYSAPI NTSTATUS NTAPI RtlAdjustPrivilege(
    ULONG       uPrivilege,
    BOOLEAN     bEnable,
    BOOLEAN     bCurrentThread,
    PBOOLEAN    pbEnabled);

/* https://github.com/reactos/reactos/blob/master/sdk/include/reactos/wine/winternl.h#L2522 */
extern NTSYSAPI NTSTATUS NTAPI NtRaiseHardError(
    NTSTATUS                    ntErrorStatus,
    ULONG                       uNumberOfParameters,
    ULONG                       uUnicodeStringParameterMask,
    PVOID                      *ppParameters,
    HARDERROR_RESPONSE_OPTION   ValidResponseOptions,
    PHARDERROR_RESPONSE         pResponse);

#endif /* _SUGAR_NTDECL */