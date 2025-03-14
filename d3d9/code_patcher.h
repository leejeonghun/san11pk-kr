// Copyright 2016 jeonghun

#ifndef SAN11PK_KR_D3D9_CODE_PATCHER_H_
#define SAN11PK_KR_D3D9_CODE_PATCHER_H_

#include <cstdint>
#include <vector>

class code_patcher final {
 public:
  code_patcher() = default;
  ~code_patcher() = default;

  bool apply(void *addr_ptr, const void *code_ptr, uint32_t length);
  bool undo();

 private:
  std::vector<uint8_t> backup_;
  void *addr_ptr_ = nullptr;
  uint32_t protect_flag_ = 0;
};

#endif  // SAN11PK_KR_D3D9_CODE_PATCHER_H_
