/*******************************************************************************
  This file is part of LayoutStudio (http://github.com/Treeki/LayoutStudio)
  Copyright (c) 2010 Treeki (treeki@gmail.com)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, version 2.0.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License 2.0 for more details.

  You should have received a copy of the GNU General Public License 2.0
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/

#include "binaryfilesection.h"

LYTBinaryFileSection::LYTBinaryFileSection() : magic(0) {
}

LYTBinaryFileSection::LYTBinaryFileSection(Magic m) : magic(m) {
}

LYTBinaryFileSection::LYTBinaryFileSection(Magic m, QByteArray d) : magic(m), data(d) {
}


void LYTBinaryFileSection::writeToDataStream(QDataStream &out) const {
	out << (quint32)magic.value;
	out << (quint32)AlignUp(data.length() + 8, 4);
	out.writeRawData(data.constData(), data.length());

	int align = AlignUp(data.length(), 4) - data.length();
	WritePadding(align, out);
}

void LYTBinaryFileSection::readFromDataStream(QDataStream &in) {
	quint32 length;

	char a[4];
	in.readRawData(a, 4);
	magic.str[0] = a[3];
	magic.str[1] = a[2];
	magic.str[2] = a[1];
	magic.str[3] = a[0];
	in >> (quint32&)length;

	char *raw = new char[length - 8];
	in.readRawData(raw, length - 8);

	data = QByteArray(raw, length - 8);
}


int LYTBinaryFileSection::writtenSize() const {
	return 8 + AlignUp(this->data.length(), 4);
}


QDataStream *LYTBinaryFileSection::createReadStream() const {
	QDataStream *stream = new QDataStream(this->data);
	InitDataStream(*stream);
	return stream;
}


QDataStream *LYTBinaryFileSection::createWriteStream() {
	QDataStream *stream = new QDataStream(&this->data, QIODevice::ReadWrite);
	InitDataStream(*stream);
	return stream;
}
