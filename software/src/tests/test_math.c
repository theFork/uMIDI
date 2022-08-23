#include <stdio.h>

#include "lookup_tables.h"
#include "math.h"

struct linear_range conv = {
    .from = 0,
    .to   = (1<<10),
};

int main(void)
{
    init_linear_to_midi(&conv);

    for (uint8_t i = 0; i <= 127; ++i) {
        uint16_t exp = lookup_exp(i);
        midi_value_t midi = linear_to_midi(&conv, exp);
        printf("%4i -> %3i\n", exp, midi);
    }

    return 0;
}
