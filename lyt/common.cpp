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

QByteArray PadByteArray(QByteArray original, int newLength, char padWith) {
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
		in.skipRawData(4); // unused?

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

QString ReadFixedLengthASCII(QDataStream &in, int length) {
	QByteArray readStr(length, '\0');
	in.readRawData(readStr.data(), readStr.length());

	QString str = QString::fromAscii(readStr.data(), readStr.length());
	if (str.contains(QChar('\0')))
		str.truncate(str.indexOf(QChar('\0')));

	return str;
}

void WriteFixedLengthASCII(QDataStream &out, QString str, int length) {
	QByteArray paddedStr = PadByteArray(str.toAscii(), length);
	out.writeRawData(paddedStr.constData(), paddedStr.length());
}

