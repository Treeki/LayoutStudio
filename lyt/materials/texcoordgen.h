#ifndef LYTTEXCOORDGEN_H
#define LYTTEXCOORDGEN_H

#include "../common.h"
#include <QtCore/QDataStream>

class LYTTexCoordGen {
public:
	LYTTexCoordGen();

	void writeToDataStream(QDataStream &out);
	void readFromDataStream(QDataStream &in);

	void dumpToDebug();

	quint8 genType;
	quint8 src;
	quint8 mtx;
};

#endif // LYTTEXCOORDGEN_H
