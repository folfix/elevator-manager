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
//    start(passenger.waitFloor);
//    start(passenger.destinationFloor);
}

void Elevator::start() {
    int destinationFloorAfterDirectionChange;

    if (currentFloor == destinationFloor) {
        qInfo() << "Should not happen";
        exit(1);
    } else if (currentFloor < destinationFloor) {
        direction = UP;
        qInfo() << "Elevator going up";
        while (currentFloor < destinationFloor) {
            QThread::sleep(1);
            slider->setValue(++currentFloor);
            qInfo() << "Elevator floor" << currentFloor;
            checkIfAnyPassengerIsInDestination();
            checkIfAnyPassengerWantsGetIn();
        }

    } else {
        direction = DOWN;
        qInfo() << "Elevator going down";
        while (currentFloor > destinationFloor) {
            QThread::sleep(1);
            slider->setValue(--currentFloor);
            qInfo() << "Elevator floor" << currentFloor;
            checkIfAnyPassengerIsInDestination();
            checkIfAnyPassengerWantsGetIn();
        }
    }


    qInfo() << "Elevator reached destination floor" << destinationFloor;
    direction = STOP;
}

int Elevator::getLowestDestinationFloor() {
    int floor = 0;
    for (auto &passenger : passengersInside) {
        floor = floor < passenger.destinationFloor ? floor : passenger.destinationFloor;
    }
    return floor;
}

int Elevator::getHighestDestinationFloor() {
    int floor = 0;
    for (auto &passenger : passengersInside) {
        floor = floor > passenger.destinationFloor ? floor : passenger.destinationFloor;
    }
    return floor;
}

void Elevator::checkIfAnyPassengerIsInDestination() {
    bool anyOneInDestination = std::any_of(passengersInside.begin(), passengersInside.end(), [=](Passenger passenger) {
        return currentFloor == passenger.destinationFloor;
    });

    if (anyOneInDestination) {
        openDoor();
        for (auto &passenger : passengersInside) {
            if (currentFloor == passenger.destinationFloor) {
                passenger.done();
            }
        }
        std::remove_if(passengersInside.begin(), passengersInside.end(), [=](Passenger passenger) {
            return passenger.getStatus() == DONE;
        });
        closeDoor();
    }
}

void Elevator::checkIfAnyPassengerWantsGetIn() {
    bool anyOneWantsGetIn = std::any_of(waitingPassengers.begin(), waitingPassengers.end(), [=](Passenger passenger) {
        return currentFloor == passenger.waitFloor;
    });

    if (anyOneWantsGetIn) {
        openDoor();
        for (auto &passenger : waitingPassengers) {
            if (currentFloor == passenger.waitFloor) {
                passenger.inElevator();
                passengersInside.push_front(passenger);
            }
        }
        std::remove_if(waitingPassengers.begin(), waitingPassengers.end(), [=](Passenger passenger) {
            return passenger.getStatus() == DONE;
        });

        if (direction == UP) {
            destinationFloor = getHighestDestinationFloor();
        } else {
            destinationFloor = getLowestDestinationFloor();
        }

        closeDoor();
    }
}

void Elevator::addPassenger(Passenger passenger) {
    //handle dodanie kolejnego

    // jak winda jedzie np. na dół na parter, to tezeba zrobić tak, żeby się stakowały wezwania
    // tak samo jak winda jedzie np na 10 piętro, to po drodze może zabrać pasażera z 3 na 5

    if (direction == STOP) {
        waitingPassengers.push_front(passenger);
        destinationFloor = passenger.waitFloor;
        QFuture<void> future = QtConcurrent::run([=] { start(); });
    }

    if (direction == UP) {
        if (passenger.getDirection() == UP) {
            waitingPassengers.push_front(passenger);
        } else {
            qInfo() << "Don't add passenger that do not follow elevator direction";
            exit(1);
        }
    }

    if (direction == DOWN) {
        if (passenger.getDirection() == DOWN) {
            waitingPassengers.push_front(passenger);
        } else {
            qInfo() << "Don't add passenger that do not follow elevator direction";
            exit(1);
        }
    }
}

void Elevator::openDoor() {
    qInfo() << "Door opening";
    QThread::sleep(1);
}

void Elevator::closeDoor() {
    qInfo() << "Door closing";
    QThread::sleep(2);
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







