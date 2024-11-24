// Copyright 2024 jeonghun

#include "rdata_patch.h"
#include <cassert>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include "pe_section.h"
#include "codepage.h"
#include "codetable.h"
#include "func_hooker.h"
#include "resource.h"
#include "resource_loader.h"
#include "../third_party/json.hpp"

namespace rdata_patch {

std::unordered_map<uintptr_t, std::vector<uint8_t>> str_table;

#pragma optimize("g", off)
static void modify_str_arg() {
  uint32_t stack_anchor;
  uint32_t *str_arg_ptr = &stack_anchor + 8;

  if (const auto it = str_table.find(*str_arg_ptr); it != str_table.cend()) {
    *str_arg_ptr = reinterpret_cast<uintptr_t>(it->second.data());
  }
}
#pragma optimize("g", on)

constexpr uintptr_t func_addr_drawstring = 0x004d9e10;
constexpr uintptr_t ret_addr =
    func_addr_drawstring + func_hooker::CODE_LENGTH_FOR_32BIT;

static func_hooker drawstring;
__declspec(naked) static void hook_drawstring() {
  __asm {
    push ecx
    call modify_str_arg;
    pop ecx
    mov eax,dword ptr ss:[esp + 4]
    push esi
    jmp ret_addr
  }
}

bool apply(HMODULE hdll) {
  HINSTANCE base = GetModuleHandle(nullptr);
  IMAGE_NT_HEADERS *nt_header_ptr = get_nt_header(base);
  if (nt_header_ptr == nullptr)
    return false;

  IMAGE_SECTION_HEADER *sh_ptr = find_pe_section(nt_header_ptr, ".rdata");
  if (sh_ptr == nullptr)
    return false;

  char *sec_ptr = reinterpret_cast<char *>(
      reinterpret_cast<DWORD_PTR>(base) + sh_ptr->PointerToRawData);

  if (sec_ptr == nullptr)
    return false;

  const uint32_t sec_size = sh_ptr->SizeOfRawData;

  DWORD old_protect = 0;
  if (VirtualProtect(sec_ptr, sec_size, PAGE_READWRITE, &old_protect) == FALSE)
    return false;

  resource_loader rsc(IDR_SAN11WPK_EXE_JSON, hdll);
  auto translate =
      nlohmann::json::parse(rsc.get_ptr(), rsc.get_ptr() + rsc.get_length());

  for (const auto &element : translate) {
    const auto &utf8 = element["kor"].get<std::string>();
    if (utf8.length() > 0) {
      const auto offset = element["offset"].get<uint32_t>();
      const auto size = element["size"].get<uint32_t>();

      uint8_t *game_txt = reinterpret_cast<uint8_t *>(sec_ptr) + offset;
      assert(game_txt != nullptr);

      codepage<> unicode(utf8.c_str());
      const wchar_t *ch = unicode;
      assert(ch != nullptr);

      uint8_t buffer[128] = { 0 };
      uint32_t txt_len = 0;
      for (uint32_t i = 0; i < _countof(buffer); i++) {
        if (*ch != L'\0') {
          if (const auto code = code_table.find(*ch);
              code != code_table.cend()) {
            *reinterpret_cast<uint16_t *>(buffer + i++) = code->second;
          } else if (iswascii(*ch)) {
            *(buffer + i) = static_cast<uint8_t>(*ch);
          } else {
            WideCharToMultiByte(932, 0, ch, 1,
                reinterpret_cast<char *>(buffer + i++), 2, nullptr, nullptr);
          }
        } else {
          *(buffer + i) = '\0';
          txt_len = i;
          break;
        }
        ch++;
      }

      if (txt_len < size) {
        memcpy(game_txt, buffer, txt_len + 1);
      } else {
        str_table.emplace(reinterpret_cast<uintptr_t>(game_txt),
            std::vector<uint8_t>(buffer, buffer + txt_len + 1));
      }
    }
  }
  VirtualProtect(sec_ptr, sec_size, old_protect, &old_protect);

  return drawstring.install(reinterpret_cast<FARPROC>(func_addr_drawstring),
      reinterpret_cast<FARPROC>(hook_drawstring));
}

void undo() { 
  drawstring.unhook();
  str_table.clear();
}

}  // namespace rdata_patch
