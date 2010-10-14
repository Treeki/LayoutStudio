/*******************************************************************************
  This file is part of LayoutStudio (http://github.com/Treeki/LayoutStudio)
  Copyright (c) 2010 Treeki (treeki@gmail.com)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, version 2.0.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License 2.0 for more details.

  You should have received a copy of the GNU General Public License 2.0
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/

#include <QtGui/QApplication>
#include "lsmainwindow.h"
#include "lsglobals.h"

#include "lyt/directorypackage.h"
#include "lyt/archivepackage.h"
#include "lyt/layout.h"

#include <QtCore/QFile>

#include "wii/archiveu8.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

	LSGlobals::setup();

	/*QFile file("H:\\ISOs\\NSMBWii\\Extracted\\Layout\\continue\\continue.arc");
	file.open(QFile::ReadOnly);
	QByteArray arc = file.readAll();
	file.close();*/

	LYTArchivePackage package("H:\\ISOs\\NSMBWii\\Extracted\\Layout\\continue\\continue.arc");
	LYTLayout layout(package, "continue_05.brlyt");
	package.savePackage();


	//LYTDirectoryPackage package("H:\\ISOs\\NSMBWii\\Extracted\\Layout\\continue\\continue\\arc");
	//LYTLayout layout(package, "continue_05.brlyt");
	//LYTDirectoryPackage package("H:\\ISOs\\TP\\banner\\arc_extr");
	//LYTLayout layout(package, "banner.brlyt");

    LSMainWindow w;
    w.show();

    return a.exec();
}
