#ifndef ELEVATORMANAGER_H
#define ELEVATORMANAGER_H

#include "elevator.h"

#include <QMainWindow>

namespace Ui {
class ElevatorManager;
}

class ElevatorManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit ElevatorManager(QWidget *parent = nullptr);
    ~ElevatorManager();

private slots:
    void on_asd_clicked();

private:
    Ui::ElevatorManager *ui;
    std::list<Elevator> elevators;
    void addElevator();
    void recalculateMaxFloor();
};

#endif // ELEVATORMANAGER_H
