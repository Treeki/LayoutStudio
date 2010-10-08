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

#include "blendmode.h"
#include "../layout.h"

LYTBlendMode::LYTBlendMode() {
}

void LYTBlendMode::dumpToDebug() {
	qDebug() << "LYTBlendMode @" << (void*)this;
	qDebug() << "srcFactor:" << srcFactor << "- destFactor:" << destFactor;
	qDebug() << "op:" << op << "- type:" << type;
}


void LYTBlendMode::writeToDataStream(QDataStream &out) {
	out << (quint8)type;
	out << (quint8)srcFactor;
	out << (quint8)destFactor;
	out << (quint8)op;
}


void LYTBlendMode::readFromDataStream(QDataStream &in) {
	in >> (quint8&)type;
	in >> (quint8&)srcFactor;
	in >> (quint8&)destFactor;
	in >> (quint8&)op;
}



