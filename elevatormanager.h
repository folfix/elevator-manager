#ifndef ELEVATORMANAGER_H
#define ELEVATORMANAGER_H

#include <QMainWindow>

namespace Ui {
class ElevatorManager;
}

class ElevatorManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit ElevatorManager(QWidget *parent = nullptr);
    ~ElevatorManager();

private slots:
    void on_callElevatorButton_clicked();

private:
    Ui::ElevatorManager *ui;
};

#endif // ELEVATORMANAGER_H
