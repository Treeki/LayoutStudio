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

#include "indirectstage.h"
#include "../layout.h"

LYTIndirectStage::LYTIndirectStage() {
}

void LYTIndirectStage::dumpToDebug() const {
	qDebug() << "LYTIndirectStage @" << (void*)this;
	qDebug() << "texCoord:" << this->texCoord << "- texMap:" << this->texMap;
	qDebug() << "wrap_s:" << this->wrap_s << "- wrap_t:" << this->wrap_t;
}


void LYTIndirectStage::writeToDataStream(QDataStream &out) const {
	out << (quint8)texCoord;
	out << (quint8)texMap;
	out << (quint8)wrap_s;
	out << (quint8)wrap_t;
}


void LYTIndirectStage::readFromDataStream(QDataStream &in) {
	in >> (quint8&)texCoord;
	in >> (quint8&)texMap;
	in >> (quint8&)wrap_s;
	in >> (quint8&)wrap_t;
}



