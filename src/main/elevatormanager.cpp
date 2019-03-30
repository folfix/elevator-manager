#include <QtWidgets/QLCDNumber>
#include "elevatormanager.h"
#include "ui_elevatormanager.h"
#include "src/settings/settings.h"

ElevatorManager::ElevatorManager(QWidget *parent) : QMainWindow(parent), ui(new Ui::ElevatorManager) {
    ui->setupUi(this);
    this->setFixedSize(1250, 732);
    this->statusBar()->setSizeGripEnabled(false);

    connect(ui->settingsButton, &QAction::triggered, this, [=] { openSettings(); });

    addElevator(0, 10);
    addElevator(0, 10);
    addElevator(0, 10);
}

ElevatorManager::~ElevatorManager() {
    delete ui;
}

void ElevatorManager::addElevator(int minFloor, int maxFloor) {
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

    auto elevator = new Elevator(QString::number(elevators.size()), minFloor, maxFloor, slider, 400);
    connect(elevator, &Elevator::updateView, this, [=] { slider->setValue(elevator->getCurrentFloor()); });
    connect(elevator, &Elevator::updateView, this, [=] { lcd->display(elevator->getCurrentFloor()); });

    this->elevators.push_front(elevator);

    recalculateMinMaxFloor();
    recalculateButtons();
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
    }
}

void ElevatorManager::callElevator(int from, int to) {
    qInfo() << "Called elevator:" << from << "->" << to;
    const Passenger &passenger = Passenger(from, to);
    passengers.push_back(passenger);
    handlePassengers();
}

void ElevatorManager::handlePassengers() {
//    for (auto &passenger : this->passengers) {

    auto passenger = passengers.begin();
    while (passenger != passengers.end()) {

        std::list<Elevator *> availableElevators;
        std::copy_if(elevators.begin(), elevators.end(), std::back_inserter(availableElevators),
                     [=](Elevator *e) { return e->canHandle(*passenger); });

        if (!availableElevators.empty()) {
            Elevator *closestElevator = availableElevators.front();
            for (auto &elevator : availableElevators) {
                int oldDistance = abs(closestElevator->getCurrentFloor() - passenger->waitFloor);
                int distance = abs(elevator->getCurrentFloor() - passenger->waitFloor);
                if (distance < oldDistance) {
                    closestElevator = elevator;
                }
            }
            closestElevator->addPassenger(*passenger);
            passenger = passengers.erase(passenger);
        } else {
            passenger++;
        }
    }
}

void ElevatorManager::openSettings() {
    auto *dialog = new Settings(this);
    dialog->exec();
}
