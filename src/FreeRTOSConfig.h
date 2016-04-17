#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#define configCPU_CLOCK_HZ F_CPU
#define configMAX_PRIORITIES 2
#define configMINIMAL_STACK_SIZE 128
#define configTICK_RATE_HZ 250
#define configUSE_16_BIT_TICKS 1
#define configUSE_IDLE_HOOK 0
#define configUSE_PREEMPTION 1
#define configUSE_TICK_HOOK 0

#define INCLUDE_vTaskDelayUntil 1

#endif /* FREERTOS_CONFIG_H */
