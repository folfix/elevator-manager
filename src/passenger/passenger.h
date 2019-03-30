#ifndef PASSENGER_H
#define PASSENGER_H

#include "src/elevator/direction.h"


enum PassengerStatus {WAITING, IN_ELEVATOR, DONE};

class Passenger {
public:
    Passenger(int waitFloor, int destinationFloor);

    int waitFloor;
    int destinationFloor;

    void goToElevator();
    void done();
    PassengerStatus getStatus();
    Direction getDirection();

    bool isInElevator();
    bool isWaiting();
    bool isDone();

    friend bool operator==(const Passenger & lhs, const Passenger & rhs);

private:
    PassengerStatus status;
};

#endif // PASSENGER_H
