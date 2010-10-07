#include "group.h"

LYTGroup::LYTGroup() {
}




void LYTGroup::writeToDataStream(QDataStream &out) {
	WriteFixedLengthASCII(out, name, 0x10);

	// write the contents
	out << (quint16)panes.count();
	out.skipRawData(2); // padding

	foreach (LYTPane *pane, panes) {
		WriteFixedLengthASCII(out, pane->name, 0x10);
	}
}


void LYTGroup::readFromDataStream(QDataStream &in, LYTPane &linkedPane) {
	name = ReadFixedLengthASCII(in, 0x10);
	qDebug() << "reading group" << name;

	// read the contents
	quint16 paneCount;
	in >> (quint16&)paneCount;
	in.skipRawData(2); // padding

	for (int i = 0; i < paneCount; i++) {
		QString paneName = ReadFixedLengthASCII(in, 0x10);

		qDebug() << "found" << paneName << "in group" << this->name;

		this->panes.append(linkedPane.findPaneByName(paneName, true));
	}
}
