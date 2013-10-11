#include <windows.h>
#include <tchar.h>
#include <stdio.h>

HINSTANCE hLThis = 0;
HINSTANCE hL = 0;


#pragma comment(linker, "/export:GetHSteamPipe=steam_api_orig.GetHSteamPipe,@1")
#pragma comment(linker, "/export:GetHSteamUser=steam_api_orig.GetHSteamUser,@2")
#pragma comment(linker, "/export:SteamAPI_GetHSteamPipe=steam_api_orig.SteamAPI_GetHSteamPipe,@3")
#pragma comment(linker, "/export:SteamAPI_GetHSteamUser=steam_api_orig.SteamAPI_GetHSteamUser,@4")
#pragma comment(linker, "/export:SteamAPI_GetSteamInstallPath=steam_api_orig.SteamAPI_GetSteamInstallPath,@5")
#pragma comment(linker, "/export:SteamAPI_Init=steam_api_orig.SteamAPI_Init,@6")
#pragma comment(linker, "/export:SteamAPI_InitSafe=steam_api_orig.SteamAPI_InitSafe,@7")
#pragma comment(linker, "/export:SteamAPI_IsSteamRunning=steam_api_orig.SteamAPI_IsSteamRunning,@8")
#pragma comment(linker, "/export:SteamAPI_RegisterCallResult=steam_api_orig.SteamAPI_RegisterCallResult,@9")
#pragma comment(linker, "/export:SteamAPI_RegisterCallback=steam_api_orig.SteamAPI_RegisterCallback,@10")
#pragma comment(linker, "/export:SteamAPI_RestartAppIfNecessary=steam_api_orig.SteamAPI_RestartAppIfNecessary,@11")
#pragma comment(linker, "/export:SteamAPI_RunCallbacks=steam_api_orig.SteamAPI_RunCallbacks,@12")
#pragma comment(linker, "/export:SteamAPI_SetBreakpadAppID=steam_api_orig.SteamAPI_SetBreakpadAppID,@13")
#pragma comment(linker, "/export:SteamAPI_SetMiniDumpComment=steam_api_orig.SteamAPI_SetMiniDumpComment,@14")
#pragma comment(linker, "/export:SteamAPI_SetTryCatchCallbacks=steam_api_orig.SteamAPI_SetTryCatchCallbacks,@15")
#pragma comment(linker, "/export:SteamAPI_Shutdown=steam_api_orig.SteamAPI_Shutdown,@16")
#pragma comment(linker, "/export:SteamAPI_UnregisterCallResult=steam_api_orig.SteamAPI_UnregisterCallResult,@17")
#pragma comment(linker, "/export:SteamAPI_UnregisterCallback=steam_api_orig.SteamAPI_UnregisterCallback,@18")
#pragma comment(linker, "/export:SteamAPI_UseBreakpadCrashHandler=steam_api_orig.SteamAPI_UseBreakpadCrashHandler,@19")
#pragma comment(linker, "/export:SteamAPI_WriteMiniDump=steam_api_orig.SteamAPI_WriteMiniDump,@20")
#pragma comment(linker, "/export:SteamApps=steam_api_orig.SteamApps,@21")
#pragma comment(linker, "/export:SteamClient=steam_api_orig.SteamClient,@22")
#pragma comment(linker, "/export:SteamContentServer=steam_api_orig.SteamContentServer,@23")
#pragma comment(linker, "/export:SteamContentServerUtils=steam_api_orig.SteamContentServerUtils,@24")
#pragma comment(linker, "/export:SteamContentServer_Init=steam_api_orig.SteamContentServer_Init,@25")
#pragma comment(linker, "/export:SteamContentServer_RunCallbacks=steam_api_orig.SteamContentServer_RunCallbacks,@26")
#pragma comment(linker, "/export:SteamContentServer_Shutdown=steam_api_orig.SteamContentServer_Shutdown,@27")
#pragma comment(linker, "/export:SteamFriends=steam_api_orig.SteamFriends,@28")
#pragma comment(linker, "/export:SteamGameServer=steam_api_orig.SteamGameServer,@29")
#pragma comment(linker, "/export:SteamGameServerApps=steam_api_orig.SteamGameServerApps,@30")
#pragma comment(linker, "/export:SteamGameServerNetworking=steam_api_orig.SteamGameServerNetworking,@31")
#pragma comment(linker, "/export:SteamGameServerStats=steam_api_orig.SteamGameServerStats,@32")
#pragma comment(linker, "/export:SteamGameServerUtils=steam_api_orig.SteamGameServerUtils,@33")
#pragma comment(linker, "/export:SteamGameServer_BSecure=steam_api_orig.SteamGameServer_BSecure,@34")
#pragma comment(linker, "/export:SteamGameServer_GetHSteamPipe=steam_api_orig.SteamGameServer_GetHSteamPipe,@35")
#pragma comment(linker, "/export:SteamGameServer_GetHSteamUser=steam_api_orig.SteamGameServer_GetHSteamUser,@36")
#pragma comment(linker, "/export:SteamGameServer_GetIPCCallCount=steam_api_orig.SteamGameServer_GetIPCCallCount,@37")
#pragma comment(linker, "/export:SteamGameServer_GetSteamID=steam_api_orig.SteamGameServer_GetSteamID,@38")
#pragma comment(linker, "/export:SteamGameServer_Init=steam_api_orig.SteamGameServer_Init,@39")
#pragma comment(linker, "/export:SteamGameServer_InitSafe=steam_api_orig.SteamGameServer_InitSafe,@40")
#pragma comment(linker, "/export:SteamGameServer_RunCallbacks=steam_api_orig.SteamGameServer_RunCallbacks,@41")
#pragma comment(linker, "/export:SteamGameServer_Shutdown=steam_api_orig.SteamGameServer_Shutdown,@42")
#pragma comment(linker, "/export:SteamHTTP=steam_api_orig.SteamHTTP,@43")
#pragma comment(linker, "/export:SteamMasterServerUpdater=steam_api_orig.SteamMasterServerUpdater,@44")
#pragma comment(linker, "/export:SteamMatchmaking=steam_api_orig.SteamMatchmaking,@45")
#pragma comment(linker, "/export:SteamMatchmakingServers=steam_api_orig.SteamMatchmakingServers,@46")
#pragma comment(linker, "/export:SteamNetworking=steam_api_orig.SteamNetworking,@47")
#pragma comment(linker, "/export:SteamRemoteStorage=steam_api_orig.SteamRemoteStorage,@48")
#pragma comment(linker, "/export:SteamScreenshots=steam_api_orig.SteamScreenshots,@49")
#pragma comment(linker, "/export:SteamUser=steam_api_orig.SteamUser,@50")
#pragma comment(linker, "/export:SteamUserStats=steam_api_orig.SteamUserStats,@51")
#pragma comment(linker, "/export:SteamUtils=steam_api_orig.SteamUtils,@52")
#pragma comment(linker, "/export:Steam_GetHSteamUserCurrent=steam_api_orig.Steam_GetHSteamUserCurrent,@53")
#pragma comment(linker, "/export:Steam_RegisterInterfaceFuncs=steam_api_orig.Steam_RegisterInterfaceFuncs,@54")
#pragma comment(linker, "/export:Steam_RunCallbacks=steam_api_orig.Steam_RunCallbacks,@55")
#pragma comment(linker, "/export:g_pSteamClientGameServer=steam_api_orig.g_pSteamClientGameServer,@56")


BOOL WINAPI DllMain(HINSTANCE hInst,DWORD reason,LPVOID)
{
  if (reason == DLL_PROCESS_ATTACH) {
		hLThis = hInst;
    hL = LoadLibrary(_T("steam_api_orig.dll"));
		if (!hL) return false;

    char dllPath[MAX_PATH];
    FILE *hintFile = fopen("mo_path.txt", "r");
    int pathLen = 0;
    if (hintFile != NULL) {
      memset(dllPath, L'\0', MAX_PATH);
      pathLen = fread(dllPath, 1, MAX_PATH, hintFile);
      fclose(hintFile);
    } // the else case will most likely lead to an error, but lets try anyway
    _snprintf(dllPath + pathLen, MAX_PATH - pathLen, "\\hook.dll");

    HINSTANCE hookdll = ::LoadLibrary(dllPath);
    if (!hookdll) {
      return false;
    }
  } else if (reason == DLL_PROCESS_DETACH) {
    FreeLibrary(hL);
  }

  return 1;
}
