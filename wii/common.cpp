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

#include "common.h"

QByteArray PadByteArray(const QByteArray original, int newLength, char padWith) {
	QByteArray newArray = original;

	if (original.length() > newLength) {
		// the original array is longer than the length desired, so truncate it
		newArray.truncate(newLength);

	} else if (original.length() < newLength) {
		// the original array is shorter, so pad it
		int oldLength = original.length();
		newArray.resize(newLength);

		for (int i = oldLength; i < newLength; i++) {
			newArray[i] = '\0';
		}
	}

	return newArray;
}

QStringList ReadStringList(QDataStream &in) {
	QStringList output;

	quint16 count;
	in >> (quint16&)count;
	in.skipRawData(2); // padding

	QVector<quint32> stringOffsets(count);

	// save the initial offset so we can get the strings later
	// string offsets are based on the first offset entry (after the count)
	// NOT on the section offset
	qint64 savedPos = in.device()->pos();

	for (int i = 0; i < count; i++) {
		quint32 offset;
		in >> (quint32&)offset;

		stringOffsets[i] = offset;
	}

	// ok, now we can get the strings
	for (int i = 0; i < count; i++) {
		in.device()->seek(savedPos + stringOffsets[i]);

		// how fun: no length is stored for each string, they're just zero
		// terminated. so let's try to figure it out!
		int stringLength = 0;
		char check;

		in >> (quint8&)check;
		while (check != 0) {
			stringLength += 1;
			in >> (quint8&)check;
		}

		// now read the string
		char *buffer = new char[stringLength];

		in.device()->seek(savedPos + stringOffsets[i]);
		in.readRawData(buffer, stringLength);

		output.append(QString::fromAscii(buffer, stringLength));

		delete[] buffer;


		qDebug() << "Read string:" << output.last();
	}

	return output;
}

void WriteStringList(QDataStream &out, const QStringList list) {
	out << (quint16)list.count();
	WritePadding(2, out);

	// calculate offsets for every string, and write them
	// offset 0 points to the first entry in the offset list, etc, so
	// take that into account for the string offset calculation
	quint32 currentOffset = list.count() * 8;

	foreach (QString str, list) {
		out << (quint32)currentOffset;
		WritePadding(4, out); // unused?

		currentOffset += str.length() + 1;
	}

	// now write the strings
	foreach (QString str, list) {
		QByteArray rawStr = str.toAscii();
		rawStr.append('\0');
		out.writeRawData(rawStr.constData(), rawStr.length());
	}
}

QString ReadFixedLengthASCII(QDataStream &in, int length) {
	QByteArray readStr(length, '\0');
	in.readRawData(readStr.data(), readStr.length());

	QString str = QString::fromAscii(readStr.data(), readStr.length());
	if (str.contains(QChar('\0')))
		str.truncate(str.indexOf(QChar('\0')));

	return str;
}

void WriteFixedLengthASCII(QDataStream &out, const QString str, int length) {
	QByteArray paddedStr = PadByteArray(str.toAscii(), length);
	out.writeRawData(paddedStr.constData(), paddedStr.length());
}


