#ifndef ELEVATORMANAGER_H
#define ELEVATORMANAGER_H

#include "src/elevator/elevator.h"

#include <QMainWindow>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>

namespace Ui {
    class ElevatorManager;
}

class ElevatorManager : public QMainWindow {
Q_OBJECT

signals:
    void passangerRequestedElevator();

public:
    explicit ElevatorManager(unsigned long waitDuration = 600, QWidget *parent = nullptr);

    ~ElevatorManager() override;

    static const int GROUND_FLOOR_NUMBER = 0;
    Elevator* addElevator(int minFloor, int maxFloor);
    void callElevator(int from, int to);

private:
    Ui::ElevatorManager *ui;
    std::list<Elevator *> elevators;
    std::list<QPushButton *> buttonsEntries;
    std::list<QLabel *> labels;
    std::list<Passenger *> passengers;
    unsigned long waitDuration;

    int maxFloorOverall = GROUND_FLOOR_NUMBER;

    void handlePassengers();
    void recalculateMinMaxFloor();
    void recalculateButtons();
    void openSettings();
    int waitingPassengers(int floor);
};

#endif // ELEVATORMANAGER_H
