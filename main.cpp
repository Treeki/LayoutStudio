#include <QtGui/QApplication>
#include "lsmainwindow.h"
#include "lsglobals.h"

#include "lyt/directorypackage.h"
#include "lyt/layout.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

	LSGlobals::setup();

	LYTDirectoryPackage package("H:\\ISOs\\NSMBWii\\Extracted\\Layout\\continue\\continue\\arc");
	LYTLayout layout(package, "continue_05.brlyt");

    LSMainWindow w;
    w.show();

    return a.exec();
}
