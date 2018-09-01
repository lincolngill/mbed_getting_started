#include "mbed.h"

int main() {
    // Set RTC time to Wed, 28 Oct 2009 11:35:37
    set_time(1256729737);

    while(true) {
        time_t seconds = time(NULL);
        printf("Seconds since Jan 1, 1970 = %llu\n", seconds);
        printf("Time = %s", ctime(&seconds));

        char buffer[32];
        strftime(buffer, 32, "%I:%M:%S %p\n", localtime(&seconds));
        printf("Custom time str = %s\n", buffer);
        wait(1.0);
    }
}