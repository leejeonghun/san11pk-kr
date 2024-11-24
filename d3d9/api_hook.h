// Copyright 2024 jeonghun

#ifndef SAN11PK_KR_D3D9_API_HOOK_H_
#define SAN11PK_KR_D3D9_API_HOOK_H_

namespace api_hook {

bool apply(HMODULE hDLL);
void undo();

}  // namespace api_hook

#endif  // SAN11PK_KR_D3D9_API_HOOK_H_
