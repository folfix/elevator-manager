#include <QtWidgets/QApplication>
#include "gtest/gtest.h"
#include "src/elevator/elevator.h"
#include "../src/passenger/passenger.h"
#include "src/elevator/direction.h"

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
    auto *slider = new QSlider();
    auto *elevator = new Elevator(QString("testInit"), 0, 10, slider);

    EXPECT_EQ(elevator->getMinFloor(), 0);
    EXPECT_EQ(elevator->getMaxFloor(), 10);
    EXPECT_EQ(elevator->getName(), QString("testInit"));
}

TEST_F(ElevatorTest, testAddPassengerGoingUpWhenElevatorIsLower) {
    auto *slider = new QSlider();
    auto elevator = new Elevator(QString("testAddPassenger"), 0, 10, slider, waitTime);
    auto passenger = Passenger(2, 4);

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
    auto *slider = new QSlider();
    auto elevator = new Elevator(QString("testAddPassenger"), 0, 10, slider, waitTime);
    auto passenger = Passenger(4, 2);

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
    auto *slider = new QSlider();
    auto elevator = new Elevator(QString("testAddPassenger"), 0, 10, slider, waitTime);
    auto passenger = Passenger(0, 2);

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