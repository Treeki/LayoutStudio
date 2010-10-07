#ifndef LYTGROUP_H
#define LYTGROUP_H

#include "common.h"
#include "pane.h"

#include <QtCore/QList>

class LYTGroup {
public:
    LYTGroup();


	void writeToDataStream(QDataStream &out);
	void readFromDataStream(QDataStream &in, LYTPane &linkedPane);

	QString name;

	QList<LYTPane *> panes;
};

#endif // LYTGROUP_H
