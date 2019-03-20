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

void Passenger::inElevator() {
    status = IN_ELEVATOR;
    qInfo() << "Passenger in elevator";
}

void Passenger::done() {
    status = DONE;
    qInfo() << "Passenger has arrived";
}

PassengerStatus Passenger::getStatus() {
    return status;
}
