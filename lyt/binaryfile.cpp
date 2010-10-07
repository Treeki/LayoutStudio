#include "binaryfile.h"
#include "binaryfilesection.h"

#include <QtCore/QDataStream>

LYTBinaryFile::LYTBinaryFile(Magic magic, Version version) {
	this->magic.value = magic.value;
	this->version.value = version.value;
}


LYTBinaryFile::LYTBinaryFile(QByteArray data) {
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


QByteArray LYTBinaryFile::pack() {
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
