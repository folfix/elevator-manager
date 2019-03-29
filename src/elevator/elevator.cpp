#include <utility>
#include <QtWidgets/QGridLayout>
#include <QtCore/QThread>
#include <QtCore/QFuture>
#include <QtConcurrent>

#include "elevator.h"
#include "src/main/elevatormanager.h"

Elevator::Elevator(QString name, int minFloor, int maxFloor, QSlider *slider, unsigned long waitDuration) {
    this->name = std::move(name);
    this->minFloor = minFloor;
    this->maxFloor = maxFloor;
    this->slider = slider;
    this->slider->setMinimum(ElevatorManager::GROUND_FLOOR_NUMBER);
    this->slider->setTickPosition(QSlider::TickPosition::TicksLeft);
    this->slider->setSingleStep(1);
    this->slider->setDisabled(true);
    this->waitDuration = waitDuration;
    direction = STOP;
    currentFloor = 0;
    isReturning = false;
}

void Elevator::start() {
    if (currentFloor == destinationFloor) {
        qInfo() << "Elevator on destination floor";
        checkIfAnyPassengerIsInDestination();
        checkIfAnyPassengerWantsGetIn();
    } else if (currentFloor < destinationFloor) {
        direction = UP;
        qInfo() << "Elevator going up";
        while (currentFloor < destinationFloor) {
            QThread::msleep(waitDuration);
            slider->setValue(++currentFloor);
            qInfo() << "Elevator floor" << currentFloor;
            checkIfAnyPassengerIsInDestination();
            checkIfAnyPassengerWantsGetIn();
        }

    } else {
        direction = DOWN;
        qInfo() << "Elevator going down";
        while (currentFloor > destinationFloor) {
            QThread::msleep(waitDuration);
            slider->setValue(--currentFloor);
            qInfo() << "Elevator floor" << currentFloor;
            checkIfAnyPassengerIsInDestination();
            checkIfAnyPassengerWantsGetIn();
        }
    }

    if (isReturning) {
        qInfo() << "Elevator reached destination floor" << destinationFloor;
        direction = STOP;
        passengers.clear();
        isReturning = false;
    } else {
        isReturning = true;
        updateDestinationFloor();
        start();
    }
}

int Elevator::getLowestWaitingFloor() {
    Passenger min = *std::min_element(passengers.begin(), passengers.end(), [=](Passenger a, Passenger b) {
        return (a.waitFloor < b.waitFloor);
    });
    return min.waitFloor;
}

int Elevator::getHighestWaitingFloor() {
    Passenger max = *std::max_element(passengers.begin(), passengers.end(), [=](Passenger a, Passenger b) {
        return (a.waitFloor < b.waitFloor);
    });
    return max.waitFloor;
}

int Elevator::getLowestDestinationFloor() {
    Passenger min = *std::min_element(passengers.begin(), passengers.end(), [=](Passenger a, Passenger b) {
        return (a.destinationFloor < b.destinationFloor);
    });
    return min.destinationFloor;
}

int Elevator::getHighestDestinationFloor() {
    Passenger max = *std::max_element(passengers.begin(), passengers.end(), [=](Passenger a, Passenger b) {
        return (a.destinationFloor < b.destinationFloor);
    });
    return max.destinationFloor;
}

void Elevator::checkIfAnyPassengerIsInDestination() {
    bool anyOneInDestination = std::any_of(passengers.begin(), passengers.end(), [=](Passenger passenger) {
        return passenger.isInElevator() && currentFloor == passenger.destinationFloor;
    });

    if (anyOneInDestination) {
        openDoor();
        for (auto &passenger : passengers) {
            if (passenger.isInElevator() && currentFloor == passenger.destinationFloor) {
                passenger.done();
            }
        }
//        std::remove_if(passengers.begin(), passengers.end(), [=](Passenger passenger) {
//            return passenger.getStatus() == DONE;
//        });
        closeDoor();
    }
}

void Elevator::checkIfAnyPassengerWantsGetIn() {
    bool anyOneWantsGetIn = std::any_of(passengers.begin(), passengers.end(), [=](Passenger passenger) {
        return passenger.isWaiting() && currentFloor == passenger.waitFloor;
    });

    bool anyOneInDestination = std::any_of(passengers.begin(), passengers.end(), [=](Passenger passenger) {
        return currentFloor == passenger.destinationFloor;
    });

    if (anyOneWantsGetIn) {
        if (!anyOneInDestination) {
            openDoor();
        }


        for (auto &passenger : passengers) {
            if (passenger.isWaiting() && currentFloor == passenger.waitFloor) {
                passenger.goToElevator();
            }
        }
//        std::remove_if(waitingPassengers.begin(), waitingPassengers.end(), [=](Passenger passenger) {
//            return passenger.getStatus() == DONE;
//        });
        updateDestinationFloor();

        if (!anyOneInDestination) {
            closeDoor();
        }
    }
}

void Elevator::addPassenger(Passenger passenger) {
    //handle dodanie kolejnego

    qInfo() << "Passenger is waiting for elevator at floor" << passenger.waitFloor;

    if (direction == STOP) {
        passengers.push_front(passenger);
        destinationFloor = passenger.waitFloor;
        updateDestinationFloor();
        future = QtConcurrent::run([=] { start(); });
    }

    if (direction == UP) {
        if (passenger.getDirection() == UP) {
            passengers.push_front(passenger);
        } else {
            qInfo() << "Don't add passenger that do not follow elevator direction";
            exit(1);
        }
    }

    if (direction == DOWN) {
        if (passenger.getDirection() == DOWN) {
            passengers.push_front(passenger);
        } else {
            qInfo() << "Don't add passenger that do not follow elevator direction";
            exit(1);
        }
    }
}

void Elevator::updateDestinationFloor() {
    int lowestWaitingFloor = getLowestWaitingFloor();
    int highestWaitingFloor = getHighestWaitingFloor();

    int lowestDestinationFloor = getLowestDestinationFloor();
    int highestDestinationFloor = getHighestDestinationFloor();

    if (isReturning) {
        if (currentFloor == highestDestinationFloor && destinationFloor == lowestDestinationFloor) {
            destinationFloor = currentFloor;
        } else {
            destinationFloor =
                    currentFloor < highestDestinationFloor ? highestDestinationFloor : lowestDestinationFloor;
        }
    } else {
        if (currentFloor == highestWaitingFloor && destinationFloor == lowestWaitingFloor) {
            destinationFloor = currentFloor;
        } else {
            destinationFloor = currentFloor < highestWaitingFloor ? highestWaitingFloor : lowestWaitingFloor;
        }
    }
    qInfo() << "Calculated destination floor as" << destinationFloor;
}

void Elevator::openDoor() {
    qInfo() << "Door opening";
    QThread::msleep(waitDuration);
}

void Elevator::closeDoor() {
    qInfo() << "Door closing";
    QThread::msleep(2 * waitDuration);
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

int Elevator::getCurrentFloor() {
    return this->currentFloor;
}

void Elevator::rerender(int maxFloor) {
    slider->setMaximum(maxFloor);
}

void Elevator::forceStop() {
    qInfo() << "Forcing stop";
    future.cancel();
    future.waitForFinished();
}






