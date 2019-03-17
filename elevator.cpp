#include <utility>
#include <QtWidgets/QGridLayout>

#include "elevator.h"
#include "elevatormanager.h"

Elevator::Elevator(QString name, int minFloor, int maxFloor, QSlider *slider) {
    this->name = std::move(name);
    this->minFloor = minFloor;
    this->maxFloor = maxFloor;
    this->slider = slider;
    this->slider->setMinimum(ElevatorManager::GROUND_FLOOR_NUMBER);
    this->slider->setTickPosition(QSlider::TickPosition::TicksLeft);
    this->slider->setSingleStep(1);
    this->slider->setDisabled(true);
}

QString Elevator::getName() {
    return this->name;
}

int Elevator::getMinFloor() {
    return this->minFloor;
}

int Elevator::getMaxFloor() {
    return this->maxFloor;
}

void Elevator::rerender(int maxFloor) {
    slider->setMaximum(maxFloor);
}



