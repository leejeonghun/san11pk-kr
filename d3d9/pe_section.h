// Copyright 2017 jeonghun

#ifndef SAN11PK_KR_D3D9_PE_SECTION_H_
#define SAN11PK_KR_D3D9_PE_SECTION_H_

#include <windows.h>

IMAGE_NT_HEADERS* get_nt_header(HINSTANCE hinst);
IMAGE_SECTION_HEADER* find_pe_section(IMAGE_NT_HEADERS *nthdrs, const char *sec_name);

#endif  // SAN11PK_KR_D3D9_PE_SECTION_H_
