#include <utility>

#include "elevator.h"

Elevator::Elevator(QString name, int start, int stop, QSlider* slider) {
    this -> name = std::move(name);
    this -> start = start;
    this -> stop = stop;
    this -> slider = slider;
}

QString Elevator::getName() {
    return this->name;
}