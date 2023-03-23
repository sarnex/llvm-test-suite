// TODO: enable on Windows once driver is ready
// REQUIRES: gpu && linux
// UNSUPPORTED: cuda || hip
//
// Check that full compilation works:
// RUN: !%clangxx -fsycl -fno-sycl-device-code-split-esimd -Xclang -fsycl-allow-func-ptr %s -o %t.out 2>&1 | FileCheck %s

/* Tests invoke_simd support in the compiler/headers
 * The test checks that compiler emits a meaningful and user friendly error message
 * when the target function of invoke_simd has arguments or returns of simd<T,N> or
 * simd_mask<T,N> type and N is not equal to subgroup size.
 */

#include "Inputs/common.hpp"

int main(void) {
  auto q = queue{gpu_selector_v};
  auto dev = q.get_device();
  std::cout << "Running on " << dev.get_info<sycl::info::device::name>()
            << "\n";
  bool passed = true;

  // simd_size 8
  passed &= test<4, 8>(q);
  // CHECK: {{.*}}error:{{.*}}static assertion failed due to requirement 'RetVecLength == 8': invoke_simd callee return type vector length must match kernel subgroup size{{.*}}
  passed &= test<16, 8>(q);
  // CHECK: {{.*}}error:{{.*}}static assertion failed due to requirement 'RetVecLength == 8': invoke_simd callee return type vector length must match kernel subgroup size{{.*}}
  passed &= test<32, 8>(q);
  // CHECK: {{.*}}error:{{.*}}static assertion failed due to requirement 'RetVecLength == 8': invoke_simd callee return type vector length must match kernel subgroup size{{.*}}

  // simd_size 16
  passed &= test<4, 16>(q);
  // CHECK: {{.*}}error:{{.*}}static assertion failed due to requirement 'RetVecLength == 16': invoke_simd callee return type vector length must match kernel subgroup size{{.*}}
  passed &= test<8, 16>(q);
  // CHECK: {{.*}}error:{{.*}}static assertion failed due to requirement 'RetVecLength == 16': invoke_simd callee return type vector length must match kernel subgroup size{{.*}}
  passed &= test<32, 16>(q);
  // CHECK: {{.*}}error:{{.*}}static assertion failed due to requirement 'RetVecLength == 16': invoke_simd callee return type vector length must match kernel subgroup size{{.*}}

  // simd_size 32
  passed &= test<4, 32>(q);
  // CHECK: {{.*}}error:{{.*}}static assertion failed due to requirement 'RetVecLength == 32': invoke_simd callee return type vector length must match kernel subgroup size{{.*}}
  passed &= test<8, 32>(q);
  // CHECK: {{.*}}error:{{.*}}static assertion failed due to requirement 'RetVecLength == 32': invoke_simd callee return type vector length must match kernel subgroup size{{.*}}
  passed &= test<16, 32>(q);
  // CHECK: {{.*}}error:{{.*}}static assertion failed due to requirement 'RetVecLength == 32': invoke_simd callee return type vector length must match kernel subgroup size{{.*}}

  std::cout << (passed ? "Passed\n" : "FAILED\n");
  return passed ? 0 : 1;
}
