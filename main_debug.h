/**
 * Need to compile with (Set in mbed_compile_debug task):
 *     -DMBED_HEAP_STATS_ENABLED=1 -DMBED_STACK_STATS_ENABLED=1 -DDEBUG_ISR_STACK_USAGE=1
 */
#include "mbed.h"
//#include "mbed-os/platform/mbed_assert.h"
//#include "mbed-os/platform/mbed_debug.h"
//#include "mbed-os/platform/mbed_error.h"
//#include "mbed-os/platform/mbed_stats.h"

#define MAX_THREAD_INFO 10

mbed_stats_heap_t heap_info;
mbed_stats_stack_t stack_info[ MAX_THREAD_INFO ];

int main() {
    debug("\nThis message is from debug function\n");
    debug_if(true, "This message is from debug_if\n");
    debug_if(false, "SOMETHING HAS GONE WRONG. Should not see this msg.\n");

    printf("Heap MemoryStats:\n");
    mbed_stats_heap_get(&heap_info);
    printf("\tCurrent Size: %d bytes\n", (int) heap_info.current_size);
    printf("\tMax Size: %d bytes\n", (int) heap_info.max_size);
    printf("\tCumulative sum od bytes ever allocated (total_size): %d bytes\n", (int) heap_info.total_size);
    printf("\tCurrent heap allocation (reserved_size): %d bytes\n", (int) heap_info.reserved_size);
    printf("\tNumber of allocations (alloc_cnt): %d\n", (int) heap_info.alloc_cnt);
    printf("\tNumber of failed allocations (alloc_fail_cnt): %d\n", (int) heap_info.alloc_fail_cnt);

    printf("Cumulative Stack Info:\n");
    mbed_stats_stack_get( stack_info );
    printf("\tMax stack size (max_size): %d bytes\n", (int) stack_info[0].max_size);
    printf("\tCurrent stack size (reserved_size): %d bytes\n", (int) stack_info[0].reserved_size);
    printf("\tNumber of stack stats (stack_cnt): %d\n", (int) stack_info[0].stack_cnt);

    printf("Thread Stack Info:\n");
    mbed_stats_stack_get_each( stack_info, MAX_THREAD_INFO );
    for (int i=0; i < MAX_THREAD_INFO; i++) {
        if (stack_info[i].thread_id != 0) {
            printf("\t%02d Thread ID: 0x%08X\n", i, (int) stack_info[i].thread_id);
            printf("\t%02d Max size (max_size): %d bytes\n", i, (int) stack_info[i].max_size);
            printf("\t%02d Current size (reserved_size): %d bytes\n", i, (int) stack_info[i].reserved_size);
            printf("\t%02d Number of stack stats (stack_cnt): %d\n", i, (int) stack_info[i].stack_cnt);
        }
    }

    printf("Done.\n\n");
}