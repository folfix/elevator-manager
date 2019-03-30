#include <QtWidgets/QApplication>
#include <src/main/elevatormanager.h>
#include "gtest/gtest.h"
#include "src/elevator/elevator.h"
#include "../src/passenger/passenger.h"
#include "src/elevator/direction.h"


class ElevatorManagerTest : public ::testing::Test {
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

TEST_F(ElevatorManagerTest, testInit) {
    auto *manager = new ElevatorManager(waitTime);

    Elevator *elevator1 = manager->addElevator(0, 10);
    Elevator *elevator2 = manager->addElevator(0, 10);

    manager->callElevator(2, 5);
    manager->callElevator(1, 10);
    manager->callElevator(7, 3);
    manager->callElevator(10, 2);
    waitInit();

    EXPECT_NE(elevator1->getDirection(), STOP);
    EXPECT_EQ(elevator2->getDirection(), STOP);
}
