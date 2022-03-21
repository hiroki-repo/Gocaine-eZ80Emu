#include "interrupt.h"
#include "emu.h"
#include "cpu.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

interrupt_state_t intrpt[2];

void intrpt_pulse(uint32_t mask) {
    intrpt_set(mask, true);
    intrpt_set(mask, false);
}

void intrpt_set(uint32_t mask, bool set) {
    size_t request;
    assert(!(mask & (mask - 1)) && "Expected no more than one bit set");
    mask &= 0x3FFFFF;
    for (request = 0; request < sizeof(intrpt) / sizeof(*intrpt); request++) {
        if (set ^ (intrpt[request].inverted & mask)) {
            intrpt[request].status |= mask;
        } else {
            intrpt[request].status &= ~mask | intrpt[request].latched;
        }
    }
}

void intrpt_reset() {
    memset(&intrpt, 0, sizeof(intrpt));
    intrpt_set(INT_PWR, true);
}

const uint32_t revision = 0x00010900;
static uint8_t intrpt_read(uint16_t pio, bool peek) {
    uint16_t index = pio >> 2 & 0x3F;
    uint8_t request = pio >> 5 & 1;
    uint8_t bit_offset = (pio & 3) << 3;

    uint8_t value = 0;

    (void)peek;

    switch(index) {
        case 0:
        case 8:
            value = read8(intrpt[request].status, bit_offset);
            break;
        case 1:
        case 9:
            value = read8(intrpt[request].enabled, bit_offset);
            break;
        case 3:
        case 11:
            value = read8(intrpt[request].latched, bit_offset);
            break;
        case 4:
        case 12:
            value = read8(intrpt[request].inverted, bit_offset);
            break;
        case 5:
        case 13:
            value = read8(intrpt[request].status & intrpt[request].enabled, bit_offset);
            break;
        case 20:
            value = read8(revision, bit_offset);
            break;
        case 21:
            value = (bit_offset & 16) ? 0 : 22;
            break;
        default:
            break;
    }
    return value;
}

static void intrpt_write(uint16_t pio, uint8_t value, bool poke) {
    uint16_t index = pio >> 2 & 0x3F;
    uint8_t request = pio >> 5 & 1;
    uint8_t bit_offset = (pio & 3) << 3;

    switch(index) {
        case 0:
        case 8:
            if (poke) {
                write8(intrpt[request].status, bit_offset, value);
            }
            break;
        case 1:
        case 9:
            write8(intrpt[request].enabled, bit_offset, value);
            break;
        case 2:
        case 10:
            intrpt[request].status &= ~(((uint32_t)value << bit_offset) & intrpt[request].latched);
            break;
        case 3:
        case 11:
            write8(intrpt[request].latched, bit_offset, value);
            break;
        case 4:
        case 12:
            write8(intrpt[request].inverted, bit_offset, value);
            break;
        case 5:
        case 13:
            if (poke) {
                intrpt[request].enabled |= value << bit_offset;
                intrpt[request].status |= value << bit_offset;
            }
            break;
    }
}

static const eZ80portrange_t device = {
    .read  = intrpt_read,
    .write = intrpt_write
};

eZ80portrange_t init_intrpt(void) {
    //gui_console_printf("[CEmu] Initialized Interrupt Controller...\n");
    return device;
}

bool intrpt_save(FILE *image) {
    bool ret = false;
    size_t request;
    for (request = 0; request < sizeof(intrpt) / sizeof(*intrpt); request++) {
        ret |= fwrite(&intrpt[request], sizeof(intrpt[request]), 1, image) == 1;
    }
    return ret;
}

bool intrpt_restore(FILE *image) {
    bool ret = false;
    size_t request;
    for (request = 0; request < sizeof(intrpt) / sizeof(*intrpt); request++) {
        ret |= fread(&intrpt[request], sizeof(intrpt[request]), 1, image) == 1;
    }
    return ret;
}
