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

/**
 * Main class of whole application.
 * Subclass of QMainWindow.
 * Holds application state and manages elevators and it's passengers.
 */
class ElevatorManager : public QMainWindow {
Q_OBJECT

signals:
    /**
     * Signal used for indicate that passenger requested elevator.
     */
    void passengerRequestedElevator();

public:
    /**
     * Constructor of elevator manager.
     *
     * @param waitDuration Time that elevator waits between floor and when opening/closing doors.
     * @param parent QWidget parent (optional).
     */
    explicit ElevatorManager(unsigned long waitDuration = 600, QWidget *parent = nullptr);

    /**
     * Destructor.
     */
    ~ElevatorManager() override;

    static const int GROUND_FLOOR_NUMBER = 0; /**< Ground floor constant*/

    /**
     * Allows adding elevator do GUI.
     * @param minFloor defines minimum reachable floor.
     * @param maxFloor defines maximum reachable floor.
     * @return created elevator.
     */
    Elevator* addElevator(int minFloor, int maxFloor);

    /**
     * Calls elevator based on given from/to floors.
     * @param from floor.
     * @param to floor.
     */
    void callElevator(int from, int to);

private:
    Ui::ElevatorManager *ui;                       /**< UI main element.*/
    std::list<Elevator *> elevators;               /**< List of elevators that are displayed and available.*/
    std::list<QPushButton *> buttonsEntries;       /**< Helper list of buttons used for calling elevator.*/
    std::list<QLabel *> labels;                    /**< Helper list of labels used to display number of waiting passengers.*/
    std::list<Passenger *> passengers;             /**< Helper list that store every passenger that was handled within manager.*/
    unsigned long waitDuration;                    /**< Time that elevator waits between floor and when opening/closing doors.*/

    int maxFloorOverall = GROUND_FLOOR_NUMBER;     /**< Maximum floor that elevators reach.*/

    /**
     * Handles waiting passengers.
     */
    void handlePassengers();

    /**
     * Rerender elevator(s) view.
     * Used when elevator is added.
     */
    void rerenderElevators();

    /**
     * Rerender buttons for calling elevator,
     * and labels of waiting passengers.
     * Used when elevator is added.
     */
    void rerenderCallButtonsAndLabels();

    /**
     * Opens settings dialog.
     */
    void openSettings();

    /**
     * Returns number of waiting passengers.
     * @param floor to check.
     * @return passengers that wait on given floor.
     */
    int waitingPassengers(int floor);
};

#endif // ELEVATORMANAGER_H
