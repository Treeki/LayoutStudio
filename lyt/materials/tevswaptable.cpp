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

#include "tevswaptable.h"
#include "../layout.h"

LYTTevSwapTable::LYTTevSwapTable() {
}

void LYTTevSwapTable::dumpToDebug() {
	qDebug() << "LYTTevSwapTable @" << (void*)this;

	for (int i = 0; i < 4; i++) {
		LYTTevSwapMode *m = &mode[i];
		qDebug() << i << ":" << m->red << "," << m->green << "," << m->blue << "," << m->alpha;
	}
}



void LYTTevSwapTable::writeToDataStream(QDataStream &out) {
	for (int i = 0; i < 4; i++) {
		quint8 val = 0;
		val |= mode[i].red;
		val |= (mode[i].green) << 2;
		val |= (mode[i].blue) << 4;
		val |= (mode[i].alpha) << 6;

		out << (quint8)val;
	}
}


void LYTTevSwapTable::readFromDataStream(QDataStream &in) {
	for (int i = 0; i < 4; i++) {
		quint8 val;
		in >> (quint8&)val;

		mode[i].red = BitExtract(val, 2, 32 - 8);
		mode[i].green = BitExtract(val, 2, 32 - 6);
		mode[i].blue = BitExtract(val, 2, 32 - 4);
		mode[i].alpha = BitExtract(val, 2, 32 - 2);
	}
}


