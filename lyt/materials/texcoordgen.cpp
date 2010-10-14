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

#include "texcoordgen.h"
#include "../layout.h"

LYTTexCoordGen::LYTTexCoordGen() {
}

void LYTTexCoordGen::dumpToDebug() const {
	qDebug() << "LYTTexCoordGen @" << (void*)this;
	qDebug() << "GenType:" << genType << "- Mtx:" << mtx << "- Src:" << src;
}


void LYTTexCoordGen::writeToDataStream(QDataStream &out) const {
	out << (quint8)genType;
	out << (quint8)src;
	out << (quint8)mtx;
	WritePadding(1, out);
}


void LYTTexCoordGen::readFromDataStream(QDataStream &in) {
	in >> (quint8&)genType;
	in >> (quint8&)src;
	in >> (quint8&)mtx;
	in.skipRawData(1); // padding
}

