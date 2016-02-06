#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#define configUSE_PREEMPTION      1
#define configCPU_CLOCK_HZ        F_CPU
#define configTICK_RATE_HZ        250
#define configMAX_PRIORITIES      2
#define configMINIMAL_STACK_SIZE  256
#define configUSE_16_BIT_TICKS    1
#define configUSE_IDLE_HOOK       0
#define configUSE_TICK_HOOK       0
#define INCLUDE_vTaskPrioritySet  0
#define INCLUDE_uxTaskPriorityGet 0
#define INCLUDE_vTaskDelete       0
#define INCLUDE_vTaskSuspend      0
#define INCLUDE_vTaskDelayUntil   1
#define INCLUDE_vTaskDelay        1

#endif /* FREERTOS_CONFIG_H */
