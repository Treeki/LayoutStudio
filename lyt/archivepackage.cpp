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

#include "archivepackage.h"

#include <QtCore/QFile>

LYTArchivePackage::LYTArchivePackage(QObject *parent) : LYTPackageBase(parent) {
	m_archive = new WiiArchiveU8;
}

LYTArchivePackage::LYTArchivePackage(QString filename, QObject *parent) : LYTPackageBase(parent) {
	m_filename = filename;

	QFile file(filename);
	file.open(QFile::ReadOnly);
	QByteArray data = file.readAll();

	QDataStream stream(data);
	m_archive = new WiiArchiveU8(stream);
}

LYTArchivePackage::~LYTArchivePackage() {
	delete m_archive;
}



WiiArchiveU8 *LYTArchivePackage::archive() const {
	return m_archive;
}

QString LYTArchivePackage::filename() const {
	return m_filename;
}
void LYTArchivePackage::setFilename(QString path) {
    m_filename = path;
}



QStringList LYTArchivePackage::list(ItemType type) const {
	WiiFSObject *obj = this->m_archive->root.resolvePath(defaultPathForItemType(type, true));

	if (obj && obj->isDirectory()) {
		QStringList output;

		foreach (WiiFSObject *ptr, ((WiiDirectory*)obj)->children) {
			output.append(ptr->name);
		}

		return output;
	}

	return QStringList();
}

QByteArray LYTArchivePackage::get(ItemType type, const QString &name) const {
	QString dirName = defaultPathForItemType(type, true);
	WiiFSObject *obj = this->m_archive->root.resolvePath(QString("%1/%2").arg(dirName, name));

	if (obj && obj->isFile()) {
		return ((WiiFile*)obj)->data;
	}

	return QByteArray();
}

bool LYTArchivePackage::write(ItemType type, const QString &name, const QByteArray &data) {
	if (name.isEmpty())
		return false;

    WiiFSObject *rootDir = this->m_archive->root.findByName("arc", false);
    if (!rootDir) {
        rootDir = new WiiDirectory;
        rootDir->name = "arc";
        m_archive->root.addChild(rootDir);
    }
    if (!rootDir->isDirectory())
        return false;

    QString dirName = defaultPathForItemType(type, false);
    WiiFSObject *dir = ((WiiDirectory*)rootDir)->findByName(dirName, false);

    if (!dir) {
        dir = new WiiDirectory;
        dir->name = dirName;
        ((WiiDirectory*)rootDir)->addChild(dir);
    }
    if (!dir->isDirectory())
        return false;

    WiiFSObject *obj = ((WiiDirectory*)dir)->findByName(name, false);

    if (obj && obj->isFile()) {
		emit aboutToModifyFile(type, name);

        ((WiiFile*)obj)->data = data;

		emit fileWasModified(type, name);

        return true;

    } else if (!obj) {
		emit aboutToAddFile(type, name);

        WiiFile *newFile = new WiiFile;
        newFile->name = name;
        newFile->data = data;
        ((WiiDirectory*)dir)->addChild(newFile);

		emit fileWasAdded(type, name);

        return true;
    }

    return false;
}

bool LYTArchivePackage::remove(ItemType type, const QString &name) {
	WiiFSObject *obj = this->m_archive->root.resolvePath(defaultPathForItemType(type, true));

	if (obj && obj->isDirectory()) {
		WiiDirectory *dir = (WiiDirectory*)obj;

		WiiFSObject *what = dir->findByName(name, false);
		if (what && what->isFile()) {
			emit aboutToRemoveFile(type, name);
			dir->removeChild(what);
			emit fileWasRemoved(type, name);
			return true;
		}
	}

	return false;
}

bool LYTArchivePackage::rename(ItemType type, const QString &from, const QString &to) {
	if (to.isEmpty())
		return false;

	WiiFSObject *obj = this->m_archive->root.resolvePath(defaultPathForItemType(type, true));

	if (obj && obj->isDirectory()) {
		WiiDirectory *dir = (WiiDirectory*)obj;

		WiiFSObject *what = dir->findByName(from, false);
		if (what && what->isFile()) {
			WiiFSObject *conflict = dir->findByName(to, false);
			if (!conflict) {
				emit aboutToRenameFile(type, from, to);
				what->name = to;
				emit fileWasRenamed(type, from, to);
				return true;
			}
		}
	}

	return false;
}



bool LYTArchivePackage::savePackage() {
	QFile file(m_filename);

	if (file.open(QFile::WriteOnly)) {
		QByteArray data;
		QDataStream stream(&data, QIODevice::ReadWrite);

		m_archive->writeToDataStream(stream);
		file.write(data);

		return true;
	}

	return false;
}


QString LYTArchivePackage::description() const {
	return m_filename;
}
