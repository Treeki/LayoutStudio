#ifndef LYTBINARYFILESECTION_H
#define LYTBINARYFILESECTION_H

#include <QtCore/QDataStream>

#include "common.h"

class LYTBinaryFileSection {
public:
    LYTBinaryFileSection();

	Magic magic;
	QByteArray data;

	void writeToDataStream(QDataStream &out);
	void readFromDataStream(QDataStream &in);

	int writtenSize();
};



#endif // LYTBINARYFILESECTION_H
