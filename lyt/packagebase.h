#ifndef LYTPACKAGEBASE_H
#define LYTPACKAGEBASE_H

#include <QStringList>
#include <QByteArray>


class LYTPackageBase {
public:
	LYTPackageBase();

	virtual QStringList listAnims() = 0;
	virtual QStringList listLayouts() = 0;
	virtual QStringList listTextures() = 0;
	virtual QStringList listFonts() = 0;

	virtual QByteArray getAnim(QString name) = 0;
	virtual QByteArray getLayout(QString name) = 0;
	virtual QByteArray getTexture(QString name) = 0;
	virtual QByteArray getFont(QString name) = 0;

	virtual bool writeAnim(QString name, QByteArray data) = 0;
	virtual bool writeLayout(QString name, QByteArray data) = 0;
	virtual bool writeTexture(QString name, QByteArray data) = 0;
	virtual bool writeFont(QString name, QByteArray data) = 0;

	virtual bool savePackage() = 0;
	virtual QString description() = 0;
};

#endif // LYTPACKAGEBASE_H
