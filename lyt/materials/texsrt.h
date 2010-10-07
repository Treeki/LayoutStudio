#ifndef LYTTEXSRT_H
#define LYTTEXSRT_H

#include "../common.h"
#include <QtCore/QDataStream>

class LYTTexSRT {
public:
	LYTTexSRT();

	void writeToDataStream(QDataStream &out);
	void readFromDataStream(QDataStream &in);

	void dumpToDebug();

	float xTrans;
	float yTrans;
	float rotate;
	float xScale;
	float yScale;
};

#endif // LYTTEXSRT_H
