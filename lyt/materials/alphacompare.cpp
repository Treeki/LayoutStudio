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

#include "alphacompare.h"
#include "../layout.h"

LYTAlphaCompare::LYTAlphaCompare() {
}

void LYTAlphaCompare::dumpToDebug() const {
	qDebug() << "LYTAlphaCompare @" << (void*)this;
	qDebug() << "Comp0:" << comp0 << "- Ref0:" << ref0;
	qDebug() << "Comp1:" << comp1 << "- Ref1:" << ref1;
	qDebug() << "Op:" << op;
}


void LYTAlphaCompare::writeToDataStream(QDataStream &out) const {
	out << (quint8)(comp0 | (comp1 << 4));
	out << (quint8)op;
	out << (quint8)ref0;
	out << (quint8)ref1;
}


void LYTAlphaCompare::readFromDataStream(QDataStream &in) {
	quint8 value;

	in >> (quint8&)value;
	comp0 = BitExtract(value, 4, 32 - 4);
	comp1 = BitExtract(value, 4, 32 - 8);

	in >> (quint8&)op;
	in >> (quint8&)ref0;
	in >> (quint8&)ref1;
}


