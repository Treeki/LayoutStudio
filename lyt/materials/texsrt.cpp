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

#include "texsrt.h"
#include "../layout.h"

LYTTexSRT::LYTTexSRT() {
}

void LYTTexSRT::dumpToDebug() const {
	qDebug() << "LYTTexSRT @" << (void*)this;
	qDebug() << "Scale:" << xScale << "," << yScale;
	qDebug() << "Rotation:" << rotate;
	qDebug() << "Translation:" << xTrans << "," << yTrans;
}


void LYTTexSRT::writeToDataStream(QDataStream &out) const {
	out << (float)xTrans;
	out << (float)yTrans;
	out << (float)rotate;
	out << (float)xScale;
	out << (float)yScale;
}


void LYTTexSRT::readFromDataStream(QDataStream &in) {
	in >> (float&)xTrans;
	in >> (float&)yTrans;
	in >> (float&)rotate;
	in >> (float&)xScale;
	in >> (float&)yScale;
}

