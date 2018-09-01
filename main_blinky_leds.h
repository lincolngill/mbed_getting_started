#include "mbed.h"

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

// main() runs in its own thread in the OS
int main() {
    while (true) {
        led1 = !led1;
        wait(0.5);
        led2 = !led2;
        wait(0.5);
        led3 = !led3;
        wait(0.5);
    }
}