// Copyright 2024 jeonghun

#ifndef SAN11PK_KR_D3D9_RDATA_PATCH_H_
#define SAN11PK_KR_D3D9_RDATA_PATCH_H_

#include <windows.h>

namespace rdata_patch {

bool apply(HMODULE hdll);
void undo();

}  // namespace rdata_patch

#endif  // SAN11PK_KR_D3D9_RDATA_PATCH_H_
