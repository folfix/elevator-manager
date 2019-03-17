#include "elevatormanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ElevatorManager w;
    w.show();

    return a.exec();
}
