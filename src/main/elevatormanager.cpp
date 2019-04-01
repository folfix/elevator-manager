#include <QtWidgets/QLCDNumber>
#include "elevatormanager.h"
#include "ui_elevatormanager.h"
#include "src/settings/settings.h"

/**
 * Constructor of elevator manager.
 * Initiate all required properties which include window size.
 *
 * @param waitDuration Time that elevator waits between floor and when opening/closing doors.
 * @param parent QWidget parent (optional).
 */
ElevatorManager::ElevatorManager(unsigned long waitDuration, QWidget *parent) : QMainWindow(parent),
                                                                                ui(new Ui::ElevatorManager) {
    ui->setupUi(this);
    this->setFixedSize(1250, 732);
    this->statusBar()->setSizeGripEnabled(false);
    this->waitDuration = waitDuration;

    connect(ui->settingsButton, &QAction::triggered, this, [=] { openSettings(); });

    addElevator(0, 10); /**< Add example elevator for a test.*/
    addElevator(0, 10); /**< Add example elevator for a test.*/
    addElevator(0, 10); /**< Add example elevator for a test.*/
}

/**
 * Destructor.
 */
ElevatorManager::~ElevatorManager() {
    delete ui;
}

/**
 * Allows adding elevator do GUI.
 * It creates all required UI elements, and attaches it to the GUI.
 * There are also signal/slot connections established.
 *
 * @param minFloor defines minimum reachable floor.
 * @param maxFloor defines maximum reachable floor.
 * @return created elevator.
 */
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
    connect(elevator, &Elevator::pickedUpPassenger, this, [=] { handlePassengers(); });

    this->elevators.push_front(elevator);

    rerenderElevators();
    rerenderCallButtonsAndLabels();
    return elevator;
}

/**
 * Rerender elevator(s) view.
 * Used when elevator is added.
 * It delegated to the elevator itself.
 */
void ElevatorManager::rerenderElevators() {
    for (auto &elevator : this->elevators) {
        elevator->rerender(this->maxFloorOverall);
    }
}

/**
 * Rerender buttons for calling elevator,
 * and labels of waiting passengers.
 * Used when elevator is added.
 * All required UI elements are added to GUI and signal/slot connections.
 */
void ElevatorManager::rerenderCallButtonsAndLabels() {
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
        connect(this, &ElevatorManager::passengerRequestedElevator, label, [=] {
            label->setText(QString::number(waitingPassengers(fromFloor)));
        });
    }
}

/**
 * Used for call elevator with given from/to floors.
 * @param from floor.
 * @param to floor.
 */
void ElevatorManager::callElevator(int from, int to) {
    auto *passenger = new Passenger(from, to);
    passengers.push_front(passenger);
    qInfo() << "[M]" << "Called elevator:" << from << "->" << to << "[passengersCount:" << passengers.size() << "]";
    handlePassengers();
}

/**
 * Handles waiting passengers.
 * Checks if passenger has not allocated elevator
 * and then starts to find the most optimal elevator (based on it's position).
 */
void ElevatorManager::handlePassengers() {
    qInfo() << "[M]" << "Invoked handlePassengers()";
    passengerRequestedElevator();
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

/**
 * Opens settings dialog.
 */
void ElevatorManager::openSettings() {
    auto *dialog = new Settings(this);
    dialog->exec();
}

/**
 * Returns number of waiting passengers.
 * @param floor to check.
 * @return passengers that wait on given floor.
 */
int ElevatorManager::waitingPassengers(int floor) {
    int count = 0;
    for (auto passenger : passengers) {
        if (passenger->isWaiting(floor)) {
            count++;
        }
    }
    return count;
}
