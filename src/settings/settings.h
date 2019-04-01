#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include "src/elevator/elevator.h"


namespace Ui {
    class Settings;
}

/**
 * Representation of settings dialog.
 */
class Settings : public QDialog {
Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);

    ~Settings();

private:
    Ui::Settings *ui;                /**< UI object. */
    std::list<Elevator> elevators;   /**< list of elevators. */
};

#endif // SETTINGS_H
