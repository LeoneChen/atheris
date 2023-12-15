// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// This is a thin wrapper that can be linked with old versions of libFuzzer to
// add support for the new LLVMFuzzerRunDriver API.

#include <cstdint>

namespace fuzzer {
typedef int (*UserCallback)(const uint8_t *Data, std::size_t Size);
typedef int (*UserCallbackCore)(const char *Script);
typedef const char* (*GetRandomSeed) (const char *Dir);
typedef const char* (*GetSpecifiedSeed) (const char *Seed);
typedef std::size_t (*UserMutator)(uint8_t* data, std::size_t size, std::size_t max_size, unsigned int seed);

int FuzzerDriver(int *argc, char ***argv, UserCallback Callback);
int FuzzerDriverPyCore(int *argc, char ***argv, 
                       UserCallbackCore Callback,
                       GetRandomSeed CbRandom,
                       GetSpecifiedSeed CbSpecified,
                       UserMutator CbUserMutator);
int FuzzerGetCovUpdateDuration();

}  // namespace fuzzer



extern "C" __attribute__((visibility("default"))) int LLVMFuzzerRunDriver(
    int *argc, char ***argv,
    int (*UserCb)(const uint8_t *Data, std::size_t Size)) {
  return fuzzer::FuzzerDriver(argc, argv, UserCb);
}


extern "C" __attribute__((visibility("default"))) int LLVMFuzzerRunDriverPyCore(
    int *argc, char ***argv, 
    int (*UserCb)(const char *Script),
    const char* (*CbRandom) (const char *Dir),
    const char* (*CbSpecified) (const char *Seed),
    std::size_t (*CbUserMutator)(uint8_t* data, std::size_t size, std::size_t max_size, unsigned int seed)) {
  return fuzzer::FuzzerDriverPyCore(argc, argv, UserCb, CbRandom, CbSpecified, CbUserMutator);
}


extern "C" __attribute__((visibility("default"))) int  LLVMFuzzerGetCovUpdateDuration() {
  return fuzzer::FuzzerGetCovUpdateDuration ();
}

