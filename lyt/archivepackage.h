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
	Q_OBJECT
public:
	LYTArchivePackage(QObject *parent = 0);
	LYTArchivePackage(QString filename, QObject *parent = 0);

	~LYTArchivePackage();

	QStringList list(ItemType type) const;
	QByteArray get(ItemType type, const QString &name) const;
	bool write(ItemType type, const QString &name, const QByteArray &data);
	bool rename(ItemType type, const QString &from, const QString &to);
	bool remove(ItemType type, const QString &name);

	bool needsExplicitSave() const { return true; }
	bool savePackage();
	QString description() const;

	WiiArchiveU8 *archive() const;
	QString filename() const;
    void setFilename(QString path);


protected:
	WiiArchiveU8 *m_archive;
	QString m_filename;
};

#endif // LYTARCHIVEPACKAGE_H
