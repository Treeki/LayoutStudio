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
