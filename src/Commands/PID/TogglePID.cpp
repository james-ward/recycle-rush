#include "TogglePID.h"

TogglePID :: TogglePID(): CommandBase ("TogglePID"){
    Requires(chassis);
}

void TogglePID::Initialize(){
    if(chassis->PIDEnabled()) {
    	chassis->DisablePID();
    } else {
    	chassis->EnablePID();
    }
}

void TogglePID::Execute(){
}

bool TogglePID::IsFinished(){
    return true;
}

void TogglePID::End(){

}

void TogglePID::Interrupted(){
}
