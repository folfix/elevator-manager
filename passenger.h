#ifndef PASSENGER_H
#define PASSENGER_H

#include "direction.h"


enum PassengerStatus {WAITING, IN_ELEVATOR, DONE};

class Passenger {
public:
    Passenger(int waitFloor, int destinationFloor);

    int waitFloor;
    int destinationFloor;

    void inElevator();
    void done();
    PassengerStatus getStatus();
    Direction getDirection();

private:
    PassengerStatus status;
};

#endif // PASSENGER_H
