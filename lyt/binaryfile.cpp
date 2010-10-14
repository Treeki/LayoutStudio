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

#include "binaryfile.h"
#include "binaryfilesection.h"

#include <QtCore/QDataStream>

LYTBinaryFile::LYTBinaryFile(Magic m, Version v) : magic(m), version(v) {
}


LYTBinaryFile::LYTBinaryFile(QByteArray data) : magic(0), version(0) {
	QDataStream reader(data);
	InitDataStream(reader);

	quint16 endian, firstSectionOffset, sectionCount;
	quint32 fileSize;

	reader >> this->magic.value;
	reader >> endian;
	reader >> this->version.value;
	reader >> fileSize;
	reader >> firstSectionOffset;
	reader >> sectionCount;

	LYTBinaryFileSection section;

	for (int i = 0; i < sectionCount; i++) {
		section.readFromDataStream(reader);
		this->sections.append(section);
	}
}


QByteArray LYTBinaryFile::pack() const {
	// first off, calculate filesize for the header
	quint32 fileSize = 16;

	foreach (LYTBinaryFileSection section, this->sections) {
		fileSize += section.writtenSize();
	}

	// set up other fields
	quint16 endian, firstSectionOffset;
	endian = 0xFEFF;
	firstSectionOffset = 16;


	// write it
	QByteArray output;
	QDataStream writer(&output, QIODevice::WriteOnly);
	InitDataStream(writer);

	writer << this->magic.value;
	writer << endian;
	writer << this->version.value;
	writer << fileSize;
	writer << firstSectionOffset;
	writer << (quint16)this->sections.count();

	foreach (LYTBinaryFileSection section, this->sections) {
		section.writeToDataStream(writer);
	}

	return output;
}
