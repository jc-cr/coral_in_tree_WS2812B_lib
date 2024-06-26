#include "libs/base/gpio.h"
#include "libs/base/led.h"
#include "libs/base/timer.h"
#include "third_party/nxp/rt1176-sdk/devices/MIMXRT1176/drivers/fsl_gpt.h"
#include "third_party/nxp/rt1176-sdk/devices/MIMXRT1176/drivers/fsl_common_arm.h"
#include "third_party/freertos_kernel/include/FreeRTOS.h"
#include "third_party/freertos_kernel/include/task.h"

namespace coralmicro 
{
    constexpr Gpio kLedPin = Gpio::kPwm0;
    // WS2812B timing (in nanoseconds)
    constexpr uint32_t T0H = 400 - 150;
    constexpr uint32_t T1H = 800 - 150;
    constexpr uint32_t T0L = 850 - 150;
    constexpr uint32_t T1L = 450 - 150;
    constexpr uint32_t RES = 6000;

    void InitializeTimer() {
        gpt_config_t gptConfig;
        GPT_GetDefaultConfig(&gptConfig);
        gptConfig.enableRunInDoze = true;
        GPT_Init(GPT1, &gptConfig);
        
        // Enable GPT1 clock - this might need to be done differently in Coral Micro
        // You may need to use a different API or it might be enabled by default
    }

    void DelayNanoseconds(uint32_t ns) {
        uint32_t start = TimerMicros();
        while ((TimerMicros() - start) * 1000 < ns) {
            __asm__ volatile ("nop");
        }
    }

    inline void SendBit(bool bit) {
        GpioSet(kLedPin, true);
        DelayNanoseconds(bit ? T1H : T0H);
        GpioSet(kLedPin, false);
        DelayNanoseconds(bit ? T1L : T0L);
    }

    inline void SendByte(uint8_t byte) {
        for (int i = 7; i >= 0; --i) {
            SendBit(byte & (1 << i));
        }
    }

    inline void SendColor(uint8_t red, uint8_t green, uint8_t blue) {
        uint32_t primask = DisableGlobalIRQ();
        SendByte(green);
        SendByte(red);
        SendByte(blue);
        EnableGlobalIRQ(primask);
    }

    inline void ResetDelay() {
        DelayNanoseconds(RES);
    }

    [[noreturn]] void Main() {
        printf("WS2812B Example using GPIO!\r\n");
        LedSet(Led::kStatus, true);
        
        InitializeTimer();
        GpioSetMode(kLedPin, GpioMode::kOutput);
        
        while (true) {
            SendColor(0, 255, 0);  // Green
            ResetDelay();
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}  // namespace coralmicro

extern "C" [[noreturn]] void app_main(void* param) {
    (void)param;
    coralmicro::Main();
}