#include "Elevator.h"

#include <RobotMap.h>
#include <Commands/Elevator/ChangeState.h>


Elevator::Elevator(): Subsystem("Elevator"){
    winchMotor = new CANTalon(WINCH_TALON_CAN_ID);

    endSwitchTripped = false;
    readySwitchTopTripped = false;
    readySwitchBottomTripped = false;

    endSwitchTrigger = new LimitTrigger(END_CHANNEL);
    endSwitchTrigger->WhenActive(new ChangeState(Elevator::switches::ENDSWITCH, this));

    readySwitchTopTrigger = new IrTrigger(READY_CHANNEL_TOP);
    readySwitchTopTrigger->WhenActive(new ChangeState(Elevator::switches::READYSWITCHTOP, this));

    readySwitchBottomTrigger = new IrTrigger(READY_CHANNEL_BOTTOM);
    readySwitchBottomTrigger->WhenActive(new ChangeState(Elevator::switches::READYSWITCHBOTTOM, this));

    binSwitchTrigger = new LimitTrigger(BIN_SWITCH_CHANNEL);
    binSwitchTrigger->WhenActive(new ChangeState(Elevator::switches::BINSWITCH, this));

    winchMotor->SetFeedbackDevice(CANTalon::AnalogEncoder);

    for(int i = 0; i < 4; i++) {
        switchLastTrippedPos[i] = getEncoder();
    }
}

Elevator::~Elevator() {
    delete winchMotor;
}



void Elevator::toState(Elevator::states desiredPos) {
    commandedState = desiredPos;
    if(abs((commandedState - state) > 0)) {
        if((commandedState - state) > 0) {
            nextState(true);
        } else {
            previousState(true);
        }
    }
    PutDashboard();
}


//called by the various atswitch methods if we have just tripped the correct switch
void Elevator::atState() {
    state = goingToState;
    if(goingToState == commandedState) {
        commandedState = state;
        changingState = false;
        stopMotor();
        toTrip = switches::NOSWITCH;
    } else {
        toState(commandedState);
    }
    PutDashboard();
}

void Elevator::nextState(bool toStateCalled) {
    if(state < states::CARRYINGTOTE4) {
        changingState = true;
        goingToState = static_cast<Elevator::states>(state + 1);
        if(!toStateCalled) {
            if(commandedState < states::CARRYINGTOTE4) {
                commandedState = static_cast<Elevator::states>(commandedState + 1);
            }
        }
        driveMotor(WINCH_MOTOR_SPEED);
        toTrip = upSwitches[goingToState - 1];
    }
    PutDashboard();
}

void Elevator::previousState(bool toStateCalled) {
    if(state > states::READYBIN) {
        changingState = true;
        goingToState = static_cast<Elevator::states>(state - 1);
        if(!toStateCalled) {
            if(commandedState > states::READYBIN) {
                commandedState = static_cast<Elevator::states>(commandedState - 1);
             }
        }
        driveMotor(-WINCH_MOTOR_SPEED);
        toTrip = downSwitches[goingToState];
    }
    PutDashboard();
}

