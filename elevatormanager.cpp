#include "elevatormanager.h"
#include "ui_elevatormanager.h"
#include "settings.h"
#include "qdebug.h"

ElevatorManager::ElevatorManager(QWidget *parent) : QMainWindow(parent), ui(new Ui::ElevatorManager) {

    ui->setupUi(this);
    this->setFixedSize(1100, 600);
    this->statusBar()->setSizeGripEnabled(false);

//    ui->buttonsView->addStretch(-10);
    addElevator(0, 2);
    addElevator(2, 3);
    addElevator(0, 5);
}

ElevatorManager::~ElevatorManager() {
    delete ui;
}

void ElevatorManager::addElevator(int minFloor, int maxFloor) {
    this->maxFloorOverall = this->maxFloorOverall < maxFloor ? maxFloor : this->maxFloorOverall;

    auto *slider = new QSlider();
    ui->elevatorsView->addWidget(slider);

    auto elevator = Elevator(QString::number(elevators.size()), minFloor, maxFloor, slider);
    this->elevators.push_front(elevator);

    recalculateMinMaxFloor();
    recalculateButtons();
}

void ElevatorManager::recalculateMinMaxFloor() {
    for (auto &i : this->elevators) {
        i.rerender(this->maxFloorOverall);
    }
}

void ElevatorManager::recalculateButtons() {
    for (auto &i : buttonsEntries) {
        delete i;
    }

    buttonsEntries.clear();

    for (int fromFloor = maxFloorOverall; fromFloor >= GROUND_FLOOR_NUMBER; fromFloor--) {
        auto lay = new QHBoxLayout();
        lay->setObjectName("Layout" + QString::number(fromFloor));
        for (int toFloor = GROUND_FLOOR_NUMBER; toFloor <= maxFloorOverall; toFloor++) {
            auto *button = new QPushButton();
            button->setText("Floor" + QString::number(fromFloor) + "/" + QString::number(toFloor));
            button->setObjectName("Button" + QString::number(fromFloor) + "/" + QString::number(toFloor));
            button->setSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Preferred);
            button->setDisabled(fromFloor == toFloor);
            lay->addWidget(button);
            connect(button, &QAbstractButton::clicked, this, [=]{ callElevator(fromFloor, toFloor); });
            buttonsEntries.push_front(button);
        }
        ui->buttonsView->addLayout(lay);
    }
}


void ElevatorManager::callElevator(int from, int to) {
    qInfo() << "Called elevator:" << from << "->" << to;
}

void ElevatorManager::on_addElevatorButton_clicked() {
    addElevator(ui->minFloorInput->value(), ui->maxFloorInput->value());
}

void ElevatorManager::on_settingsButton_triggered() {
    auto *dialog = new Settings(this);
    dialog->exec();
}
