#ifndef LYTPANE_H
#define LYTPANE_H

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QDataStream>
#include <QtCore/QDebug>

#include "common.h"

class LYTLayout;

class LYTPane {
public:
	LYTPane(LYTLayout &layout);
	virtual ~LYTPane();


	virtual void writeToDataStream(QDataStream &out);
	virtual void readFromDataStream(QDataStream &in);

	virtual void dumpToDebug(bool showHeading=true);

	LYTPane *findPaneByName(QString name, bool recursive);

	LYTLayout &layout() const;

	LYTPane *parent;
	QList<LYTPane *> children;

	quint8 flags;
	quint8 origin;
	quint8 alpha;

	QString name;
	QString userdata;

	float xTrans;
	float yTrans;
	float zTrans;

	float xRot;
	float yRot;
	float zRot;

	float xScale;
	float yScale;

	float width;
	float height;

protected:
	LYTLayout &m_layout;
};



#endif // LYTPANE_H
