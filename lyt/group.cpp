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

#include "group.h"

LYTGroup::LYTGroup() {
}




void LYTGroup::writeToDataStream(QDataStream &out) {
	WriteFixedLengthASCII(out, name, 0x10);

	// write the contents
	out << (quint16)panes.count();
	out.skipRawData(2); // padding

	foreach (LYTPane *pane, panes) {
		WriteFixedLengthASCII(out, pane->name, 0x10);
	}
}


void LYTGroup::readFromDataStream(QDataStream &in, LYTPane &linkedPane) {
	name = ReadFixedLengthASCII(in, 0x10);
	qDebug() << "reading group" << name;

	// read the contents
	quint16 paneCount;
	in >> (quint16&)paneCount;
	in.skipRawData(2); // padding

	for (int i = 0; i < paneCount; i++) {
		QString paneName = ReadFixedLengthASCII(in, 0x10);

		qDebug() << "found" << paneName << "in group" << this->name;

		this->panes.append(linkedPane.findPaneByName(paneName, true));
	}
}
