#ifndef LYTPICTURE_H
#define LYTPICTURE_H

#include <QtGui/QColor>
#include <QtCore/QVector>

#include "common.h"
#include "pane.h"

class LYTPicture : public LYTPane {
public:
	LYTPicture(LYTLayout &layout);


	void writeToDataStream(QDataStream &out);
	void readFromDataStream(QDataStream &in);

	void dumpToDebug(bool showHeading=true);

	QColor vtxColours[4];
	QString materialName;

	QVector<LYTTexCoords> texCoords;
};


#endif // LYTPICTURE_H
