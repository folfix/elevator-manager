#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QSlider>
#include <QString>
#include <QFuture>
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
    std::list<Passenger> passengers;
    bool isReturning;
    unsigned long waitDuration;

    QSlider* slider;
    QFuture<void> future;

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
    Elevator(QString name, int minFloor, int maxFloor, QSlider* slider, unsigned long waitDuration = 1000);
    QString getName();
    int getMinFloor();
    int getMaxFloor();
    int getCurrentFloor();
    void rerender(int maxFloor);
    Direction getDirection();

    void addPassenger(Passenger passenger);

    void forceStop();
};


#endif // ELEVATOR_H
