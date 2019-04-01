#include "passenger.h"
#include "qdebug.h"

/**
 * Constructor of passenger.
 * All required properties are set.
 * The initial state od passenger is NEW.
 *
 * @param waitFloor Floor that passenger is waiting on.
 * @param destinationFloor Destination floor of ride.
 */
Passenger::Passenger(int waitFloor, int destinationFloor) {
    if (waitFloor == destinationFloor) {
        throw std::invalid_argument("Wait floor is the same as the destination");
    }

    this->waitFloor = waitFloor;
    this->destinationFloor = destinationFloor;
    status = NEW;
}

/**
 * Returns direction of ride.
 * @return
 */
Direction Passenger::getDirection() {
    return waitFloor < destinationFloor ? UP : DOWN;
}

/**
 * Marks passenger as in elevator.
 */
void Passenger::goToElevator() {
    status = IN_ELEVATOR;
    qInfo() << "[P]" <<"Passenger in elevator:" << waitFloor << "->" << destinationFloor;
}

/**
 * Marks passenger as done.
 */
void Passenger::done() {
    status = DONE;
    qInfo() << "[P]" <<"Passenger has arrived:" << waitFloor << "->" << destinationFloor;
}

/**
 * Mark passenger as waiting, but elevator has been allocated.
 */
void Passenger::elevatorAllocated() {
    status = WAITING_FOR_ELEVATOR;
    qInfo() << "[P]" <<"Passenger waits for allocated elevator";
}

/**
 * Returns if passenger is in elevator.
 * @return if passenger is in elevator.
 */
bool Passenger::isInElevator() {
    return status == IN_ELEVATOR;
}

/**
 * Returns if passenger has NOT allocated elevator.
 * @return if passenger has NOT allocated elevator.
 */
bool Passenger::hasNotAllocatedElevator() {
    return status == NEW;
}

/**
 * Returns if passenger is waiting (apart from elevator allocation).
 * @return if passenger is waiting.
 */
bool Passenger::isWaiting() {
    return status == WAITING_FOR_ELEVATOR || status == NEW;
}

/**
 * Returns if passenger is waiting on specified floor (apart from elevator allocation).
 * @param floor to check.
 * @return if passenger is waiting on specified floor.
 */
bool Passenger::isWaiting(int floor) {
    return isWaiting() && waitFloor == floor;
}

/**
 * Returns if passenger has finished ride and it's done.
 * @return if ride is done.
 */
bool Passenger::isDone() {
    return status == DONE;
}

/**
 * Returns floor that passenger is waiting on.
 * @return waiting floor.
 */
int Passenger::getWaitFloor() {
    return waitFloor;
}

/**
 * Returns the destination floor of passenger's ride.
 * @return destination floor.
 */
int Passenger::getDestinationFloor() {
    return destinationFloor;
}

bool operator==(const Passenger &lhs, const Passenger &rhs) {
    return lhs.destinationFloor == rhs.destinationFloor && lhs.waitFloor == rhs.waitFloor;
}

QDebug& operator<< (QDebug& stream, const Passenger& p) {
    stream << "Passenger[waitFloor:" << p.waitFloor << ",destFloor:" << p.destinationFloor << ",status:" << p.status << "]";
    return stream;
}
