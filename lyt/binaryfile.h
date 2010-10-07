#ifndef LYTBINARYFILE_H
#define LYTBINARYFILE_H

#include <QtCore/QList>

#include "common.h"
#include "binaryfilesection.h"

class LYTBinaryFile {
public:
	LYTBinaryFile(Magic magic, Version version);
	LYTBinaryFile(QByteArray data);

	QByteArray pack();

	Magic magic;
	Version version;

	QList<LYTBinaryFileSection> sections;
};

#endif // LYTBINARYFILE_H
