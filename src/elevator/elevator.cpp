#include <utility>
#include <QtWidgets/QGridLayout>
#include <QtCore/QThread>
#include <QtCore/QFuture>
#include <QtConcurrent>

#include "elevator.h"
#include "src/main/elevatormanager.h"


Elevator::Elevator(QString name, int minFloor, int maxFloor, QProgressBar *slider, unsigned long waitDuration) {
    this->name = std::move(name);
    this->minFloor = minFloor;
    this->maxFloor = maxFloor;
    this->slider = slider;
    this->slider->setMinimum(ElevatorManager::GROUND_FLOOR_NUMBER);
    this->slider->setOrientation(Qt::Orientation::Vertical);
    this->waitDuration = waitDuration;
    direction = STOP;
    currentFloor = 0;
    isReturning = false;
}

void Elevator::start() {
    if (currentFloor == destinationFloor) {
        qInfo() << "[E]" << "Elevator on destination floor";
        checkIfAnyPassengerIsInDestination();
        checkIfAnyPassengerWantsGetIn();
    } else if (currentFloor < destinationFloor) {
        direction = UP;
        qInfo() << "[E]" << "Elevator going up";
        while (currentFloor < destinationFloor) {
            QThread::msleep(waitDuration);
            currentFloor++;
            updateView();
            qInfo() << "[E]" << "Elevator floor" << currentFloor;
            checkIfAnyPassengerIsInDestination();
            checkIfAnyPassengerWantsGetIn();
        }

    } else {
        direction = DOWN;
        qInfo() << "[E]" << "Elevator going down";
        while (currentFloor > destinationFloor) {
            QThread::msleep(waitDuration);
            currentFloor--;
            updateView();
            qInfo() << "[E]" << "Elevator floor" << currentFloor;
            checkIfAnyPassengerIsInDestination();
            checkIfAnyPassengerWantsGetIn();
        }
    }

    if (isReturning) {
        qInfo() << "[E]" << "Elevator reached destination floor" << destinationFloor;
        direction = STOP;
        passengers.clear();
        isReturning = false;
        finishedTransfer();
    } else {
        isReturning = true;
        updateDestinationFloor();
        start();
    }
}

int Elevator::getLowestWaitingFloor() {
    Passenger *min = *std::min_element(passengers.begin(), passengers.end(), [=](Passenger *a, Passenger *b) {
        return (a->getWaitFloor() < b->getWaitFloor());
    });
    return min->getWaitFloor();
}

int Elevator::getHighestWaitingFloor() {
    Passenger *max = *std::max_element(passengers.begin(), passengers.end(), [=](Passenger *a, Passenger *b) {
        return (a->getWaitFloor() < b->getWaitFloor());
    });
    return max->getWaitFloor();
}

int Elevator::getLowestDestinationFloor() {
    Passenger *min = *std::min_element(passengers.begin(), passengers.end(), [=](Passenger *a, Passenger *b) {
        return (a->getDestinationFloor() < b->getDestinationFloor());
    });
    return min->getDestinationFloor();
}

int Elevator::getHighestDestinationFloor() {
    Passenger *max = *std::max_element(passengers.begin(), passengers.end(), [=](Passenger *a, Passenger *b) {
        return (a->getDestinationFloor() < b->getDestinationFloor());
    });
    return max->getDestinationFloor();
}

void Elevator::checkIfAnyPassengerIsInDestination() {
    bool anyOneInDestination = std::any_of(passengers.begin(), passengers.end(), [=](Passenger *passenger) {
        return passenger->isInElevator() && currentFloor == passenger->getDestinationFloor();
    });

    if (anyOneInDestination) {
        openDoor();
        for (auto &passenger : passengers) {
            if (passenger->isInElevator() && currentFloor == passenger->getDestinationFloor()) {
                passenger->done();
            }
        }
        closeDoor();
    }
}

void Elevator::checkIfAnyPassengerWantsGetIn() {
    bool anyOneWantsGetIn = std::any_of(passengers.begin(), passengers.end(), [=](Passenger *passenger) {
        return passenger->isWaiting(currentFloor) && (passenger->getDirection() == direction || passenger->getWaitFloor() == destinationFloor);
    });

    bool anyOneInDestination = std::any_of(passengers.begin(), passengers.end(), [=](Passenger *passenger) {
        return currentFloor == passenger->getDestinationFloor();
    });

    if (anyOneWantsGetIn) {
        if (!anyOneInDestination) {
            openDoor();
        }

        for (auto &passenger : passengers) {
            if (passenger->isWaiting(currentFloor)) {
                passenger->goToElevator();
            }
        }
        updateDestinationFloor();

        if (!anyOneInDestination) {
            closeDoor();
        }
    }
}

void Elevator::addPassenger(Passenger *passenger) {
    qInfo() << "[E]" << "Passenger is waiting for elevator at floor" << passenger->getWaitFloor();

    if (!canHandle(passenger)) {
        throw std::invalid_argument("Don't add passenger when it's not handled");
    }

    passengers.push_front(passenger);

    if (direction == STOP) {
        destinationFloor = passenger->getWaitFloor();
        updateDestinationFloor();
        future = QtConcurrent::run([=] { start(); });
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
    qInfo() << "[E]" << "Calculated destination floor as" << destinationFloor;
}

bool Elevator::canHandle(Passenger *passenger) {
    if (passenger->getWaitFloor() != 0 &&
        (passenger->getWaitFloor() < minFloor || passenger->getWaitFloor() > maxFloor)) {
        return false;
    }

    if (passenger->getDestinationFloor() != 0 &&
        (passenger->getDestinationFloor() < minFloor || passenger->getDestinationFloor() > maxFloor)) {
        return false;
    }

    if (direction == STOP) {
        return true;
    }

    if (direction == UP) {
        if (isReturning) {
            return passenger->getDirection() == UP && passenger->getWaitFloor() > currentFloor;
        } else {
            return passenger->getDirection() == DOWN && passenger->getWaitFloor() <= destinationFloor;
        }
    }

    if (direction == DOWN) {
        if (isReturning) {
            return passenger->getDirection() == DOWN && passenger->getWaitFloor() < currentFloor;
        } else {
            return passenger->getDirection() == UP && passenger->getWaitFloor() >= destinationFloor;
        }
    }
}

void Elevator::openDoor() {
    qInfo() << "[E]" << "Door opening";
    QThread::msleep(waitDuration);
}

void Elevator::closeDoor() {
    qInfo() << "[E]" << "Door closing";
    QThread::msleep(2 * waitDuration);
    pickedUpPassager();
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
    qInfo() << "[E]" << "Forcing stop";
    future.cancel();
    future.waitForFinished();
}






