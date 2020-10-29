#pragma once

#include <cstdint>

// These offsets have to be reverse engineered by hand. These might or
// might not be the same over different applications and Direct3D9 versions.
const intptr_t offsetPresent = 0x44;
const intptr_t offsetBeginScene = 0xA4;
const intptr_t offsetEndScene = 0xA8;
const intptr_t offsetClear = 0xAC;
