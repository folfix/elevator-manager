#ifndef ELEVATORMANAGER_H
#define ELEVATORMANAGER_H

#include "elevator.h"

#include <QMainWindow>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>

namespace Ui {
    class ElevatorManager;
}

class ElevatorManager : public QMainWindow {
Q_OBJECT

public:
    explicit ElevatorManager(QWidget *parent = nullptr);

    ~ElevatorManager() override;

    static const int GROUND_FLOOR_NUMBER = 0;

private slots:

    void on_addElevatorButton_clicked();

    void on_settingsButton_triggered();

    void callElevator(int from, int to);

private:
    Ui::ElevatorManager *ui;
    std::list<Elevator> elevators;
    std::list<QPushButton *> buttonsEntries;

    int maxFloorOverall = GROUND_FLOOR_NUMBER;

    void addElevator(int minFloor, int maxFloor);

    void recalculateMinMaxFloor();

    void recalculateButtons();
};

#endif // ELEVATORMANAGER_H
