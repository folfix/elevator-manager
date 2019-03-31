#ifndef PASSENGER_H
#define PASSENGER_H

#include "src/elevator/direction.h"


enum PassengerStatus {NEW, WAITING_FOR_ELEVATOR, IN_ELEVATOR, DONE};

class Passenger {

private:
    PassengerStatus status;
    int waitFloor;
    int destinationFloor;

public:
    Passenger(int waitFloor, int destinationFloor);

    //TODO make private
    //TODO dokumentacja doxygen

    void goToElevator();
    void done();
    void elevatorAllocated();

    Direction getDirection();

    bool isWaiting();
    bool isWaiting(int floor);
    bool isInElevator();
    bool hasNotAllocatedElevator();

    bool isDone();

    friend bool operator==(const Passenger & lhs, const Passenger & rhs);

    int getWaitFloor();
    int getDestinationFloor();

};

#endif // PASSENGER_H
