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

#include "directorypackage.h"

#include <QtCore/QDir>

LYTDirectoryPackage::LYTDirectoryPackage(QString path, QObject *parent) : LYTPackageBase(parent) {
	qWarning("LYTDirectoryPackage is currently unmaintained, you probably shouldn't use it");

	QDir fix_path(path);
	this->m_path = fix_path.absolutePath();
}



QStringList LYTDirectoryPackage::list(ItemType type) const {
	QDir search(m_path);

	if (search.cd(defaultPathForItemType(type))) {
		return search.entryList();
	}

	return QStringList();
}

QByteArray LYTDirectoryPackage::get(ItemType type, const QString &name) const {
	QDir search(m_path);

	if (search.cd(defaultPathForItemType(type))) {
		QFile file(search.absoluteFilePath(name));

		if (file.open(QFile::ReadOnly)) {
			return file.readAll();
		}
	}

	return QByteArray();
}

bool LYTDirectoryPackage::write(ItemType type, const QString &name, const QByteArray &data) {
	QDir search(m_path);
    QString dirName = defaultPathForItemType(type);

    if (!search.cd(dirName)) {
        if (!search.mkdir(dirName))
            return false;
        if (!search.cd(dirName))
            return false;
    }

    QFile file(search.absoluteFilePath(name));

    if (file.open(QFile::WriteOnly)) {
        if (file.write(data) != -1) {
            return true;
        }
    }

    return false;
}

bool LYTDirectoryPackage::rename(ItemType type, const QString &from, const QString &to) {
	// Not implemented
	return false;
}

bool LYTDirectoryPackage::remove(ItemType type, const QString &name) {
	QDir search(m_path);

	if (search.cd(defaultPathForItemType(type))) {
		QFile file(search.absoluteFilePath(name));

		if (file.open(QFile::WriteOnly)) {
			return file.remove();
		}
	}

	return false;
}



bool LYTDirectoryPackage::savePackage() {
	// No-op since this is a directory
	return true;
}


QString LYTDirectoryPackage::description() const {
	return m_path;
}
