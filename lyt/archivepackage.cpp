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

LYTArchivePackage::LYTArchivePackage() : LYTPackageBase() {
	m_archive = new WiiArchiveU8;
}

LYTArchivePackage::LYTArchivePackage(QString filename) : LYTPackageBase() {
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



WiiArchiveU8 *LYTArchivePackage::archive() {
	return m_archive;
}

QString LYTArchivePackage::filename() {
	return m_filename;
}



QStringList LYTArchivePackage::listSubDirIfExists(QString dirName) {
	WiiFSObject *obj = this->m_archive->root.resolvePath(dirName);

	if (obj && obj->isDirectory()) {
		QStringList output;

		foreach (WiiFSObject *ptr, ((WiiDirectory*)obj)->children) {
			output.append(ptr->name);
		}

		return output;
	}

	return QStringList();
}


QByteArray LYTArchivePackage::getFileFromSubDirIfExists(QString dirName, QString fileName) {
	WiiFSObject *obj = this->m_archive->root.resolvePath(QString("%1/%2").arg(dirName, fileName));

	if (obj && obj->isFile()) {
		return ((WiiFile*)obj)->data;
	}

	return QByteArray();
}


bool LYTArchivePackage::writeFileToSubDir(QString dirName, QString fileName, QByteArray data) {
	WiiFSObject *obj = this->m_archive->root.resolvePath(QString("%1/%2").arg(dirName, fileName));

	if (obj && obj->isFile()) {
		((WiiFile*)obj)->data = data;
		return true;
	}

	return false;
}




QStringList LYTArchivePackage::listAnims() {
	return this->listSubDirIfExists("arc/anim");
}

QStringList LYTArchivePackage::listLayouts() {
	return this->listSubDirIfExists("arc/blyt");
}

QStringList LYTArchivePackage::listTextures() {
	return this->listSubDirIfExists("arc/timg");
}

QStringList LYTArchivePackage::listFonts() {
	return this->listSubDirIfExists("arc/font");
}



QByteArray LYTArchivePackage::getAnim(QString name) {
	return this->getFileFromSubDirIfExists("arc/anim", name);
}

QByteArray LYTArchivePackage::getLayout(QString name) {
	return this->getFileFromSubDirIfExists("arc/blyt", name);
}

QByteArray LYTArchivePackage::getTexture(QString name) {
	return this->getFileFromSubDirIfExists("arc/timg", name);
}

QByteArray LYTArchivePackage::getFont(QString name) {
	return this->getFileFromSubDirIfExists("arc/font", name);
}



bool LYTArchivePackage::writeAnim(QString name, QByteArray data) {
	return this->writeFileToSubDir("arc/anim", name, data);
}

bool LYTArchivePackage::writeLayout(QString name, QByteArray data) {
	return this->writeFileToSubDir("arc/blyt", name, data);
}

bool LYTArchivePackage::writeTexture(QString name, QByteArray data) {
	return this->writeFileToSubDir("arc/timg", name, data);
}

bool LYTArchivePackage::writeFont(QString name, QByteArray data) {
	return this->writeFileToSubDir("arc/font", name, data);
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


QString LYTArchivePackage::description() {
	return m_filename;
}
