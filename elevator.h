#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QSlider>
#include <QString>

class Elevator {
private:
    QString name;
    int minFloor;
    int maxFloor;
    QSlider* slider;
    QTimer* timer;

public:
    Elevator(QString name, int minFloor, int maxFloor, QSlider* slider);
    QString getName();
    int getMinFloor();
    int getMaxFloor();
    void rerender(int maxFloor);
};

#endif // ELEVATOR_H
