#include <utility>
#include <QtWidgets/QGridLayout>
#include <QtCore/QThread>
#include <QtCore/QFuture>
#include <QtConcurrent>

#include "elevator.h"
#include "src/main/elevatormanager.h"

/**
 * Elevator constructor.
 * Initializes every required property of elevator.
 *
 * @param name Name of elevator.
 * @param minFloor Minimal reachable floor.
 * @param maxFloor Maximum reachable floor.
 * @param slider Visual representation of elevator.
 * @param waitDuration Duration of how log elevator will wait for change floor and open/close doors.
 */
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

/**
 * Starts single ride.
 *
 * It's checked which direction elevator have to ride firstly.
 * Function is ran twice per ride. There is isReturning flag used
 * to determinate if elevator is returning.
 * Within this function currentFloor, and destinationFloor are recalculated indirectly,
 * appropriate functions are invoked.
 */
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

/**
 * Function that based on passenger(s) calculates the lowest floor for waiting passenger(s).
 * @return lowest waiting floor.
 */
int Elevator::getLowestWaitingFloor() {
    Passenger *min = *std::min_element(passengers.begin(), passengers.end(), [=](Passenger *a, Passenger *b) {
        return (a->getWaitFloor() < b->getWaitFloor());
    });
    return min->getWaitFloor();
}

/**
 * Function that based on passenger(s) calculates the highest floor for waiting passenger(s).
 * @return highest waiting floor.
 */
int Elevator::getHighestWaitingFloor() {
    Passenger *max = *std::max_element(passengers.begin(), passengers.end(), [=](Passenger *a, Passenger *b) {
        return (a->getWaitFloor() < b->getWaitFloor());
    });
    return max->getWaitFloor();
}

/**
 * Function that based on passenger(s) calculates the lowest destination floor for passenger(s).
 * @return lowest destination floor.
 */
int Elevator::getLowestDestinationFloor() {
    Passenger *min = *std::min_element(passengers.begin(), passengers.end(), [=](Passenger *a, Passenger *b) {
        return (a->getDestinationFloor() < b->getDestinationFloor());
    });
    return min->getDestinationFloor();
}

/**
 * Function that based on passenger(s) calculates the lowest destination floor for passenger(s).
 * @return lowest destination floor.
 */
int Elevator::getHighestDestinationFloor() {
    Passenger *max = *std::max_element(passengers.begin(), passengers.end(), [=](Passenger *a, Passenger *b) {
        return (a->getDestinationFloor() < b->getDestinationFloor());
    });
    return max->getDestinationFloor();
}

/**
 * Checks if any passenger(s) reaches it's destination floor.
 * If it's fulfilled, then door are open and closed.
 * Passenger(s) are marked as done.
 */
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

/**
 * Checks if any passenger(s) wants to get in to elevator.
 * If it's fulfilled, then door are open and closed.
 * Passenger(s) are marked as in elevator.
 */
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

/**
 * Adds passenger to ride.
 * It's checked if passenger can be handled.
 * In case that it's not, exception is thrown.
 *
 * @param passenger to handle.
 */
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

/**
 * Updates destination floor based on passenger(s).
 */
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

/**
 * Checks if given passenger can be handled in elevator.
 * @param passenger to check.
 * @return result of check.
 */
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

/**
 * Opens the door.
 * It's single waitDuration time required to open the door.
 */
void Elevator::openDoor() {
    qInfo() << "[E]" << "Door opening";
    QThread::msleep(waitDuration);
}

/**
 * Closes the door.
 * It's doubled waitDuration time required to close the door.
 */
void Elevator::closeDoor() {
    qInfo() << "[E]" << "Door closing";
    QThread::msleep(2 * waitDuration);
    pickedUpPassenger();
}

/**
 * Returns direction.
 * @return direction.
 */    /**
     * Returns direction.
     * @return direction.
     */
Direction Elevator::getDirection() {
    return direction;
}

/**
 * Returns name.
 * @return
 */
QString Elevator::getName() {
    return this->name;
}

/**
 * Returns minimal reachable floor.
 * @return minimal reachable floor.
 */
int Elevator::getMinFloor() {
    return this->minFloor;
}

/**
 * Returns maximum reachable floor.
 * @return maximum reachable floor.
 */
int Elevator::getMaxFloor() {
    return this->maxFloor;
}

/**
 * Returns current floor.
 * @return current floor.
 */
int Elevator::getCurrentFloor() {
    return this->currentFloor;
}

/**
 * Rerender visualization of elevator.
 * @param maxFloor Top limit for elevator visualization.
 */
void Elevator::rerender(int maxFloor) {
    slider->setMaximum(maxFloor);
}

/**
 * Used in testing purposes to stop ride future.
 */
void Elevator::forceStop() {
    qInfo() << "[E]" << "Forcing stop";
    future.cancel();
    future.waitForFinished();
}






