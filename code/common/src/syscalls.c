
#include <libopencm3/cm3/cortex.h>

#include <errno.h>

[[noreturn]] void _exit(int status)
{
    (void)status;

    cm_disable_interrupts();

    // TODO: Blink the LED to indicate an error condition.
    while (1) {
#ifndef NDEBUG
        __builtin_trap();
#endif
    }
}

int _kill(int pid, int sig)
{
    (void)pid;
    (void)sig;
    errno = EINVAL;
    return -1;
}

int _getpid(void)
{
    return 1;
}
