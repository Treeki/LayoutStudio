#include "directorypackage.h"

#include <QDir>

LYTDirectoryPackage::LYTDirectoryPackage(QString path) : LYTPackageBase() {
	QDir fix_path(path);
	this->m_path = fix_path.absolutePath();
}



QStringList LYTDirectoryPackage::listSubDirIfExists(QString dirName) {
	QDir search(m_path);

	if (search.cd(dirName)) {
		return search.entryList();
	}

	return QStringList();
}


QByteArray LYTDirectoryPackage::getFileFromSubDirIfExists(QString dirName, QString fileName) {
	QDir search(m_path);

	if (search.cd(dirName)) {
		QFile file(search.absoluteFilePath(fileName));

		if (file.open(QFile::ReadOnly)) {
			return file.readAll();
		}
	}

	return QByteArray();
}


bool LYTDirectoryPackage::writeFileToSubDir(QString dirName, QString fileName, QByteArray data) {
	QDir search(m_path);

	if (search.cd(dirName)) {
		QFile file(search.absoluteFilePath(fileName));

		if (file.open(QFile::WriteOnly)) {
			if (file.write(data) != -1) {
				return true;
			}
		}
	}

	return false;
}




QStringList LYTDirectoryPackage::listAnims() {
	return this->listSubDirIfExists("anim");
}

QStringList LYTDirectoryPackage::listLayouts() {
	return this->listSubDirIfExists("blyt");
}

QStringList LYTDirectoryPackage::listTextures() {
	return this->listSubDirIfExists("timg");
}

QStringList LYTDirectoryPackage::listFonts() {
	return this->listSubDirIfExists("font");
}



QByteArray LYTDirectoryPackage::getAnim(QString name) {
	return this->getFileFromSubDirIfExists("anim", name);
}

QByteArray LYTDirectoryPackage::getLayout(QString name) {
	return this->getFileFromSubDirIfExists("blyt", name);
}

QByteArray LYTDirectoryPackage::getTexture(QString name) {
	return this->getFileFromSubDirIfExists("timg", name);
}

QByteArray LYTDirectoryPackage::getFont(QString name) {
	return this->getFileFromSubDirIfExists("font", name);
}



bool LYTDirectoryPackage::writeAnim(QString name, QByteArray data) {
	return this->writeFileToSubDir("anim", name, data);
}

bool LYTDirectoryPackage::writeLayout(QString name, QByteArray data) {
	return this->writeFileToSubDir("blyt", name, data);
}

bool LYTDirectoryPackage::writeTexture(QString name, QByteArray data) {
	return this->writeFileToSubDir("timg", name, data);
}

bool LYTDirectoryPackage::writeFont(QString name, QByteArray data) {
	return this->writeFileToSubDir("font", name, data);
}



bool LYTDirectoryPackage::savePackage() {
	// No-op since this is a directory
	return true;
}


QString LYTDirectoryPackage::description() {
	return m_path;
}
