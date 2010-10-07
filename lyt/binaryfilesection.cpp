#include "binaryfilesection.h"

LYTBinaryFileSection::LYTBinaryFileSection() {
}


void LYTBinaryFileSection::writeToDataStream(QDataStream &out) {
	out << (quint32)magic.value;
	out << (quint32)data.length() + 8;
	out.writeRawData(data.constData(), data.length());
}

void LYTBinaryFileSection::readFromDataStream(QDataStream &in) {
	quint32 length;

	in >> (quint32&)magic.value;
	in >> (quint32&)length;

	char *raw = new char[length - 8];
	in.readRawData(raw, length - 8);

	data = QByteArray(raw, length - 8);
}


int LYTBinaryFileSection::writtenSize() {
	return 8 + this->data.length();
}
