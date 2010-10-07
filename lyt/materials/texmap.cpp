#include "texmap.h"
#include "../layout.h"

LYTTexMap::LYTTexMap() {
}

void LYTTexMap::dumpToDebug() {
	qDebug() << "LYTTexMap @" << (void*)this << ":" << textureName;
	qDebug() << "- wrap_s:" << wrap_s << "- wrap_t:" << wrap_t;
	qDebug() << "- mag_filter:" << mag_filter << "- min_filter:" << min_filter;
}


void LYTTexMap::writeToDataStream(QDataStream &out, LYTLayout &layout) {
	quint16 texNum = layout.m_textureRefs.indexOf(textureName);
	out << (quint16)texNum;

	quint8 var1, var2;
	var1 = wrap_s | (((min_filter + 7) & 7) << 2);
	var2 = wrap_t | (((mag_filter + 1) & 1) << 2);
	out << (quint8)var1;
	out << (quint8)var2;
}


void LYTTexMap::readFromDataStream(QDataStream &in, LYTLayout &layout) {
	quint16 texNum;
	in >> (quint16&)texNum;

	textureName = layout.m_textureRefs[texNum];

	quint8 var1, var2;
	in >> (quint8&)var1;
	in >> (quint8&)var2;

	wrap_s = BitExtract(var1, 2, 30);
	wrap_t = BitExtract(var2, 2, 30);

	min_filter = (BitExtract(var1, 3, 27) + 1) & 7;
	mag_filter = (BitExtract(var2, 1, 29) + 1) & 1;
}
