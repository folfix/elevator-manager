#include "elevatormanager.h"
#include "ui_elevatormanager.h"
#include <QDebug>

ElevatorManager::ElevatorManager(QWidget *parent) : QMainWindow(parent), ui(new Ui::ElevatorManager) {

    ui->setupUi(this);
}

ElevatorManager::~ElevatorManager() {
    delete ui;
}

void ElevatorManager::on_asd_clicked() {
    qInfo() << "Clicked";
    ui->elevator0->setValue(5);
    addElevator();
}

void ElevatorManager::addElevator() {
    auto *button = new QPushButton(this);
    button->setText("Button ASD");
    ui->horizontalLayout_7->addWidget(button);

    auto *slider = new QSlider(this);
    ui->horizontalLayout->addWidget(slider);

    auto elevator = Elevator("next", 0, 10, slider);
    this->elevators.push_front(elevator);
}

void ElevatorManager::recalculateMaxFloor() {
    for (auto &i : this->elevators) {
        qInfo() << i.getName();
    }
}
