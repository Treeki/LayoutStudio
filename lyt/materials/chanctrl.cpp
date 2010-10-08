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

#include "chanctrl.h"
#include "../layout.h"

LYTChanCtrl::LYTChanCtrl() {
}

void LYTChanCtrl::dumpToDebug() {
	qDebug() << "LYTChanCtrl @" << (void*)this;
	qDebug() << "ColourMatSrc:" << colourMatSrc << "- AlphaMatSrc:" << alphaMatSrc;
}


void LYTChanCtrl::writeToDataStream(QDataStream &out) {
	out << (quint8)colourMatSrc;
	out << (quint8)alphaMatSrc;
	out.skipRawData(2); // padding
}


void LYTChanCtrl::readFromDataStream(QDataStream &in) {
	in >> (quint8&)colourMatSrc;
	in >> (quint8&)alphaMatSrc;
	in.skipRawData(2); // padding
}

