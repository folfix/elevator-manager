#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include "elevator.h"


namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private:
    Ui::Settings *ui;
    std::list<Elevator> elevators;
};

#endif // SETTINGS_H
