
#ifndef COMMON_DEBUG_H
#define COMMON_DEBUG_H

#include "terminate.h"

#ifndef NDEBUG
#define DEBUG_ASSERT(cond,...) \
    do { if (!(bool)(cond)) { terminate(__VA_ARGS__); } } while (0)
#else
#define DEBUG_ASSERT(cond,...)
#endif

#endif /* COMMON_DEBUG_H */
