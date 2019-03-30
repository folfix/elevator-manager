#include "passenger.h"
#include "qdebug.h"

Passenger::Passenger(int waitFloor, int destinationFloor) {
    this->waitFloor = waitFloor;
    this->destinationFloor = destinationFloor;
    status = WAITING;
}

Direction Passenger::getDirection() {
    return waitFloor < destinationFloor ? UP : DOWN;
}

void Passenger::goToElevator() {
    status = IN_ELEVATOR;
    qInfo() << "Passenger in elevator:" << waitFloor << "->" << destinationFloor;
}

void Passenger::done() {
    status = DONE;
    qInfo() << "Passenger has arrived:" << waitFloor << "->" << destinationFloor;
}



PassengerStatus Passenger::getStatus() {
    return status;
}

bool Passenger::isInElevator() {
    return status == IN_ELEVATOR;
}

bool Passenger::isWaiting() {
    return status == WAITING;
}

bool Passenger::isDone() {
    return status == DONE;
}

bool operator==(const Passenger &lhs, const Passenger &rhs) {
    return lhs.destinationFloor == rhs.destinationFloor && lhs.waitFloor == rhs.waitFloor;
}


