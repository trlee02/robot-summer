#include "config.h"
#include "claw.h"
#include "arm.h"
#include "state-machine.h"
#include "custom-servo.h"
#include "display-manager.h"

namespace Claw {

    volatile bool magnetic_idol = false;
    bool seen_magnet = false;

    void leftGoLowerLimit() {
        LeftTiltServo::write(LEFT_LOWER_ANGLE);
        delay(SERVO_WAIT_TIME);
    }

    void rightGoLowerLimit() {
        RightTiltServo::write(RIGHT_LOWER_ANGLE);
        delay(SERVO_WAIT_TIME);
    }

    void leftGoUpperLimit() {
        LeftTiltServo::write(LEFT_UPPER_ANGLE);
        delay(SERVO_WAIT_TIME);
   }

    void rightGoUpperLimit() {
        RightTiltServo::write(RIGHT_UPPER_ANGLE);
        delay(SERVO_WAIT_TIME);
    }

    void open() {
        GrabServo::write(SERVO_OPEN_ANGLE);
        delay(SERVO_WAIT_TIME);
    }

    void close(int angle) {
        if (!magnetic_idol) {
            GrabServo::write(SERVO_CLOSE_ANGLE/SERVO_ANGLE_DIVISION * angle);
            delay(SERVO_WAIT_TIME/4);
        }
    } 

    void leftGoMiddle() {
        
        LeftTiltServo::write((int)(LEFT_LOWER_ANGLE + LEFT_UPPER_ANGLE)/2);
        delay(SERVO_WAIT_TIME);
    }

    void rightGoMiddle() {
        RightTiltServo::write((int)(RIGHT_LOWER_ANGLE + RIGHT_UPPER_ANGLE)/2);
        delay(SERVO_WAIT_TIME);
    }

    void setupServos() {
        LeftTiltServo::write(LEFT_LOWER_ANGLE - 20);
        RightTiltServo::write(RIGHT_LOWER_ANGLE + 20);
        open();

        delay(SERVO_WAIT_TIME);


        LeftTiltServo::write(LEFT_UPPER_ANGLE);
        RightTiltServo::write(RIGHT_UPPER_ANGLE);
        GrabServo::write(SERVO_CLOSE_ANGLE);
    }

    void setupHallSensor() {
        pinMode(MAGNET_INTERRUPT_PIN, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(MAGNET_INTERRUPT_PIN), handleMagneticField, FALLING);
    }

    /**
     * @brief Waits 1 second, scans again then handles situation
     * 
     */
    void handleMagneticField() {

        Display::display_handler.clearDisplay();
        Display::display_handler.setCursor(0,0);
        Display::display_handler.println("first detection");
        Display::display_handler.println(digitalRead(MAGNET_INTERRUPT_PIN));
        Display::display_handler.display();

        delay(1000);

        if (digitalRead(MAGNET_INTERRUPT_PIN) < 1) {
            Display::display_handler.clearDisplay();
            Display::display_handler.setCursor(0,0);
            Display::display_handler.println("its a bomb");
            Display::display_handler.display();
            magnetic_idol = true;
            detachInterrupt(MAGNET_INTERRUPT_PIN);
            delay(1000);
            Display::display_handler.clearDisplay();
            Display::display_handler.setCursor(0,0);
            Display::display_handler.println("interrupt detatched");
            Display::display_handler.display();
            StateMachine::StateHandler = StateMachine::state_magneticField;
            return;            
        }

        Display::display_handler.clearDisplay();
        Display::display_handler.setCursor(0,0);
        Display::display_handler.println("false alarm");
        Display::display_handler.display();    
        }
}