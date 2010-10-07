#include "texcoordgen.h"
#include "../layout.h"

LYTTexCoordGen::LYTTexCoordGen() {
}

void LYTTexCoordGen::dumpToDebug() {
	qDebug() << "LYTTexCoordGen @" << (void*)this;
	qDebug() << "GenType:" << genType << "- Mtx:" << mtx << "- Src:" << src;
}


void LYTTexCoordGen::writeToDataStream(QDataStream &out) {
	out << (quint8)genType;
	out << (quint8)src;
	out << (quint8)mtx;
	out.skipRawData(1); // padding
}


void LYTTexCoordGen::readFromDataStream(QDataStream &in) {
	in >> (quint8&)genType;
	in >> (quint8&)src;
	in >> (quint8&)mtx;
	in.skipRawData(1); // padding
}

