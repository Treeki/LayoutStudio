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

#include "window.h"
#include "layout.h"
#include "common.h"


LYTWindowFrame::LYTWindowFrame(LYTWindow &window) : m_window(window) {
}

void LYTWindowFrame::writeToDataStream(QDataStream &out) {
	// calculate the material number
	int materialNum = m_window.m_layout.materials.keys().indexOf(materialName);
	out << (quint16)materialNum;

	out << (quint8)type;
	out.skipRawData(1); // padding
}

void LYTWindowFrame::readFromDataStream(QDataStream &in) {
	// read the material name
	quint16 materialNum;
	in >> (quint16&)materialNum;

	materialName = m_window.m_layout.materials.keys().at(materialNum);

	in >> (quint8&)type;
	in.skipRawData(1); // padding
}

void LYTWindowFrame::dumpToDebug() {
	qDebug() << "LYTWindowFrame @" << (void*)this << "- type:" << type << "- material:" << materialName;
}



LYTWindow::LYTWindow(LYTLayout &layout) : LYTPane(layout) {
}

LYTWindow::~LYTWindow() {
	foreach (LYTWindowFrame *frame, frames)
		delete frame;
}



// all of this needs to be done

void LYTWindow::dumpToDebug(bool showHeading) {
	if (showHeading)
		qDebug() << "LYTWindow" << name << "@" << (void*)this;

	LYTPane::dumpToDebug(false);

	qDebug() << "- Content VtxColours:" << contentVtxColours[0] << "-" << contentVtxColours[1];
	qDebug() << "                     " << contentVtxColours[2] << "-" << contentVtxColours[3];
	qDebug() << "- Content Material:" << contentMaterialName;
	qDebug() << "- Content Tex Coords:" << contentTexCoords.count();

	foreach (LYTTexCoords texCoord, contentTexCoords) {
		qDebug() << "----" << texCoord.coord[0] << "-" << texCoord.coord[1] << "-" << texCoord.coord[2] << "-" << texCoord.coord[3];
	}

	qDebug() << "- Content Overflow: Left" << contentOverflowLeft << "- Right" << contentOverflowRight;
	qDebug() << "                    Top" << contentOverflowTop << "- Bottom" << contentOverflowBottom;

	qDebug() << "- Frames:" << frames.count();

	foreach (LYTWindowFrame *frame, frames) {
		frame->dumpToDebug();
	}
}



void LYTWindow::writeToDataStream(QDataStream &out) {
	LYTPane::writeToDataStream(out);

	out << (float)contentOverflowLeft;
	out << (float)contentOverflowRight;
	out << (float)contentOverflowTop;
	out << (float)contentOverflowBottom;

	out << (quint8)frames.count();
	out.skipRawData(3); // padding

	out << (quint32)0x68; // offset to content struct
	out << (quint32)(0x7C + (contentTexCoords.count()*0x20)); // offset to frame offset list

	for (int i = 0; i < 4; i++)
		WriteRGBA8Color(contentVtxColours[i], out);

	// calculate the material number
	int materialNum = m_layout.materials.keys().indexOf(contentMaterialName);
	out << (quint16)materialNum;

	// write texcoords
	out << (quint8)contentTexCoords.count();
	out.skipRawData(1); // padding

	foreach (LYTTexCoords texCoord, contentTexCoords) {
		for (int i = 0; i < 4; i++)
			WritePointF(out, texCoord.coord[i]);
	}

	// write frame offsets
	quint32 frameOffset = 0x7C; // end of fixed-size part of content struct
	frameOffset += (contentTexCoords.count() * 0x20); // end of content struct
	frameOffset += (frames.count() * 4); // end of offset list

	for (int i = 0; i < frames.count(); i++) {
		out << (quint32)frameOffset;
		frameOffset += 4; // size of frame struct
	}

	// now write frames
	foreach (LYTWindowFrame *frame, frames) {
		frame->writeToDataStream(out);
	}
}


void LYTWindow::readFromDataStream(QDataStream &in) {
	qint64 startPos = in.device()->pos();

	LYTPane::readFromDataStream(in);

	in >> (float&)contentOverflowLeft;
	in >> (float&)contentOverflowRight;
	in >> (float&)contentOverflowTop;
	in >> (float&)contentOverflowBottom;

	quint8 frameCount;
	in >> (quint8&)frameCount;
	in.skipRawData(3); // padding

	quint32 contentOffset, frameListOffset;
	in >> (quint32&)contentOffset;
	in >> (quint32&)frameListOffset;

	// read content struct
	// subtract 8 from the offset because section.data doesn't contain
	// the nw4r::ut::BinaryBlockHeader whereas these offsets do count it
	in.device()->seek(startPos + contentOffset - 8);

	for (int i = 0; i < 4; i++)
		ReadRGBA8Color(contentVtxColours[i], in);

	// read the material name
	quint16 materialNum;
	in >> (quint16&)materialNum;

	contentMaterialName = m_layout.materials.keys().at(materialNum);

	// read texcoords
	quint8 texCoordNum;
	in >> (quint8&)texCoordNum;
	in.skipRawData(1); // padding

	contentTexCoords.resize(texCoordNum);

	foreach (LYTTexCoords texCoord, contentTexCoords) {
		for (int i = 0; i < 4; i++)
			ReadPointF(in, texCoord.coord[i]);
	}

	// read frame offset list
	// subtract 8 from the offset once again
	in.device()->seek(startPos + frameListOffset - 8);

	QVector<quint32> frameOffsets(frameCount);

	for (int i = 0; i < frameCount; i++) {
		quint32 offset;
		in >> (quint32&)offset;
		frameOffsets[i] = offset;
	}

	// now read each frame
	for (int i = 0; i < frameCount; i++) {
		in.device()->seek(frameOffsets[i] - 8);

		frames.append(new LYTWindowFrame(*this));
		frames.last()->readFromDataStream(in);
	}
}
