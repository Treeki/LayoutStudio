#ifndef LYTTEXTBOX_H
#define LYTTEXTBOX_H

#include <QtGui/QColor>

#include "pane.h"

class LYTTextBox : public LYTPane {
public:
	LYTTextBox(LYTLayout &layout);


	void writeToDataStream(QDataStream &out);
	void readFromDataStream(QDataStream &in);

	void dumpToDebug(bool showHeading=true);

	quint16 bufferLength;
	QString text;

	QString materialName;
	QString fontName;

	quint8 alignment;
	quint8 alignmentOverride;

	QColor colour1;
	QColor colour2;

	float fontSizeX;
	float fontSizeY;
	float charSpace;
	float lineSpace;
};


#endif // LYTTEXTBOX_H
