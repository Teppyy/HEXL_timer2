// Copyright (C) 2020-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include <benchmark/benchmark.h>

#include <vector>

#include "eltwise/eltwise-cmp-sub-mod-avx512.hpp"
#include "eltwise/eltwise-cmp-sub-mod-internal.hpp"
#include "hexl/eltwise/eltwise-cmp-sub-mod.hpp"
#include "hexl/logging/logging.hpp"
#include "hexl/util/aligned-allocator.hpp"
#include "util/util-internal.hpp"

namespace intel {
namespace hexl {

//=================================================================

// state[0] is the degree
static void BM_EltwiseCmpSubModNative(benchmark::State& state) {  //  NOLINT
  size_t input_size = state.range(0);

  uint64_t modulus = 100;
  uint64_t bound = GenerateInsecureUniformRandomValue(modulus - 1) + 1;
  uint64_t diff = GenerateInsecureUniformRandomValue(modulus - 1) + 1;
  auto input1 = GenerateInsecureUniformRandomValues(input_size, modulus);

  for (auto _ : state) {
    EltwiseCmpSubModNative(input1.data(), input1.data(), input_size, modulus,
                           CMPINT::NLT, bound, diff);
  }
}

BENCHMARK(BM_EltwiseCmpSubModNative)
    ->Unit(benchmark::kMicrosecond)
    ->Args({1024})
    ->Args({4096})
    ->Args({16384});

//=================================================================

#ifdef HEXL_HAS_AVX512DQ
// state[0] is the degree
static void BM_EltwiseCmpSubModAVX512(benchmark::State& state) {  //  NOLINT
  size_t input_size = state.range(0);
  uint64_t modulus = 100;
  uint64_t bound = GenerateInsecureUniformRandomValue(modulus);
  uint64_t diff = GenerateInsecureUniformRandomValue(modulus);
  auto input1 = GenerateInsecureUniformRandomValues(input_size, modulus);

  for (auto _ : state) {
    EltwiseCmpSubModAVX512(input1.data(), input1.data(), input_size, modulus,
                           CMPINT::NLT, bound, diff);
  }
}

BENCHMARK(BM_EltwiseCmpSubModAVX512)
    ->Unit(benchmark::kMicrosecond)
    ->Args({1024})
    ->Args({4096})
    ->Args({16384});
#endif

//=================================================================

}  // namespace hexl
}  // namespace intel
