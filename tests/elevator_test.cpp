#include <QtWidgets/QApplication>
#include "gtest/gtest.h"
#include "src/elevator/elevator.h"
#include "../src/passenger/passenger.h"
#include "src/elevator/direction.h"

/**
 * Notice that sometimes tests fails because it's real-time system.
 * I such situation re-run tests.
 */
class ElevatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        Test::SetUp();
        int argc = 0;
        char arg = ' ';
        char * argPointer = &arg;
        char **argPointerPointer = &argPointer;
        auto *pApplication = new QApplication(argc, argPointerPointer);
    }

    void waitInit() {
        ::usleep(500 * waitTime);
    }

    void waitBetweenFloors() {
        ::usleep(1000 * waitTime);
    }

    void waitDoorOpenClose() {
        this->waitBetweenFloors();
        this->waitBetweenFloors();
        this->waitBetweenFloors();
    }

    __useconds_t waitTime = 1;
};

TEST_F(ElevatorTest, testInit) {
    auto *slider = new QProgressBar();
    auto *elevator = new Elevator(QString("testInit"), 0, 10, slider);

    EXPECT_EQ(elevator->getMinFloor(), 0);
    EXPECT_EQ(elevator->getMaxFloor(), 10);
    EXPECT_EQ(elevator->getName(), QString("testInit"));
}

TEST_F(ElevatorTest, testAddPassengerGoingUpWhenElevatorIsLower) {
    auto *slider = new QProgressBar();
    auto elevator = new Elevator(QString("testAddPassenger"), 0, 10, slider, waitTime);
    auto *passenger = new Passenger(2, 4);

    EXPECT_EQ(elevator->getDirection(), STOP);
    elevator->addPassenger(passenger);
    waitInit();

    EXPECT_EQ(elevator->getDirection(), UP);

    EXPECT_EQ(elevator->getCurrentFloor(), 0);
    waitBetweenFloors();
    EXPECT_EQ(elevator->getCurrentFloor(), 1);
    waitBetweenFloors();
    EXPECT_EQ(elevator->getCurrentFloor(), 2);
    waitBetweenFloors();

    waitDoorOpenClose();
    EXPECT_EQ(elevator->getCurrentFloor(), 3);
    waitBetweenFloors();
    EXPECT_EQ(elevator->getCurrentFloor(), 4);

    waitDoorOpenClose();
    EXPECT_EQ(elevator->getDirection(), STOP);

    elevator->forceStop();
}

TEST_F(ElevatorTest, testAddPassengerGoingDownWhenElevatorIsLower) {
    auto *slider = new QProgressBar();
    auto elevator = new Elevator(QString("testAddPassenger"), 0, 10, slider, waitTime);
    auto *passenger = new Passenger(4, 2);

    EXPECT_EQ(elevator->getDirection(), STOP);
    elevator->addPassenger(passenger);
    waitInit();

    EXPECT_EQ(elevator->getDirection(), UP);

    EXPECT_EQ(elevator->getCurrentFloor(), 0);
    waitBetweenFloors();
    EXPECT_EQ(elevator->getCurrentFloor(), 1);
    waitBetweenFloors();
    EXPECT_EQ(elevator->getCurrentFloor(), 2);
    waitBetweenFloors();
    EXPECT_EQ(elevator->getCurrentFloor(), 3);
    waitBetweenFloors();
    EXPECT_EQ(elevator->getCurrentFloor(), 4);
    waitBetweenFloors();

    waitDoorOpenClose();
    EXPECT_EQ(elevator->getCurrentFloor(), 3);
    waitBetweenFloors();
    EXPECT_EQ(elevator->getCurrentFloor(), 2);

    waitDoorOpenClose();
    EXPECT_EQ(elevator->getDirection(), STOP);

    elevator->forceStop();
}

