#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QSlider>
#include <QString>
#include "passenger.h"

enum Direction {
    STOP, UP, DOWN
};

class Elevator {
private:
    QString name;

    int minFloor;
    int maxFloor;
    int currentFloor;
    Direction direction;
    std::list<int> floorQueue;

    QSlider* slider;
    QTimer* timer;
    void moveToFloor(int floor);
    void openCloseDoor();
    void handlePassenger(Passenger passenger);

    void addFloorToQueue(int floor);

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
