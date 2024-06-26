
#include "third_party/nxp/rt1176-sdk/devices/MIMXRT1176/drivers/fsl_gpt.h"
#include "third_party/nxp/rt1176-sdk/devices/MIMXRT1176/drivers/fsl_qtmr.h"
#include "libs/base/gpio.h"
#include "libs/base/led.h"
#include "third_party/freertos_kernel/include/FreeRTOS.h"
#include "third_party/freertos_kernel/include/task.h"



namespace coralmicro 
{
    constexpr Gpio kLedPin = Gpio::kPwm0;
    
    // WS2812B timing (in nanoseconds)
    constexpr uint32_t T0H = 250;
    constexpr uint32_t T1H = 650;
    constexpr uint32_t T0L = 700;
    constexpr uint32_t T1L = 300;
    constexpr uint32_t RES = 50000;

    // QTMR configuration
    #define BOARD_QTMR_BASEADDR       TMR3
    #define BOARD_QTMR_PWM_CHANNEL    kQTMR_Channel_0
    #define QTMR_SOURCE_CLOCK CLOCK_GetRootClockFreq(kCLOCK_Root_Bus)


    void InitializeQTMR()
    {
        qtmr_config_t qtmrConfig;
        QTMR_GetDefaultConfig(&qtmrConfig);
        qtmrConfig.primarySource = kQTMR_ClockDivide_1;
        QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_QTMR_PWM_CHANNEL, &qtmrConfig);
    }


    void DelayNanoseconds(uint32_t ns)
    {
        uint32_t ticks = (ns * QTMR_SOURCE_CLOCK) / 1000000000U;
        QTMR_SetTimerPeriod(BOARD_QTMR_BASEADDR, BOARD_QTMR_PWM_CHANNEL, ticks);
        QTMR_StartTimer(BOARD_QTMR_BASEADDR, BOARD_QTMR_PWM_CHANNEL, kQTMR_PriSrcRiseEdge);
        while (!(QTMR_GetStatus(BOARD_QTMR_BASEADDR, BOARD_QTMR_PWM_CHANNEL) & kQTMR_CompareFlag)) {}
        QTMR_ClearStatusFlags(BOARD_QTMR_BASEADDR, BOARD_QTMR_PWM_CHANNEL, kQTMR_CompareFlag);
        QTMR_StopTimer(BOARD_QTMR_BASEADDR, BOARD_QTMR_PWM_CHANNEL);
    }


    void __attribute__((noinline, optimize("O0"))) SendBit(bool bit)
    {
        GpioSet(kLedPin, true);
        DelayNanoseconds(bit ? T1H : T0H);
        GpioSet(kLedPin, false);
        DelayNanoseconds(bit ? T1L : T0L);
    }

    void __attribute__((noinline, optimize("O0"))) SendByte(uint8_t byte)
    {
        for (int i = 7; i >= 0; --i) {
            SendBit(byte & (1 << i));
        }
    }

    void SendColor(uint8_t red, uint8_t green, uint8_t blue)
    {
        uint32_t primask = DisableGlobalIRQ();
        SendByte(green);
        SendByte(red);
        SendByte(blue);
        EnableGlobalIRQ(primask);
    }

    void ResetDelay()
    {
        DelayNanoseconds(RES);
    }

    [[noreturn]] void Main()
    {
        printf("WS2812B Example using GPIO and QTMR on M7 core!\r\n");
        LedSet(Led::kStatus, true);
        
        GpioSetMode(kLedPin, GpioMode::kOutput);
        InitializeQTMR();
        
        while (true) {
            SendColor(0, 255, 0);  // Green: GRB = (255, 0, 0)
            ResetDelay();
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}  // namespace coralmicro

extern "C" [[noreturn]] void app_main(void* param)
{
    (void)param;
    coralmicro::Main();
}