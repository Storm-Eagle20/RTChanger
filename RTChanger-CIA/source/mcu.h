#pragma once

#include <3ds.h>


Result mcuInit();
Result mcuExit();
Result mcuReadRegister(u8 reg, void* data, u32 size);
Result mcuWriteRegister(u8 reg, void* data, u32 size);
