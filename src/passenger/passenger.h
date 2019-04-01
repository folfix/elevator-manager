#ifndef PASSENGER_H
#define PASSENGER_H

#include <QtCore/QDebug>
#include "src/elevator/direction.h"
#include "ostream"


enum PassengerStatus {NEW, WAITING_FOR_ELEVATOR, IN_ELEVATOR, DONE};

/**
 * Representation of the passenger.
 */
class Passenger {

private:
    PassengerStatus status;   /**< Status of passenger.*/
    int waitFloor;            /**< Floor that passengers is waiting on.*/
    int destinationFloor;     /**< Destination floor that passenger wants to get on.*/

public:
    /**
     * Constructor of passenger.
     *
     * @param waitFloor Floor that passenger is waiting on.
     * @param destinationFloor Destination floor of ride.
     */
    Passenger(int waitFloor, int destinationFloor);

    /**
     * Marks passenger as in elevator.
     */
    void goToElevator();

    /**
     * Marks passenger as done.
     */
    void done();

    /**
     * Mark passenger as waiting, but elevator has been allocated.
     */
    void elevatorAllocated();

    /**
     * Returns direction of ride.
     * @return
     */
    Direction getDirection();

    /**
     * Returns if passenger is waiting (apart from elevator allocation).
     * @return if passenger is waiting.
     */
    bool isWaiting();

    /**
     * Returns if passenger is waiting on specified floor (apart from elevator allocation).
     * @param floor to check.
     * @return if passenger is waiting on specified floor.
     */
    bool isWaiting(int floor);

    /**
     * Returns if passenger is in elevator.
     * @return if passenger is in elevator.
     */
    bool isInElevator();

    /**
     * Returns if passenger has NOT allocated elevator.
     * @return if passenger has NOT allocated elevator.
     */
    bool hasNotAllocatedElevator();

    /**
     * Returns if passenger has finished ride and it's done.
     * @return if ride is done.
     */
    bool isDone();

    /**
     * Returns floor that passenger is waiting on.
     * @return waiting floor.
     */
    int getWaitFloor();

    /**
     * Returns the destination floor of passenger's ride.
     * @return destination floor.
     */
    int getDestinationFloor();

    friend bool operator==(const Passenger & lhs, const Passenger & rhs);
    friend QDebug& operator<< (QDebug& stream, const Passenger& p);
};

#endif // PASSENGER_H
