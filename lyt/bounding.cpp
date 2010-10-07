#include "bounding.h"
#include "layout.h"
#include "common.h"


LYTBounding::LYTBounding(LYTLayout &layout) : LYTPane(layout) {
}


void LYTBounding::dumpToDebug(bool showHeading) {
	if (showHeading)
		qDebug() << "LYTBounding" << name << "@" << (void*)this;

	LYTPane::dumpToDebug(false);
}



void LYTBounding::writeToDataStream(QDataStream &out) {
	LYTPane::writeToDataStream(out);
}


void LYTBounding::readFromDataStream(QDataStream &in) {
	LYTPane::readFromDataStream(in);
}
