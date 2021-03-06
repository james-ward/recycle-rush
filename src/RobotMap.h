#ifndef ROBOTMAP_H
#define ROBOTMAP_H

#include "WPILib.h"

/**
 * The RobotMap is a mapping from the ports sensors and actuators are wired into
 * to a variable name. This provides flexibility changing wiring, makes checking
 * the wiring easier and significantly reduces the number of magic numbers
 * floating around.
 */
 
// For example to map the left and right motors, you could define the
// following variables to use with your drivetrain subsystem.
//const int LEFTMOTOR = 1;
//const int RIGHTMOTOR = 2;

// If you are using multiple modules, make sure to define both the port
// number and the module. For example you with a rangefinder:
//const int RANGE_FINDER_PORT = 1;
//const int RANGE_FINDER_MODULE = 1;

const int DRIVE_MOTOR_A_ID = 4;
const int DRIVE_MOTOR_B_ID = 7;
const int DRIVE_MOTOR_C_ID = 1;
const int DRIVE_MOTOR_D_ID = 3;

const int WINCH_TALON_CAN_ID = 2;

const double JOY_DRV_DEAD_X = 0.05;
const double JOY_DRV_DEAD_Y = 0.05;
const double JOY_DRV_DEAD_Z = 0.25;

const double JOYSTICK_X_EXPONENTIAL = 10;
const double JOYSTICK_Y_EXPONENTIAL = 10;
const double JOYSTICK_Z_EXPONENTIAL = 40;

const int GYRO_RESET_BUTTON = 11;
const int FIELD_ORIENT_BUTTON = 12;
const int TOGGLE_PID_BUTTON = 10;
const int RAISE_ELEVATOR_BUTTON = 6;
const int LOWER_ELEVATOR_BUTTON = 4;
const int MOVE_ELEVATOR_TO_NEXT_POSITION_BUTTON = 5;
const int MOVE_ELEVATOR_TO_PREVIOUS_POSITION_BUTTON = 3;
const int STACK_BUTTON = 7;
const int STROBE_BUTTON = 9;
const int STOP_ELEVATOR_BUTTON = 8;

const double STROBE_RATE = 2.0;

const int ENCODER_COUNTS_PER_REVOLUTION = 1000; //counts per revolution

const double TO_SCORING_ZONE = 10; // meters
const double AUTON_TURN_THROTTLE = 0.4;
const double AUTON_TURN_Z = 1.0;

const int BIN_SWITCH_CHANNEL = 0;
const int READY_CHANNEL_BOTTOM = 1;
const int READY_CHANNEL_TOP = 2;
const int END_CHANNEL = 3;

const double WINCH_MOTOR_SPEED = 1.0;

const double ELEVATOR_DIO_TIMEOUT = 5;

const int LIGHT_RING_CHANNEL = 0;

#endif
