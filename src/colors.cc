#include "libs/base/gpio.h"
#include "libs/base/led.h"
#include "libs/base/timer.h"
#include "third_party/freertos_kernel/include/FreeRTOS.h"
#include "third_party/freertos_kernel/include/task.h"

namespace coralmicro {

constexpr Gpio kLedPin = Gpio::kAd26;

void DelayMicros(uint32_t us) {
  uint64_t start_time = TimerMicros();
  while ((TimerMicros() - start_time) < us) {
    // Busy wait
  }
}

void SendBit(bool bit) {
  if (bit) {
    GpioSet(kLedPin, true);
    DelayMicros(0.8);
    GpioSet(kLedPin, false);
    DelayMicros(0.45);
  } else {
    GpioSet(kLedPin, true);
    DelayMicros(0.4);
    GpioSet(kLedPin, false);
    DelayMicros(0.85);
  }
}

void SendByte(uint8_t byte) {
  for (int i = 7; i >= 0; --i) {
    SendBit(byte & (1 << i));
  }
}

void SendColor(uint8_t red, uint8_t green, uint8_t blue) {
  SendByte(green);
  SendByte(red);
  SendByte(blue);
}

[[noreturn]] void Main() {
  printf("WS2812B Example!\r\n");
  // Turn on Status LED to show the board is on.
  LedSet(Led::kStatus, true);

  GpioSetMode(kLedPin, GpioMode::kOutput);
  TimerInit();

  while (true) {
    // Send color data to the LEDs (example: red, green, blue)
    SendColor(255, 0, 0); // Red
    SendColor(0, 255, 0); // Green
    SendColor(0, 0, 255); // Blue

    // Reset the LEDs
    DelayMicros(50); // Wait for at least 50us

    vTaskDelay(pdMS_TO_TICKS(1000)); // Wait 1 second
  }
}

}  // namespace coralmicro

extern "C" [[noreturn]] void app_main(void* param) {
  (void)param;
  coralmicro::Main();
}
