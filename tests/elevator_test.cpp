#include <QtWidgets/QApplication>
#include "gtest/gtest.h"
#include "elevator.h"
#include "passenger.h"

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
};

TEST_F(ElevatorTest, testInit) {
    auto *slider = new QSlider();
    auto *elevator = new Elevator(QString("testInit"), 0, 10, slider);

    EXPECT_EQ(elevator->getMinFloor(), 0);
    EXPECT_EQ(elevator->getMaxFloor(), 10);
    EXPECT_EQ(elevator->getName(), QString("testInit"));
}

TEST_F(ElevatorTest, testAddPassenger) {
    auto *slider = new QSlider();
    auto elevator = new Elevator(QString("testAddPassenger"), 0, 10, slider);
    auto passenger = Passenger(5, 10);

    elevator->addPassenger(passenger);

}