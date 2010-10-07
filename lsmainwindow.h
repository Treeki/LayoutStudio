#ifndef LSMAINWINDOW_H
#define LSMAINWINDOW_H

#include "ui_lsmainwindow.h"

class LSMainWindow : public QMainWindow {
    Q_OBJECT
public:
    LSMainWindow(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::LSMainWindow ui;
};

#endif // LSMAINWINDOW_H
