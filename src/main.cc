// main.cc for M7
#include <cstdio>

#include "libs/base/ipc_m7.h"
#include "libs/base/mutex.h"

// Does nothing except start the M4

extern "C" void app_main(void* param) {
  (void)param;
  printf("Multicore LED Example!\r\n");

  coralmicro::IpcM7::GetSingleton()->StartM4();
  CHECK(coralmicro::IpcM7::GetSingleton()->M4IsAlive(500));
  vTaskSuspend(nullptr);
}
