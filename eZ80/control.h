#ifndef CONTROL_H
#define CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "port.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

enum {
    BATTERY_DISCHARGED,
    BATTERY_0,
    BATTERY_1,
    BATTERY_2,
    BATTERY_3,
    BATTERY_4
};

typedef struct control_state {
    uint8_t ports[0x80];
    uint8_t cpuSpeed;
    bool usbSelfPowered;
    bool usbBusPowered;

    uint8_t setBatteryStatus;
    uint8_t readBatteryStatus;
    bool batteryCharging;

    uint32_t privileged;
    uint32_t stackLimit;
    uint32_t protectedStart;
    uint32_t protectedEnd;
    uint8_t protectionStatus;
    uint8_t protectedPortsUnlocked;
    uint8_t flashUnlocked;

    bool off;
} control_state_t;

extern control_state_t control;

__declspec(dllexport) eZ80portrange_t init_control(void);
__declspec(dllexport) void control_reset(void);
__declspec(dllexport) bool control_restore(FILE *image);
__declspec(dllexport) bool control_save(FILE *image);
__declspec(dllexport) bool protected_ports_unlocked(void);
__declspec(dllexport) bool flash_unlocked(void);
__declspec(dllexport) bool unprivileged_code(void);
__declspec(dllexport) int cpu_get_ctlptr();
__declspec(dllexport) extern uint8_t control_read(const uint16_t pio, bool peek);
__declspec(dllexport) extern void control_write(const uint16_t pio, const uint8_t byte, bool poke);

#ifdef __cplusplus
}
#endif

#endif
