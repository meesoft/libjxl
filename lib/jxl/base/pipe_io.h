// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef LIB_JXL_BASE_PIPE_IO_H_
#define LIB_JXL_BASE_PIPE_IO_H_

// Helper functions for reading/writing pipes.

#include <stdio.h>
#include <sys/stat.h>

#include <string>

#include "lib/jxl/base/compiler_specific.h"
#include "lib/jxl/base/padded_bytes.h"
#include "lib/jxl/base/status.h"

namespace jxl {

Status ReadFromPipe(const std::string& pathname, void** bytes, int* size);

Status WriteToPipe(void const* bytes, int size, const std::string& pathname);

}  // namespace jxl

#endif  // LIB_JXL_BASE_PIPE_IO_H_