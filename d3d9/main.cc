// Copyright 2024 jeonghun

#include <windows.h>
#include <d3d9.h>
#include "api_hook.h"
#include "dll_loader.h"

static dll_loader d3d9(L"d3d9", true);
IDirect3D9 *WINAPI Direct3DCreate9(UINT SDKVersion) {
  return d3d9.call<decltype(&Direct3DCreate9)>("Direct3DCreate9", SDKVersion);
}

static bool check_launcher_instance() {
  wchar_t filename[MAX_PATH] = { 0 };
  GetModuleFileName(NULL, filename, _countof(filename));
  return _wcsicmp(PathFindFileName(filename), L"San11WPK_Launcher.exe") == 0;
}

int APIENTRY DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved) {
  switch (Reason) {
    case DLL_PROCESS_ATTACH:
      DisableThreadLibraryCalls(hDLL);
      if (check_launcher_instance() == false)
        api_hook::apply(hDLL);
      break;

    case DLL_PROCESS_DETACH:
      api_hook::undo();
      break;
  }

  return TRUE;
}
