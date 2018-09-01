/**
 * Sleep locks, sleep manager, reboot, idle loop
 */
#include "mbed.h"
#include "rtos_idle.h"

InterruptIn button(BUTTON1);
DigitalOut led1(LED1);
DigitalOut led3(LED3);

void toggle() {
    led1 = !led1;
}

void nodeepsleep() {
    // Lock deep sleep to prevent deep sleep
    // improve interrupt performance and expense of higher power consuption
    DeepSleepLock lock;

    while(true) {
        // Wait and let interrupts take care of the rest
        wait(10.0);
    }
}

void withdeepsleep() {
    while(true) {
        // Wait and let interrupts take care of the rest
        wait(10.0);
    }
}

void sleepmode() {
    printf("Deep sleep allowed: %i\r\n", sleep_manager_can_deep_sleep());
    wait(1.0);

    printf("Locking deep sleep\r\n");
    sleep_manager_lock_deep_sleep();

    printf("Deep sleep allowed: %i\r\n", sleep_manager_can_deep_sleep());
    wait(1.0);
}

void reboot() {
    printf("Rebooting...\r\n");
    wait(1.0);
    system_reset();
}


void idle_loop() {
    led3 = !led3;
    wait(0.1);
}

int main() {
    button.rise(&toggle);
    button.fall(&toggle);

    //nodeepsleep();
    //withdeepsleep();
    rtos_attach_idle_hook(&idle_loop);
    sleepmode();
    //reboot();
}