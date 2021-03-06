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

#include "pane.h"
#include "layout.h"

LYTPane::LYTPane(LYTLayout &layout) : m_layout(layout) {
	this->parent = 0;
	m_type = PaneType;
}

LYTPane::~LYTPane() {
	foreach (LYTPane *ptr, this->children)
		delete ptr;
}

LYTPane *LYTPane::findPaneByName(QString name, bool recursive) const {
	foreach (LYTPane *pane, this->children) {
		if (pane->name == name)
			return pane;

		if (recursive) {
			LYTPane *tryThis = pane->findPaneByName(name, recursive);
			if (tryThis != 0)
				return tryThis;
		}
	}

	return 0;
}

LYTLayout &LYTPane::layout() const {
	return m_layout;
}


Magic LYTPane::magic() const {
	return Magic('pan1');
}

void LYTPane::dumpToDebug(bool showHeading) const {
	if (showHeading)
		qDebug() << "LYTPane" << name << "@" << (void*)this;

	qDebug() << "- Translation:" << xTrans << "," << yTrans << "," << zTrans;
	qDebug() << "- Rotation:" << xRot << "," << yRot << "," << zRot;
	qDebug() << "- Scale:" << xScale << "," << yScale;
	qDebug() << "- Size:" << width << "x" << height;
	qDebug() << "- Visible:" << visible << "- Influenced Alpha:" << influencedAlpha << "- Widescreen:" << isWidescreen;
	qDebug() << "- Origin:" << horzOrigin << "," << vertOrigin;
	qDebug() << "- Alpha:" << alpha << "- Userdata:" << userdata;
}



void LYTPane::writeToDataStream(QDataStream &out) const {
	quint8 flags =
			(visible ? 1 : 0) |
			(influencedAlpha ? 2 : 0) |
			(isWidescreen ? 4 : 0);

	out << (quint8)flags;
	out << (quint8)((int)horzOrigin + ((int)vertOrigin * 3));
	out << (quint8)alpha;
	WritePadding(1, out);

	WriteFixedLengthASCII(out, name, 0x10);
	WriteFixedLengthASCII(out, userdata, 8);

	out << (float)xTrans;
	out << (float)yTrans;
	out << (float)zTrans;
	out << (float)xRot;
	out << (float)yRot;
	out << (float)zRot;
	out << (float)xScale;
	out << (float)yScale;
	out << (float)width;
	out << (float)height;
}

void LYTPane::readFromDataStream(QDataStream &in) {
	quint8 flags;
	in >> flags;

	visible = (flags & 1);
	influencedAlpha = (flags & 2);
	isWidescreen = (flags & 4);

	quint8 rawOrigin;
	in >> rawOrigin;
	horzOrigin = (OriginType)(rawOrigin % 3);
	vertOrigin = (OriginType)(rawOrigin / 3);
	in >> (quint8&)alpha;
	in.skipRawData(1); // padding

	name = ReadFixedLengthASCII(in, 0x10);
	userdata = ReadFixedLengthASCII(in, 8);

	in >> (float&)xTrans;
	in >> (float&)yTrans;
	in >> (float&)zTrans;
	in >> (float&)xRot;
	in >> (float&)yRot;
	in >> (float&)zRot;
	in >> (float&)xScale;
	in >> (float&)yScale;
	in >> (float&)width;
	in >> (float&)height;
}


void LYTPane::addFontRefsToList(QStringList &list) const {
	//qDebug() << "Getting font refs for" << this->name;

	foreach (LYTPane *p, this->children) {
		p->addFontRefsToList(list);
	}
}


float LYTPane::drawnVertexX() const {
	switch (horzOrigin) {
	case Center:
		return -width / 2.0f;
	case Right:
		return -width;
	default:
		return 0.0f;
	}
}

float LYTPane::drawnVertexY() const {
	switch (vertOrigin) {
	case Center:
		return height / 2.0f;
	case Bottom:
		return height;
	default:
		return 0.0f;
	}
}