void Elevator::PutDashboard() {
    std::string state_string;
    switch(state) {
    case states::READYBIN:
        state_string = "Ready Bin"; break;
    case states::READYTOTE1 :
        state_string = "Ready Tote 1"; break;
    case states::READYSTACK1 :
        state_string = "Ready Stack 1"; break;
    case states::READYTOTE2 :
        state_string = "Ready Tote 2"; break;
    case states::READYTOTE3 :
        state_string = "Ready Tote 3"; break;
    case states::READYSTACK2 :
        state_string = "Ready Stack 2"; break;
    case states::READYTOTE4 :
        state_string = "Ready Tote 4"; break;
    case states::CARRYINGTOTE4 :
        state_string = "Carrying Tote 4"; break;
    }
    SmartDashboard::PutString("Elevator State: ", state_string);
    std::string switch_string;
    switch(toTrip) {
    case switches::ENDSWITCH:
        switch_string = "End Switch"; break;
    case switches::READYSWITCHTOP:
        switch_string = "Top Ready Switch"; break;
    case switches::READYSWITCHBOTTOM:
        switch_string = "Bottom Ready Switch"; break;
    case switches::BINSWITCH:
        switch_string = "Bin Switch"; break;
    case switches::NOSWITCH:
        switch_string = "N/A"; break;
    }
    SmartDashboard::PutString("To Trip: ", switch_string);
    std::string commanded_string;
    if(commandedState != state) {
        switch(goingToState) {
            case states::READYBIN:
                commanded_string = "Ready Bin"; break;
            case states::READYTOTE1 :
                commanded_string = "Ready Tote 1"; break;
            case states::READYSTACK1 :
                commanded_string = "Ready Stack 1"; break;
            case states::READYTOTE2 :
                commanded_string = "Ready Tote 2"; break;
            case states::READYTOTE3 :
                commanded_string = "Ready Tote 3"; break;
            case states::READYSTACK2 :
                commanded_string = "Ready Stack 2"; break;
            case states::READYTOTE4 :
                commanded_string = "Ready Tote 4"; break;
            case states::CARRYINGTOTE4 :
                commanded_string = "Carrying Tote 4"; break;
        }
    } else {
        commanded_string = "Current";
    }
    SmartDashboard::PutString("Commanded State: ", commanded_string);
    std::string going_to_string;
    if(goingToState != state) {
        switch(goingToState) {
            case states::READYBIN:
                going_to_string = "Ready Bin"; break;
            case states::READYTOTE1 :
                going_to_string = "Ready Tote 1"; break;
            case states::READYSTACK1 :
                going_to_string = "Ready Stack 1"; break;
            case states::READYTOTE2 :
                going_to_string = "Ready Tote 2"; break;
            case states::READYTOTE3 :
                going_to_string = "Ready Tote 3"; break;
            case states::READYSTACK2 :
                going_to_string = "Ready Stack 2"; break;
            case states::READYTOTE4 :
                going_to_string = "Ready Tote 4"; break;
            case states::CARRYINGTOTE4 :
                going_to_string = "Carrying Tote 4"; break;
        }
    } else {
        going_to_string = "At State";
    }
    SmartDashboard::PutString("Going To State: ", going_to_string);
    SmartDashboard::PutBoolean("Changing State: ", changingState);
}

void Elevator::driveMotor(double speed) {
    speed = -speed;
    if(speed < 0 && endSwitchTrigger->Get()){
        return;
    } else if (speed > 0 && binSwitchTrigger->Get()) {
        return;
    }
    winchMotor->Set(speed);
}

void Elevator::stopMotor() {
    winchMotor->Set(0.0f);
}

void Elevator::atEndSwitch() {
    /*if(abs(switchLastTrippedPos[3] - getEncoder()) > LIMIT_SWITCH_IGNORE) {
        state = states::CARRYINGTOTE4;
        goingToState = state;
        commandedState = state;
        changingState = false;
        if(winchMotor->Get() < 0) {
            stopMotor();
        }
        toTrip = switches::NOSWITCH;
    }*/
    state = states::CARRYINGTOTE4;
    goingToState = state;
    commandedState = state;
    changingState = false;
    if(winchMotor->Get() < 0) {
        stopMotor();
    }
    toTrip = switches::NOSWITCH;
}

void Elevator::atReadySwitchTop() {
    if(toTrip == Elevator::switches::READYSWITCHTOP
            //&& abs(switchLastTrippedPos[2] - getEncoder()) > LIMIT_SWITCH_IGNORE
            ) {
        atState();
    }
}

void Elevator::atReadySwitchBottom() {
    if(toTrip == Elevator::switches::READYSWITCHBOTTOM
            //&& abs(switchLastTrippedPos[1] - getEncoder()) > LIMIT_SWITCH_IGNORE
            ) {
        atState();
    }
}

void Elevator::atBinSwitch() {
    /*if(abs(switchLastTrippedPos[0] - getEncoder()) > LIMIT_SWITCH_IGNORE) {
        state = states::READYBIN;
        goingToState = state;
        commandedState = state;
        changingState = false;
        if(winchMotor->Get() > 0) {
            stopMotor();
        }
        stopMotor();
        toTrip = switches::NOSWITCH;
    }*/
    state = states::READYBIN;
    goingToState = state;
    commandedState = state;
    changingState = false;
    if(winchMotor->Get() > 0) {
        stopMotor();
    }
    toTrip = switches::NOSWITCH;
}

Elevator::states Elevator::getState() {
    return state;
}

Elevator::switches Elevator::getToTrip() {
    return toTrip;
}

double Elevator::getEncoder() {
    //return winchMotor->GetPosition();
    //return winchMotor->GetAnalogInRaw();
    return winchMotor->GetPosition()/ENCODER_COUNTS_PER_REVOLUTION*ENCODER_TO_SPOOL*ELEVATOR_MAX_SPOOL_SIZE;
}
