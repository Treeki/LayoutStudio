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

#include "textbox.h"
#include "layout.h"
#include "common.h"


LYTTextBox::LYTTextBox(LYTLayout &layout) : LYTPane(layout) {
}


Magic LYTTextBox::magic() const {
	return Magic('txt1');
}


void LYTTextBox::dumpToDebug(bool showHeading) const {
	if (showHeading)
		qDebug() << "LYTTextBox" << name << "@" << (void*)this;

	LYTPane::dumpToDebug(false);

	qDebug() << "- Text:" << text;
	qDebug() << "- Buffer Length:" << bufferLength;
	qDebug() << "- Material:" << materialName << "- Font:" << fontName;
	qDebug() << "- Alignment:" << alignment << "- Alignment Override:" << alignmentOverride;
	qDebug() << "- Colours:" << colour1 << "--" << colour2;
	qDebug() << "- Font Size:" << fontSizeX << "x" << fontSizeY;
	qDebug() << "- Char Space:" << charSpace << "- Line Space:" << lineSpace;
}



void LYTTextBox::writeToDataStream(QDataStream &out) const {
	LYTPane::writeToDataStream(out);

	// lengths are stored in bytes (including zero terminator) not characters
	out << (quint16)((bufferLength + 1) * 2);
	out << (quint16)((text.length() + 1) * 2);

	// calculate the material and font numbers
	int materialNum = m_layout.materials.getIndexOfName(materialName);
	int fontNum = m_layout.m_fontRefs.indexOf(fontName);

	out << (quint16)materialNum;
	out << (quint16)fontNum;

	out << (quint8)alignment;
	out << (quint8)alignmentOverride;

	WritePadding(2, out);

	out << (quint32)0x74; // fixed offset to textbox contents

	WriteRGBA8Color(colour1, out);
	WriteRGBA8Color(colour2, out);

	out << (float)fontSizeX;
	out << (float)fontSizeY;
	out << (float)charSpace;
	out << (float)lineSpace;

	// write the textbox contents
	const ushort *convertedText = text.utf16();
	for (int i = 0; i < text.length(); i++)
		out << (quint16)convertedText[i];

	out << (quint16)0; // zeroterm
}


void LYTTextBox::readFromDataStream(QDataStream &in) {
	qint64 saveStartPos = in.device()->pos();

	LYTPane::readFromDataStream(in);

	// the lengths are stored in bytes (not characters) and count the
	// zero terminator, and strings are UTF-16 (I think) so we need
	// to take it off here
	in >> (quint16&)bufferLength;
	bufferLength >>= 1;
	bufferLength--;

	quint16 stringLength;
	in >> (quint16&)stringLength;
	stringLength >>= 1;
	stringLength--;

	// read the material and font names
	quint16 materialNum, fontNum;
	in >> (quint16&)materialNum;
	in >> (quint16&)fontNum;

	materialName = m_layout.materials.getNameOfIndex(materialNum);
	fontName = m_layout.m_fontRefs.at(fontNum);

	in >> (quint8&)alignment;
	in >> (quint8&)alignmentOverride;

	in.skipRawData(2); // padding

	quint32 stringOffset;
	in >> (quint32&)stringOffset;

	ReadRGBA8Color(colour1, in);
	ReadRGBA8Color(colour2, in);

	in >> (float&)fontSizeX;
	in >> (float&)fontSizeY;
	in >> (float&)charSpace;
	in >> (float&)lineSpace;

	// read the textbox contents
	// subtract 8 to account for BinaryBlockHeader or whatever it's called
	in.device()->seek(saveStartPos + stringOffset - 8);

	ushort *rawText = new ushort[stringLength];

	for (int i = 0; i < stringLength; i++)
		in >> (quint16&)rawText[i];

	text.setUtf16(rawText, stringLength);
	delete[] rawText;
}


void LYTTextBox::addFontRefsToList(QStringList &list) const {
	LYTPane::addFontRefsToList(list);

	if (!list.contains(this->fontName)) {
		list.append(this->fontName);
	}
}
