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

#include "layoutgl/widget.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    LSMainWindow w;
    w.show();

    return a.exec();

	/*QFile file("H:\\ISOs\\NSMBWii\\Extracted\\Layout\\continue\\continue.arc");
	file.open(QFile::ReadOnly);
	QByteArray arc = file.readAll();
	file.close();*/

	//LYTArchivePackage package("H:\\ISOs\\NSMBWii\\Extracted\\Layout\\continue\\continue.arc");
	QString blah1 = "preGame";
	//QString blah = "preGame/preGame.arc";
	QString blah = QString("%1/%1.arc").arg(blah1);
	QString cpath;
	if (QFile::exists("/home/me/Games/Newer/ISO/files/Layout/" + blah)) {
		cpath = "/home/me/Games/Newer/ISO/files/Layout/" + blah;
	} else {
		cpath = "Z:\\stuff\\Games\\Newer\\ISO\\files\\Layout\\" + blah;
	}
	LYTArchivePackage package(cpath);
	LYTLayout layout(package, package.listLayouts().first());
	//LYTLayout layout(package, "continue_05.brlyt");
	//QByteArray brlyt = layout.pack();
	//QFile file("H:\\ISOs\\NSMBWii\\Extracted\\Layout\\continue\\continue\\arc\\blyt\\continue_05_repack.brlyt");
	//file.open(QFile::WriteOnly);
	//file.write(brlyt);
	//file.close();
	//package.writeLayout("continue_05.brlyt", brlyt);
	//package.savePackage();


	//LYTDirectoryPackage package("H:\\ISOs\\NSMBWii\\Extracted\\Layout\\continue\\continue\\arc");
	//LYTLayout layout(package, "continue_05.brlyt");
	//LYTDirectoryPackage package("H:\\ISOs\\TP\\banner\\arc_extr");
	//LYTDirectoryPackage package("/mnt/h/ISOs/TP/banner/arc_extr");
	//LYTDirectoryPackage package("/mnt/h/ISOs/hbm/InetChannelNew/0001000148414450/00000000_app_OUT/meta/banner_bin_OUT/arc");
	//LYTDirectoryPackage package("/mnt/h/ISOs/CSWii/BannerTools/0001000157435645/00000000_app_OUT/meta/banner_bin_OUT/arc");
	//LYTLayout layout(package, "banner.brlyt");

    //LSMainWindow w;
    w.show();

	LGLWidget w2;
	w2.setLayout(&layout);
	w2.show();

    return a.exec();
}
