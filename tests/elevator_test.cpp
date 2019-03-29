#include "gtest/gtest.h"
#include "elevator.h"

TEST(basic_test, eq) {
    QApplication a();
    auto *slider = new QSlider();
    auto * elevator = new Elevator(QString("test"), 0, 10, slider);

    EXPECT_EQ(elevator->getMinFloor(), 0);
    EXPECT_EQ(elevator->getMaxFloor(), 10);
}