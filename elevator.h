#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QString>

class Elevator {
private:
    QString name;
    int start;
    int stop;

public:
    int getPosition();
    Elevator(QString, int, int);
};

#endif // ELEVATOR_H
