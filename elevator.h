#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QSlider>
#include <QString>
#include "direction.h"
#include "passenger.h"

class Elevator {
private:
    QString name;

    int minFloor;
    int maxFloor;
    int currentFloor;
    int destinationFloor;
    Direction direction;
    std::list<Passenger> waitingPassengers;
    std::list<Passenger> passengersInside;
    bool isReturning;

    QSlider* slider;
    QTimer* timer;
    void start();
    void openDoor();
    void closeDoor();

    void checkIfAnyPassengerIsInDestination();
    void checkIfAnyPassengerWantsGetIn();

    int getLowestDestinationFloor();
    int getHighestDestinationFloor();
    int getLowestWaitingFloor();
    int getHighestWaitingFloor();

    void updateDestinationFloor();


public:
    Elevator(QString name, int minFloor, int maxFloor, QSlider* slider);
    QString getName();
    int getMinFloor();
    int getMaxFloor();
    void rerender(int maxFloor);
    Direction getDirection();

    void addPassenger(Passenger passenger);
};


#endif // ELEVATOR_H
