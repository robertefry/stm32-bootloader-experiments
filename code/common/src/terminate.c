
#include "common/terminate.h"

static void terminate_handler(char const*)
{
    do {
#ifndef NDEBUG
        __builtin_trap();
#endif
    }
    while (1);
}

static void (*termination_handler)(const char*) = terminate_handler;

void set_termination_handler(void(*handler)(char const*))
{
    termination_handler = handler;
}

void terminate(char const* message)
{
    termination_handler(message);
}
