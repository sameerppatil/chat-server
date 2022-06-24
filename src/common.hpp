#pragma once

#include <plog/Log.h>
#include "plog/Initializers/RollingFileInitializer.h"
#include <cassert>

#define assertm(exp, msg) assert(((void)msg, exp))