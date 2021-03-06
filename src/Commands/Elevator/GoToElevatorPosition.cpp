#include <Commands/Elevator/GoToElevatorPosition.h>

GoToElevatorPosition :: GoToElevatorPosition(Elevator::states toState): CommandBase ("GoToElevatorPosition"){
    this->commandedState = toState;
    Requires(elevator);
}

void GoToElevatorPosition::Initialize(){
}

void GoToElevatorPosition::Execute(){
    elevator->toState(commandedState);

}

bool GoToElevatorPosition::IsFinished(){
    return true;
}

void GoToElevatorPosition::End(){
}

void GoToElevatorPosition::Interrupted(){
    End();
}
