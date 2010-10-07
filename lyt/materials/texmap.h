#ifndef LYTTEXMAP_H
#define LYTTEXMAP_H

#include "../common.h"
#include <QtCore/QDataStream>

class LYTLayout; // forward declaration

class LYTTexMap {
public:
	LYTTexMap();

	void writeToDataStream(QDataStream &out, LYTLayout &layout);
	void readFromDataStream(QDataStream &in, LYTLayout &layout);

	void dumpToDebug();

	QString textureName;
	int wrap_s;
	int wrap_t;
	int mag_filter;
	int min_filter;
};

#endif // LYTTEXMAP_H
