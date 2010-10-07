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
