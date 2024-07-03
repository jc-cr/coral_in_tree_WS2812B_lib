#include "third_party/nxp/rt1176-sdk/devices/MIMXRT1176/drivers/fsl_gpt.h"
#include "libs/base/gpio.h"
#include "libs/base/led.h"
#include "third_party/freertos_kernel/include/FreeRTOS.h"
#include "third_party/freertos_kernel/include/task.h"

extern "C" void _ZN10coralmicro7SendBitEb(bool bit);
extern "C" void _ZN10coralmicro15InitializeGpioEv();
extern "C" void _ZN10coralmicro10ResetDelayEv();

namespace coralmicro {

inline void SendBit(bool bit) {
    _ZN10coralmicro7SendBitEb(bit);
}

inline void InitializeGpio() {
    _ZN10coralmicro15InitializeGpioEv();
}

inline void ResetDelay() {
    _ZN10coralmicro10ResetDelayEv();
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

[[noreturn]] void Main() {
    LedSet(Led::kStatus, true);
    
    InitializeGpio();  // Initialize GPIO using our assembly function
    
    while (true) {
        SendColor(255, 0, 0);
        SendColor(255, 0, 0);
        SendColor(255, 0, 0);
        ResetDelay();

        // Add 2 sec delay
        vTaskDelay(pdMS_TO_TICKS(2000));
        
        SendColor(0, 255, 0);
        SendColor(0, 255, 0);
        SendColor(0, 255, 0);
        ResetDelay();

        vTaskDelay(pdMS_TO_TICKS(2000));

        
        SendColor(0, 0, 255);
        SendColor(0, 0, 255);
        SendColor(0, 0, 255);
        ResetDelay();


        vTaskDelay(pdMS_TO_TICKS(2000));

    }
}

} // namespace coralmicro

extern "C" [[noreturn]] void app_main(void* param) {
    (void)param;
    coralmicro::Main();
}