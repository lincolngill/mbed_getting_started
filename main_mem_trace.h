/**
 * Need to set MBED_MEM_TRACING_ENABLED
 */
#include <stdlib.h>
#include "mbed.h"
#include "mbed_mem_trace.h"

DigitalOut led1(LED1);

int main() {
    printf("mem trace\n");
    mbed_mem_trace_set_callback(mbed_mem_trace_default_callback);
    while (true) {
        led1 = !led1;
        void *p =malloc(50);
        wait(0.5);
        free(p);
    }
}