TEST_F(ElevatorTest, testAddPassengerGoingDownWhenElevatorIsOnTheSameFloor) {
    auto *slider = new QProgressBar();
    auto elevator = new Elevator(QString("testAddPassenger"), 0, 10, slider, waitTime);
    auto *passenger = new Passenger(0, 2);

    EXPECT_EQ(elevator->getDirection(), STOP);
    elevator->addPassenger(passenger);
    waitInit();
    waitDoorOpenClose();

    EXPECT_EQ(elevator->getDirection(), UP);

    EXPECT_EQ(elevator->getCurrentFloor(), 0);
    waitBetweenFloors();
    EXPECT_EQ(elevator->getCurrentFloor(), 1);
    waitBetweenFloors();
    EXPECT_EQ(elevator->getCurrentFloor(), 2);
    waitBetweenFloors();

    waitDoorOpenClose();
    EXPECT_EQ(elevator->getDirection(), STOP);

    elevator->forceStop();
}

TEST_F(ElevatorTest, testAddSecondPassager) {
    auto *slider = new QProgressBar();
    auto elevator = new Elevator(QString("testAddPassenger"), 0, 10, slider, waitTime);
    auto *passenger1 = new Passenger(0, 5);
    auto *passenger2 = new Passenger(3, 4);

    EXPECT_EQ(elevator->getDirection(), STOP);
    elevator->addPassenger(passenger1);
    waitInit();
    waitDoorOpenClose();

    EXPECT_EQ(elevator->getDirection(), UP);

    EXPECT_EQ(elevator->getCurrentFloor(), 0);
    waitBetweenFloors();
    EXPECT_EQ(elevator->getCurrentFloor(), 1);
    waitBetweenFloors();

    elevator->addPassenger(passenger2);

    EXPECT_EQ(elevator->getCurrentFloor(), 2);
    waitBetweenFloors();
    EXPECT_EQ(elevator->getCurrentFloor(), 3);
    waitBetweenFloors();

    waitDoorOpenClose();

    EXPECT_EQ(elevator->getCurrentFloor(), 4);
    waitBetweenFloors();

    waitDoorOpenClose();

    EXPECT_EQ(elevator->getCurrentFloor(), 5);
    waitBetweenFloors();

    waitDoorOpenClose();

    EXPECT_EQ(elevator->getDirection(), STOP);

    elevator->forceStop();
}

TEST_F(ElevatorTest, testAddThreePassagers) {
    auto *slider = new QProgressBar();
    auto elevator = new Elevator(QString("testAddPassenger"), 0, 10, slider, waitTime);
    auto *passenger1 = new Passenger(0, 5);
    auto *passenger2 = new Passenger(3, 4);
    auto *passenger3 = new Passenger(4, 8);

    EXPECT_EQ(elevator->getDirection(), STOP);
    elevator->addPassenger(passenger1);
    waitInit();
    waitDoorOpenClose();

    EXPECT_EQ(elevator->getDirection(), UP);

    EXPECT_EQ(elevator->getCurrentFloor(), 0);
    waitBetweenFloors();
    EXPECT_EQ(elevator->getCurrentFloor(), 1);
    waitBetweenFloors();

    elevator->addPassenger(passenger2);

    EXPECT_EQ(elevator->getCurrentFloor(), 2);
    waitBetweenFloors();

    elevator->addPassenger(passenger3);

    EXPECT_EQ(elevator->getCurrentFloor(), 3);
    waitBetweenFloors();

    waitDoorOpenClose();

    EXPECT_EQ(elevator->getCurrentFloor(), 4);
    waitBetweenFloors();

    waitDoorOpenClose();

    EXPECT_EQ(elevator->getCurrentFloor(), 5);
    waitBetweenFloors();

    waitDoorOpenClose();

    EXPECT_EQ(elevator->getCurrentFloor(), 6);
    waitBetweenFloors();
    EXPECT_EQ(elevator->getCurrentFloor(), 7);
    waitBetweenFloors();
    EXPECT_EQ(elevator->getCurrentFloor(), 8);
    waitBetweenFloors();

    waitDoorOpenClose();

    EXPECT_EQ(elevator->getDirection(), STOP);

    elevator->forceStop();
}