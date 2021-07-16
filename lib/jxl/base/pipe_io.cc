// Copyright (c) the JPEG XL Project Authors. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "lib/jxl/base/pipe_io.h"

#include <windows.h>
#include <stdio.h>
#include <sys/stat.h>

#include <string>

#include "lib/jxl/base/compiler_specific.h"
#include "lib/jxl/base/padded_bytes.h"
#include "lib/jxl/base/status.h"

namespace jxl {

// Pipe example
// https://www.codeproject.com/Articles/4816/Unleashing-anonymous-pipes-Part-1

uint64_t ExtractHandle(const std::string& pathname) {
  int extIndex = pathname.find('.', 1);
  if (extIndex <= 0) {
    fprintf(stderr, "File extension not found\n");
    return 0;
  }
  std::string handleStr = pathname.substr(1, extIndex - 1);
  fprintf(stderr, handleStr.c_str());
  return atoi(handleStr.c_str());
}

Status ReadFromPipe(const std::string& pathname, void** bytes, int* size) {
  fprintf(stderr, "Reading from pipe ");
  uint64_t handle = ExtractHandle(pathname);
  fprintf(stderr, "\n");
  if (handle == 0) return JXL_FAILURE("Invalid pipe handle");
  DWORD read;
  if (!ReadFile(reinterpret_cast<HANDLE>(handle), size, 4, &read, nullptr))
    return JXL_FAILURE("Failed to read pipe size");
  if (read < 4) return JXL_FAILURE("Read less than expected");
  if (*size == 0) return JXL_FAILURE("Input is empty");

  *bytes = new char[*size];
  if (!ReadFile(reinterpret_cast<HANDLE>(handle), *bytes, *size, &read, nullptr))
    return JXL_FAILURE("Failed to read from pipe");

  fprintf(stderr, "Done reading from pipe\n");
  return true;
}

Status WriteToPipe(void const* bytes, int size, const std::string& pathname) {
  fprintf(stderr, "Writing to pipe ");
  uint64_t handle = ExtractHandle(pathname);
  fprintf(stderr, "\n");
  if (handle == 0) return JXL_FAILURE("Invalid pipe handle");
  DWORD written;
  if (!WriteFile(reinterpret_cast<HANDLE>(handle), &size, 4, &written, nullptr))
    return JXL_FAILURE("Failed to write to pipe");
  if (!WriteFile(reinterpret_cast<HANDLE>(handle), bytes, size, &written, nullptr))
    return JXL_FAILURE("Failed to write to pipe");

  fprintf(stderr, "Done writing to pipe\n");
  return true;
}

}  // namespace jxl