#include "texsrt.h"
#include "../layout.h"

LYTTexSRT::LYTTexSRT() {
}

void LYTTexSRT::dumpToDebug() {
	qDebug() << "LYTTexSRT @" << (void*)this;
	qDebug() << "Scale:" << xScale << "," << yScale;
	qDebug() << "Rotation:" << rotate;
	qDebug() << "Translation:" << xTrans << "," << yTrans;
}


void LYTTexSRT::writeToDataStream(QDataStream &out) {
	out << (float)xTrans;
	out << (float)yTrans;
	out << (float)rotate;
	out << (float)xScale;
	out << (float)yScale;
}


void LYTTexSRT::readFromDataStream(QDataStream &in) {
	in >> (float&)xTrans;
	in >> (float&)yTrans;
	in >> (float&)rotate;
	in >> (float&)xScale;
	in >> (float&)yScale;
}

