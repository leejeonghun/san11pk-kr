// Copyright 2024 jeonghun

#include "api_hook.h"
#include <unordered_map>
#include "codepage.h"
#include "codetable.h"
#include "iat_hooker.h"
#include "code_patcher.h"
#include "rdata_patch.h"

namespace api_hook {

HMODULE dll_handle = NULL;
code_patcher ime_patch;

iat_hooker createwindowexa;
HWND WINAPI hook_createwindowexa(DWORD dwExStyle, LPCSTR lpClassName,
    LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight,
    HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) {
  HWND hwnd = createwindowexa.call_origin<decltype(&CreateWindowExA)>(dwExStyle,
      lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent,
      hMenu, hInstance, lpParam);

  if (reinterpret_cast<uintptr_t>(lpClassName) == 0x0078f008) {
    SetWindowTextW(hwnd, L"삼국지11 with 파워업키트");

    void* addr_char_modify = reinterpret_cast<void*>(0x004df715);
    const uint8_t nop[] = { 0x90, 0x90, 0x90, 0x90, 0x90 };
    ime_patch.apply(addr_char_modify, nop, sizeof(nop));

    rdata_patch::apply(dll_handle);
  }

  return hwnd;
}

iat_hooker createfilea;
HANDLE WINAPI hook_createfilea(LPCSTR lpFileName, DWORD dwDesiredAccess,
    DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile) {
  return CreateFileW(codepage<932>(lpFileName), dwDesiredAccess, dwShareMode,
      lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes,
      hTemplateFile);
}

iat_hooker createdirectorya;
BOOL WINAPI hook_createdirectorya(
    LPCSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes) {
  return CreateDirectoryW(codepage<932>(lpPathName), lpSecurityAttributes);
}

iat_hooker getfileattributesa;
DWORD WINAPI hook_getfileattributesa(LPCSTR lpFileName) {
  return GetFileAttributesW(codepage<932>(lpFileName));
}

iat_hooker getfullpathnamea;
DWORD WINAPI hook_getfullpathnamea(
    LPCSTR lpFileName, DWORD nBufferLength, LPSTR lpBuffer, LPSTR* lpFilePart) {
  wchar_t buffer[MAX_PATH] = { 0 };
  LPWSTR file_part = nullptr;
  DWORD ret = GetFullPathNameW(
      codepage<932>(lpFileName), _countof(buffer), buffer, &file_part);
  if (ret) {
    strcpy_s(lpBuffer, nBufferLength, codepage<>(buffer).to_cp932());
    if (lpFilePart != nullptr) {
      char* findsep = strrchr(lpBuffer, '\\');
      if (findsep != nullptr) {
        *lpFilePart = findsep + 1;
      }
    }
  }
  return ret;
}

iat_hooker findfirstfilea;
HANDLE WINAPI hook_findfirstfilea(
    LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData) {
  WIN32_FIND_DATAW ffd = { 0 };
  HANDLE ret = FindFirstFileW(codepage<932>(lpFileName), &ffd);
  memcpy(lpFindFileData, &ffd, offsetof(WIN32_FIND_DATAW, cFileName));
  strcpy_s(lpFindFileData->cFileName, codepage<>(ffd.cFileName).to_cp932());
  strcpy_s(lpFindFileData->cAlternateFileName,
      codepage<>(ffd.cAlternateFileName).to_cp932());
  return ret;
}

iat_hooker deletefilea;
BOOL WINAPI hook_deletefilea(LPCSTR lpFileName) {
  return DeleteFileW(codepage<932>(lpFileName));
}

iat_hooker immgetcompositionstringa;
LONG WINAPI hook_immgetcompositionstringa(HIMC himc, DWORD flag, LPVOID lpBuf, DWORD dwBufLen) {
  LONG result = immgetcompositionstringa.call_origin<decltype(&ImmGetCompositionStringA)>(
      himc, flag, lpBuf, dwBufLen);
  if (result == 2 && dwBufLen >= 2) {
    const codepage<949> unicode(reinterpret_cast<char*>(lpBuf));
    const auto iter = code_table.find(*static_cast<const wchar_t*>(unicode));
    if (iter != code_table.cend()) {
      memcpy(lpBuf, &iter->second, sizeof(iter->second));
    }
  }
  return result;
}

bool apply(HMODULE hDLL) {
  dll_handle = hDLL;
  HINSTANCE hinst = GetModuleHandle(nullptr);
  return 
    createwindowexa.hook(hinst, "user32.dll", "CreateWindowExA", hook_createwindowexa) &&
    createfilea.hook(hinst, "kernel32.dll", "CreateFileA", hook_createfilea) &&
    createdirectorya.hook(hinst, "kernel32.dll", "CreateDirectoryA", hook_createdirectorya) &&
    getfullpathnamea.hook(hinst, "kernel32.dll", "GetFullPathNameA", hook_getfullpathnamea) &&
    findfirstfilea.hook(hinst, "kernel32.dll", "FindFirstFileA", hook_findfirstfilea) &&
    getfileattributesa.hook(hinst, "kernel32.dll", "GetFileAttributesA", hook_getfileattributesa) &&
    deletefilea.hook(hinst, "kernel32.dll", "DeleteFileA", hook_deletefilea) &&
    immgetcompositionstringa.hook(hinst, "imm32.dll", "ImmGetCompositionStringA", hook_immgetcompositionstringa);
}

void undo() {
  ime_patch.undo();
  rdata_patch::undo();
  createwindowexa.unhook();
  createfilea.unhook();
  createdirectorya.unhook();
  getfullpathnamea.unhook();
  findfirstfilea.unhook();
  getfileattributesa.unhook();
  deletefilea.unhook();
  immgetcompositionstringa.unhook();
}

}  // namespace api_hook
