#include "mbed.h"

RawSerial pc(USBTX, USBRX);
RawSerial dev(D1,D0);

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

void dev_recv() {
    led2 = !led2;
    while(dev.readable()) {
        pc.putc(dev.getc());
    }
}

void pc_recv() {
    led3 = !led3;
    while(pc.readable()) {
        dev.putc(pc.getc());
    }
}

int main() {
    pc.baud(9600);
    dev.baud(9600);

    pc.attach(&pc_recv, Serial::RxIrq);
    dev.attach(&dev_recv, Serial::RxIrq);

    pc.printf("Hello From STM32F429ZI\n");
    while (true) {
        led1 = !led1;
        wait(1.0);
    }
}