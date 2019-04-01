#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QSlider>
#include <QString>
#include <QFuture>
#include <QtWidgets/QProgressBar>
#include "direction.h"
#include "../passenger/passenger.h"

/**
 * Representation of elevator with it's logic.
 * Sub-class of QObject
 */
class Elevator : public QObject {
Q_OBJECT

signals:

    /**
     * Signal sent when it's required to update view of elevator.
     */
    void updateView();

    /**
     * Signal sent when elevator finishes it's transfer.
     */
    void finishedTransfer();

    /**
     * Signal sent when passenger has been picked.
     */
    void pickedUpPassenger();

private:
    QString name;                        /**< Name of elevator.*/
    int minFloor;                        /**< Minimal floor that elevator reaches.*/
    int maxFloor;                        /**< Maximum floor that elevator reaches.*/
    int currentFloor;                    /**< Current floor.*/
    int destinationFloor;                /**< Temporary destination floor, recalculated when new passenger gets in to elevator.*/
    Direction direction;                 /**< Temporary direction, recalculated when destination floor is reached.*/
    std::list<Passenger *> passengers;   /**< List of passengers that elevator is carrying or that waits for elevator.*/
    bool isReturning;                    /**< Flag representing if elevator is on return way (in context of single ride).*/
    unsigned long waitDuration;          /**< Time that elevator waits between floor and when opening/closing doors.*/

    QProgressBar *slider;                /**< Visual (Qt) representation of elevator.*/
    QFuture<void> future;                /**< Future that handle single ride.*/


    /**
     * Starts single ride.
     */
    void start();

    /**
     * Opens doors.
     */
    void openDoor();

    /**
     * Closes doors.
     */
    void closeDoor();

    /**
     * Checks if any passenger(s) is in destination floor.
     * Handles opening and closing door if so.
     */
    void checkIfAnyPassengerIsInDestination();

    /**
     * Checks if any passenger(s) wants to get in.
     * Handles opening and closing door if so.
     */
    void checkIfAnyPassengerWantsGetIn();

    /**
     * Calculates the lowest destination floor based on passengers.
     * @return lowest destination floor
     */
    int getLowestDestinationFloor();

    /**
     * Calculates the highest destination floor based on passengers.
     * @return highest destination floor
     */
    int getHighestDestinationFloor();

    /**
     * Calculates the lowest waiting floor based on waiting passengers.
     * @return lowest waiting floor
     */
    int getLowestWaitingFloor();

    /**
     * Calculates the highest waiting floor based on waiting passengers.
     * @return highest waiting floor
     */
    int getHighestWaitingFloor();

    /**
     * Updates destination floor based on passengers.
     */
    void updateDestinationFloor();


public:
    /**
     * Elevator constructor.
     *
     * @param name Name of elevator.
     * @param minFloor Minimal reachable floor.
     * @param maxFloor Maximum reachable floor.
     * @param slider Visual representation of elevator.
     * @param waitDuration Duration of how log elevator will wait for change floor and open/close doors.
     */
    Elevator(QString name, int minFloor, int maxFloor, QProgressBar *slider, unsigned long waitDuration = 1000);

    /**
     * Returns name.
     * @return
     */
    QString getName();

    /**
     * Returns minimal reachable floor.
     * @return minimal reachable floor.
     */
    int getMinFloor();

    /**
     * Returns maximum reachable floor.
     * @return maximum reachable floor.
     */
    int getMaxFloor();

    /**
     * Returns current floor.
     * @return current floor.
     */
    int getCurrentFloor();

    /**
     * Rerender visualization of elevator.
     * @param maxFloor Top limit for elevator visualization.
     */
    void rerender(int maxFloor);

    /**
     * Returns direction.
     * @return direction.
     */
    Direction getDirection();

    /**
     * Adds passengers to elevator.
     * @param passenger to add.
     */
    void addPassenger(Passenger *passenger);

    /**
     * Checks if elevator can handle given passenger.
     * @param passenger to check.
     * @return check result.
     */
    bool canHandle(Passenger *passenger);

    /**
     * Used in testing purposes to stop ride future.
     */
    void forceStop();
};


#endif // ELEVATOR_H
