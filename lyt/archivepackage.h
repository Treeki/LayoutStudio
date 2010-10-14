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

#ifndef LYTARCHIVEPACKAGE_H
#define LYTARCHIVEPACKAGE_H

#include "packagebase.h"
#include "wii/archiveu8.h"

class LYTArchivePackage : public LYTPackageBase {
public:
	LYTArchivePackage();
	LYTArchivePackage(QString filename);

	~LYTArchivePackage();

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

	WiiArchiveU8 *archive();
	QString filename();


protected:
	QStringList listSubDirIfExists(QString dirName);
	QByteArray getFileFromSubDirIfExists(QString dirName, QString fileName);
	bool writeFileToSubDir(QString dirName, QString fileName, QByteArray data);

	WiiArchiveU8 *m_archive;
	QString m_filename;
};

#endif // LYTARCHIVEPACKAGE_H
