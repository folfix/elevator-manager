#include "settings.h"
#include "ui_settings.h"

/**
 * Settings constructor.
 * @param parent of dialog.
 */
Settings::Settings(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::Settings) {
    ui->setupUi(this);
    this->setWindowTitle("Settings");
}

/**
 * Destructor.
 */
Settings::~Settings() {
    delete ui;
}
