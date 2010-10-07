#include "lsmainwindow.h"

LSMainWindow::LSMainWindow(QWidget *parent) : QMainWindow(parent) {
    ui.setupUi(this);
}

void LSMainWindow::changeEvent(QEvent *e) {
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui.retranslateUi(this);
        break;
    default:
        break;
    }
}
