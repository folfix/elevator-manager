#include "elevatormanager.h"
#include "ui_elevatormanager.h"
#include "elevator.h"
#include "manager.h"
#include <QDebug>

ElevatorManager::ElevatorManager(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::ElevatorManager) {
    ui->setupUi(this);
}

ElevatorManager::~ElevatorManager() {
    delete ui;
}

void ElevatorManager::on_asd_clicked() {
    qInfo() << "Clicked";
    ui->elevator0->setValue(5);

    auto *button = new QPushButton(this); // Create a dynamic button object
    button->setText("Button ASD");
    ui->horizontalLayout_7->addWidget(button);

}
