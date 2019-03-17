#include "elevatormanager.h"
#include "ui_elevatormanager.h"
#include "elevator.h"
#include <QDebug>

ElevatorManager::ElevatorManager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ElevatorManager)
{
    ui->setupUi(this);
}

ElevatorManager::~ElevatorManager()
{
    delete ui;
}

void ElevatorManager::on_callElevatorButton_clicked()
{
    qInfo() << "C++ Style Info Message";

    ui->elevator0->setValue(5);
}
