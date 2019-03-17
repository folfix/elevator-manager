#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QSlider>
#include <QString>

class Elevator {
private:
    QString name;
    int start;
    int stop;
    QSlider* slider;

public:
    int getPosition();
    Elevator(QString name, int minFloor, int maxFloor, QSlider* slider);
    QString getName();
};

#endif // ELEVATOR_H
