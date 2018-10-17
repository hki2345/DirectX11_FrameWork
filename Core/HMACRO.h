#pragma once
#include <assert.h>

#define TASSERT(VALUE) assert(!(VALUE))

#define BOOM TASSERT(true);