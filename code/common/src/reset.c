
#include "common/reset.h"

#include <libopencm3/cm3/vector.h>
#include <libopencm3/cm3/scb.h>

#include <stddef.h>

void reset(size_t address)
{
    vector_table_t* table = (vector_table_t*)address;
    // TODO: Reset the state of the chip, and disable interrupts, before jumping.

    SCB_VTOR = (uint32_t)table;
    table->reset();
}
