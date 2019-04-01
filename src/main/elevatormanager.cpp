#include <QtWidgets/QLCDNumber>
#include "elevatormanager.h"
#include "ui_elevatormanager.h"
#include "src/settings/settings.h"

ElevatorManager::ElevatorManager(unsigned long waitDuration, QWidget *parent) : QMainWindow(parent),
                                                                                ui(new Ui::ElevatorManager) {
    ui->setupUi(this);
    this->setFixedSize(1250, 732);
    this->statusBar()->setSizeGripEnabled(false);
    this->waitDuration = waitDuration;

    connect(ui->settingsButton, &QAction::triggered, this, [=] { openSettings(); });

    addElevator(0, 10);
}

ElevatorManager::~ElevatorManager() {
    delete ui;
}

Elevator *ElevatorManager::addElevator(int minFloor, int maxFloor) {
    this->maxFloorOverall = this->maxFloorOverall < maxFloor ? maxFloor : this->maxFloorOverall;

    auto lay = new QVBoxLayout();
    auto lcd = new QLCDNumber();
    lcd->setFixedHeight(40);
    lcd->setFixedWidth(25);
    lcd->setDigitCount(2);
    lcd->setSegmentStyle(QLCDNumber::SegmentStyle::Flat);
    lay->addWidget(lcd);
    auto *slider = new QProgressBar();
    lay->addWidget(slider);
    ui->elevatorsView->addLayout(lay);

    auto elevator = new Elevator(QString::number(elevators.size()), minFloor, maxFloor, slider, waitDuration);
    connect(elevator, &Elevator::updateView, this, [=] { slider->setValue(elevator->getCurrentFloor()); });
    connect(elevator, &Elevator::updateView, this, [=] { lcd->display(elevator->getCurrentFloor()); });
    connect(elevator, &Elevator::finishedTransfer, this, [=] { handlePassengers(); });
    connect(elevator, &Elevator::pickedUpPassager, this, [=] { handlePassengers(); });

    this->elevators.push_front(elevator);

    recalculateMinMaxFloor();
    recalculateButtons();
    return elevator;
}

void ElevatorManager::recalculateMinMaxFloor() {
    for (auto &elevator : this->elevators) {
        elevator->rerender(this->maxFloorOverall);
    }
}

void ElevatorManager::recalculateButtons() {
    for (auto &i : buttonsEntries) {
        delete i;
    }

    buttonsEntries.clear();

    for (auto &l : labels) {
        delete l;
    }

    labels.clear();

    for (int fromFloor = maxFloorOverall; fromFloor >= GROUND_FLOOR_NUMBER; fromFloor--) {
        auto lay = new QHBoxLayout();
        lay->setObjectName("Layout" + QString::number(fromFloor));
        for (int toFloor = GROUND_FLOOR_NUMBER; toFloor <= maxFloorOverall; toFloor++) {
            auto *button = new QPushButton();
            button->setText(QString::number(fromFloor) + " -> " + QString::number(toFloor));
            button->setObjectName("Button" + QString::number(fromFloor) + "/" + QString::number(toFloor));
            button->setSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Preferred);
            button->setDisabled(fromFloor == toFloor);
            lay->addWidget(button);
            connect(button, &QPushButton::clicked, this, [=] { callElevator(fromFloor, toFloor); });
            buttonsEntries.push_front(button);
        }
        ui->buttonsView->addLayout(lay);

        auto *label = new QLabel();
        label->setText(QString("0"));
        ui->waitingList->addWidget(label);
        labels.push_front(label);
        connect(this, &ElevatorManager::passangerRequestedElevator, label, [=] {
            label->setText(QString::number(waitingPassengers(fromFloor)));
        });
    }
}

void ElevatorManager::callElevator(int from, int to) {
    auto *passenger = new Passenger(from, to);
    passengers.push_front(passenger);
    qInfo() << "[M]" << "Called elevator:" << from << "->" << to << "[passengersCount:" << passengers.size() << "]";
    handlePassengers();
}

void ElevatorManager::handlePassengers() {
    qInfo() << "[M]" << "Invoked handlePassengers()";
    passangerRequestedElevator();
    for (auto passenger : passengers) {
        std::list<Elevator *> availableElevators;
        std::copy_if(elevators.begin(), elevators.end(), std::back_inserter(availableElevators),
                     [=](Elevator *e) { return e->canHandle(passenger); });

        if (passenger->hasNotAllocatedElevator()) {
            if (!availableElevators.empty()) {
                Elevator *closestElevator = availableElevators.front();
                for (auto &elevator : availableElevators) {
                    int oldDistance = abs(closestElevator->getCurrentFloor() - passenger->getWaitFloor());
                    int distance = abs(elevator->getCurrentFloor() - passenger->getWaitFloor());
                    if (distance < oldDistance) {
                        closestElevator = elevator;
                    }
                }
                closestElevator->addPassenger(passenger);
                passenger->elevatorAllocated();
                qInfo() << "[M]" << "Handled passenger" << *passenger;
            } else {
                qInfo() << "[M]" << "No elevator left" << *passenger;
            }
        }
    }
}

void ElevatorManager::openSettings() {
    auto *dialog = new Settings(this);
    dialog->exec();
}

int ElevatorManager::waitingPassengers(int floor) {
    int count = 0;
    for (auto passenger : passengers) {
        if (passenger->isWaiting(floor)) {
            count++;
        }
    }
    return count;
}
