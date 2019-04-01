#include "src/main/elevatormanager.h"
#include <QApplication>

/**
 * Entry-point for application.
 * Starts Qt UI.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ElevatorManager w;
    w.show();
    return a.exec();
}
