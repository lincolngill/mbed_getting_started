/**
 * Sonar class using LV-MaxSonar -EZ1
 * Demo
 *    Callbacks using object instance methods
 *    Timed event (echo pulse) using timer
 *    Regular task (trigger_reading) using a ticker
 *    Task duration (trigger length) using timeout
 */
#include "mbed.h"

DigitalOut led3(LED3);

class Sonar {
    DigitalOut   trigger;
    InterruptIn  echo;
    Timer        timer;
    Timeout      timeout;
    Ticker       ticker;
    int32_t      begin;
    int32_t      end;
    int32_t      micro_secs;
    
    public:
    Sonar(PinName trigger_pin, PinName echo_pin) : trigger(trigger_pin), echo(echo_pin) {
        trigger = 0;
        micro_secs = -1;
        echo.rise(callback(this, &Sonar::echo_begin));
        echo.fall(callback(this, &Sonar::echo_end));
        wait(0.25); // Wait for Sonar to initialise after power up
    }

    void start(void) {
        // 100ms ticker to trigger reading
        // LV-MaxSonar -EZ1 Datasheet: Reading take 49ms
        ticker.attach(callback(this, &Sonar::trigger_reading), 0.1f);
    }

    void stop(void) {
        ticker.detach();
    }

    /**
     * Echo pulse starts on rising edge
     */ 
    void echo_begin(void) {
        timer.reset();
        timer.start();
        begin = timer.read_us();
    }

    /**
     * Echo pulse ends on falling edge
     */
    void echo_end(void) {
        end = timer.read_us();
        timer.stop();
        micro_secs = end - begin;
    }

    /**
     * Turn off trigger
     */
    void trigger_off(void) {
        trigger = 0;
    }

    /**
     * Trigger reading - Turn off trigger using timeout
     */
    void trigger_reading(void) {
        trigger = 1;
        timeout.attach(callback(this, &Sonar::trigger_off), 10.0e-6);
    }

    /**
     * Return Sonar distance reading
     * Datasheet: 147us/inch. QED 147*2.54 = 57.9us/cm
     * \return distance (cm)
     */
    float read(void) {
        return micro_secs / 57.9f;
    }

};

int main() {
    printf("Sonar\n");
    Sonar sonar(D5, D6);
    sonar.start();
    while(true) {
        led3 = !led3;
        wait(1.0);
        printf("%3.2f cm\r\n", sonar.read());
    }
}