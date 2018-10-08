#pragma once
#include <assert.h>

#define KASSERT(VALUE) assert(!(VALUE))
#define NEW_DELETE(VALUE)  if (VALUE != nullptr) { delete VALUE; VALUE = nullptr; }
#define KUINT unsigned int