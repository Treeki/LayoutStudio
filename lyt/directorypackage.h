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

#ifndef LYTDIRECTORYPACKAGE_H
#define LYTDIRECTORYPACKAGE_H

#include "packagebase.h"

class LYTDirectoryPackage : public LYTPackageBase {
public:
	LYTDirectoryPackage(QString path);

	QStringList listAnims();
	QStringList listLayouts();
	QStringList listTextures();
	QStringList listFonts();

	QByteArray getAnim(QString name);
	QByteArray getLayout(QString name);
	QByteArray getTexture(QString name);
	QByteArray getFont(QString name);

	bool writeAnim(QString name, QByteArray data);
	bool writeLayout(QString name, QByteArray data);
	bool writeTexture(QString name, QByteArray data);
	bool writeFont(QString name, QByteArray data);

	bool savePackage();
	QString description();
	QString path();

protected:
	QStringList listSubDirIfExists(QString dirName);
	QByteArray getFileFromSubDirIfExists(QString dirName, QString fileName);
	bool writeFileToSubDir(QString dirName, QString fileName, QByteArray data);


	QString m_path;
};

#endif // LYTDIRECTORYPACKAGE_H
