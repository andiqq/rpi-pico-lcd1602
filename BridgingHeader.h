//===----------------------------------------------------------------------===//
//
// This source file is part of the Swift open source project
//
// Copyright (c) 2023 Apple Inc. and the Swift project authors.
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://swift.org/LICENSE.txt for license information
//
//===----------------------------------------------------------------------===//

#pragma once

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"

static inline i2c_inst_t* get_default_i2c(void) {
    return PICO_DEFAULT_I2C_INSTANCE();
}