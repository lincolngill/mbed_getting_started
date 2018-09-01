#include "mbed.h"
#include "mbed-os/rtos/Thread.h"
#include "mbed-os/rtos/rtos_idle.h"
#include "mbed-os/platform/mbed_critical.h"

#define USE_CRITICAL_SECTION_LOCK   1   //Should not get race condition
//#define USE_CRITICAL_SECTION_LOCK   0   //May get race condition

#define THREAD_CNT 8

int32_t value = 100000;
volatile int32_t count = 0;

void increment(void) {
    for (int i = 0; i < value; i++) {
#if (USE_CRITICAL_SECTION_LOCK == 1)
        CriticalSectionLock lock;
#endif
        count += 1;
    }
}

int get_count(void) {
    if (count == (value * THREAD_CNT)) {
        printf("No Race condition\n");
    } else {
        printf("Race condition\n");
    }
    return count;
}

int main() {
    Thread counter_thread[THREAD_CNT];

    for (int i=0; i < THREAD_CNT; i++) {
        counter_thread[i].start(callback(increment));
    }

    // Wait for the threads to finish
    for (int i = 0; i < THREAD_CNT; i++) {
        counter_thread[i].join();
    }

    printf("Counter = %d\n", get_count());
}