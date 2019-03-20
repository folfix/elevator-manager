#include <utility>
#include <QtWidgets/QGridLayout>
#include <QtCore/QThread>
#include <QtCore/QFuture>
#include <QtConcurrent>

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
    direction = STOP;
    currentFloor = 0;
}

void Elevator::handlePassenger(Passenger passenger) {
    moveToFloor(passenger.waitFloor);
    openCloseDoor();
    moveToFloor(passenger.destinationFloor);
    openCloseDoor();
}

void Elevator::moveToFloor(int floor) {
    if (currentFloor == floor) {
        qInfo() << "Elevator stop";
        direction = STOP;
        return;
    }

    if (currentFloor < floor) {
        direction = UP;
        qInfo() << "Elevator going up";
        while (currentFloor < floor) {
            QThread::sleep(1);
            slider->setValue(++currentFloor);
            qInfo() << "Elevator floor" << currentFloor;
        }
    } else {
        direction = DOWN;
        qInfo() << "Elevator going down";
        while (currentFloor > floor) {
            QThread::sleep(1);
            slider->setValue(--currentFloor);
            qInfo() << "Elevator floor" << currentFloor;
        }
    }
    qInfo() << "Elevator reached destination floor" << floor;
}

void Elevator::openCloseDoor() {
    qInfo() << "Door opening";
    QThread::sleep(3);
    qInfo() << "Door closed";
}

void Elevator::addPassenger(Passenger passenger) {
    //handle dodanie kolejnego
    QFuture<void> future = QtConcurrent::run([=] { handlePassenger(passenger); });
}


Direction Elevator::getDirection() {
    return direction;
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





