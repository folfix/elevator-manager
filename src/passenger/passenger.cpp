#include "passenger.h"
#include "qdebug.h"

Passenger::Passenger(int waitFloor, int destinationFloor) {
    if (waitFloor == destinationFloor) {
        throw std::invalid_argument("Wait floor is the same as the destination");
    }

    this->waitFloor = waitFloor;
    this->destinationFloor = destinationFloor;
    status = NEW;
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

void Passenger::elevatorAllocated() {
    status = WAITING_FOR_ELEVATOR;
    qInfo() << "Passenger waits for allocated elevator";
}

bool Passenger::isInElevator() {
    return status == IN_ELEVATOR;
}

bool Passenger::hasNotAllocatedElevator() {
    return status == NEW;
}

bool Passenger::isWaiting() {
    return status == WAITING_FOR_ELEVATOR || status == NEW;
}

bool Passenger::isWaiting(int floor) {
    return isWaiting() && waitFloor == floor;
}

bool Passenger::isDone() {
    return status == DONE;
}

bool operator==(const Passenger &lhs, const Passenger &rhs) {
    return lhs.destinationFloor == rhs.destinationFloor && lhs.waitFloor == rhs.waitFloor;
}

int Passenger::getWaitFloor() {
    return waitFloor;
}

int Passenger::getDestinationFloor() {
    return destinationFloor;
}


