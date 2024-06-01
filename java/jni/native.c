#include "omg_alcoloid_sugarcrash_SugarCrash.h"

#include "sugarcrash.h"

static DWORD THREAD SugarThreadProc(LPVOID lpParam)
{
    return Sugar_Crash();
}

JNIEXPORT void JNICALL
Java_omg_alcoloid_sugarcrash_SugarCrash_sugarCrash(JNIEnv *pEnv, jclass jClz)
{
    CreateThread(NULL, 0, &SugarThreadProc, NULL, 0, NULL);
}