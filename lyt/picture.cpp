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

#include "picture.h"
#include "layout.h"
#include "common.h"


LYTPicture::LYTPicture(LYTLayout &layout) : LYTPane(layout) {
}


void LYTPicture::dumpToDebug(bool showHeading) {
	if (showHeading)
		qDebug() << "LYTPicture" << name << "@" << (void*)this;

	LYTPane::dumpToDebug(false);

	qDebug() << "- Vertex Colours:" << vtxColours[0] << "-" << vtxColours[1];
	qDebug() << "                 " << vtxColours[2] << "-" << vtxColours[3];
	qDebug() << "- Material:" << materialName;
	qDebug() << "- Tex Coords:" << texCoords.count();

	foreach (LYTTexCoords texCoord, texCoords) {
		qDebug() << "----" << texCoord.coord[0] << "-" << texCoord.coord[1] << "-" << texCoord.coord[2] << "-" << texCoord.coord[3];
	}
}




void LYTPicture::writeToDataStream(QDataStream &out) {
	LYTPane::writeToDataStream(out);

	for (int i = 0; i < 4; i++)
		WriteRGBA8Color(vtxColours[i], out);

	// calculate the material number
	int materialNum = m_layout.materials.keys().indexOf(materialName);
	out << (quint16)materialNum;

	// write texcoords
	out << (quint8)texCoords.count();
	out.skipRawData(1); // padding

	foreach (LYTTexCoords texCoord, texCoords) {
		for (int i = 0; i < 4; i++)
			WritePointF(out, texCoord.coord[i]);
	}
}


void LYTPicture::readFromDataStream(QDataStream &in) {
	LYTPane::readFromDataStream(in);

	for (int i = 0; i < 4; i++)
		ReadRGBA8Color(vtxColours[i], in);

	// read the material name
	quint16 materialNum;
	in >> (quint16&)materialNum;

	materialName = m_layout.materials.keys().at(materialNum);

	// read texcoords
	quint8 texCoordNum;
	in >> (quint8&)texCoordNum;
	in.skipRawData(1); // padding

	texCoords.resize(texCoordNum);

	foreach (LYTTexCoords texCoord, texCoords) {
		for (int i = 0; i < 4; i++)
			ReadPointF(in, texCoord.coord[i]);
	}
}
