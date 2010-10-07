#ifndef LYTWINDOW_H
#define LYTWINDOW_H

#include <QtGui/QColor>
#include <QtCore/QVector>

#include "common.h"
#include "pane.h"


class LYTWindow; // forward declaration

class LYTWindowFrame {
public:
	LYTWindowFrame(LYTWindow &window);


	void writeToDataStream(QDataStream &out);
	void readFromDataStream(QDataStream &in);

	void dumpToDebug();

	quint8 type; // 0-5; controls texture flipping; must investigate this more
	QString materialName;

protected:
	LYTWindow &m_window;
};


class LYTWindow : public LYTPane {
public:
	LYTWindow(LYTLayout &layout);
	~LYTWindow();


	void writeToDataStream(QDataStream &out);
	void readFromDataStream(QDataStream &in);

	void dumpToDebug(bool showHeading=true);


	float contentOverflowLeft;
	float contentOverflowRight;
	float contentOverflowTop;
	float contentOverflowBottom;

	QColor contentVtxColours[4];
	QString contentMaterialName;
	QVector<LYTTexCoords> contentTexCoords;

	QList<LYTWindowFrame *> frames;


protected:
	void writeContentInfo(QDataStream &out);
	void readContentInfo(QDataStream &in);

	friend class LYTWindowFrame;
};


#endif // LYTWINDOW_H
