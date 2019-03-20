#ifndef PASSENGER_H
#define PASSENGER_H


class Passenger
{
public:
    Passenger(int waitFloor, int destinationFloor);
    int waitFloor;
    int destinationFloor;
};

#endif // PASSENGER_H