/**
 * Sonar class using LV-MaxSonar -EZ1
 * Demo
 *    Callbacks using object instance methods
 *    Timed event (echo pulse) using timer
 *    Regular task (trigger_reading) using a ticker
 *    Task duration (trigger length) using timeout
 * 
 *  EZ1
 *    BW (Input) - Held HIGH then TX does not output serial (low noise, pulse chaining mode)
 *    RX (Input) = Trigger Pin 
 *      LOW - Stop Ranging - Checked at end of every range cycle.
 *      HIGH - For >= 20uS to initiate a range reading
 *    PW (Output) = Echo pulse - 147uS/inch
 *    Power-up time = 250mS before ready to accept RX.
 *    Range reading cycle = 49mS (1st reading takes longer. ~+100mS)
 *      - Sends 13 x 42KHz waves
 *      - PW is set HIGH - For Max interval = 37.5mS if no target detected
 *      - PW is set LOW when target detected. 
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