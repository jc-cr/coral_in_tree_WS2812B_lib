#include <cstdint>
#include "libs/base/gpio.h"
#include "libs/base/led.h"
#include "third_party/nxp/rt1176-sdk/devices/MIMXRT1176/drivers/fsl_gpt.h"
#include "third_party/freertos_kernel/include/FreeRTOS.h"
#include "third_party/freertos_kernel/include/task.h"

namespace coralmicro {

constexpr Gpio kLedPin = Gpio::kPwm0;

// WS2812B timing (in nanoseconds)
constexpr uint32_t T0H = 250;
constexpr uint32_t T1H = 650;
constexpr uint32_t T0L = 700;
constexpr uint32_t T1L = 300;
constexpr uint32_t RES = 50000;

// Assuming M7 core runs at 800MHz, each cycle is 1.25ns
constexpr uint32_t CYCLES_PER_NS = 8 / 10; // 0.8 cycles per nanosecond

// Pre-calculated cycle counts
constexpr uint32_t T0H_CYCLES = T0H * CYCLES_PER_NS;
constexpr uint32_t T1H_CYCLES = T1H * CYCLES_PER_NS;
constexpr uint32_t T0L_CYCLES = T0L * CYCLES_PER_NS;
constexpr uint32_t T1L_CYCLES = T1L * CYCLES_PER_NS;
constexpr uint32_t RES_CYCLES = RES * CYCLES_PER_NS;

inline __attribute__((always_inline)) void delay_cycles(uint32_t count) {
    asm volatile (
        "1: subs %[count], %[count], #1 \n"
        "   bne 1b \n"
        : [count] "+r" (count)
        :
        : "cc"
    );
}

void __attribute__((noinline, optimize("O0"))) SendBit(bool bit) {
    if (bit) {
        GpioSet(kLedPin, true);
        delay_cycles(T1H_CYCLES);
        GpioSet(kLedPin, false);
        delay_cycles(T1L_CYCLES);
    } else {
        GpioSet(kLedPin, true);
        delay_cycles(T0H_CYCLES);
        GpioSet(kLedPin, false);
        delay_cycles(T0L_CYCLES);
    }
}

void __attribute__((noinline, optimize("O0"))) SendByte(uint8_t byte) {
    SendBit(byte & 0x80);
    SendBit(byte & 0x40);
    SendBit(byte & 0x20);
    SendBit(byte & 0x10);
    SendBit(byte & 0x08);
    SendBit(byte & 0x04);
    SendBit(byte & 0x02);
    SendBit(byte & 0x01);
}

void SendColor(uint8_t red, uint8_t green, uint8_t blue) {
    uint32_t primask = DisableGlobalIRQ();
    SendByte(green);
    SendByte(red);
    SendByte(blue);
    EnableGlobalIRQ(primask);
}

void ResetDelay() {
    GpioSet(kLedPin, false);
    delay_cycles(RES_CYCLES);
}

[[noreturn]] void Main() {
    printf("WS2812B Example using GPIO on M7 core!\r\n");
    LedSet(Led::kStatus, true);
    GpioSetMode(kLedPin, GpioMode::kOutput);

    while (true) {
        SendColor(255, 0, 0);
        ResetDelay();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

} // namespace coralmicro

extern "C" [[noreturn]] void app_main(void* param) {
    (void)param;
    coralmicro::Main();
}