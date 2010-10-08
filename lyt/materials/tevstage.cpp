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

#include "tevstage.h"
#include "../layout.h"

LYTTevStage::LYTTevStage() {
}

void LYTTevStage::dumpToDebug() {
	qDebug() << "LYTTevStage @" << (void*)this;
}


void LYTTevStage::writeToDataStream(QDataStream &out) {
}


void LYTTevStage::readFromDataStream(QDataStream &in) {
	// not fun. at all.

	char data[0x10];
	in.readRawData(data, 0x10);

	// TEV order:
	texCoord = data[0];
	colour = data[1];
	texMap = data[2] | (BitExtract(data[3], 1, 32 - 1) << 8);

	// SwapMode:
	rasSwapMode = BitExtract(data[3], 2, 32 - 3);
	texSwapMode = BitExtract(data[3], 2, 32 - 5);

	// Colour In:
	colourInA = BitExtract(data[4], 4, 32 - 4);
	colourInB = BitExtract(data[4], 4, 32 - 8);
	colourInC = BitExtract(data[5], 4, 32 - 4);
	colourInD = BitExtract(data[5], 4, 32 - 8);

	// Colour Op:
	colourOp = BitExtract(data[6], 4, 32 - 4);
	colourBias = BitExtract(data[6], 2, 32 - 6);
	colourScale = BitExtract(data[6], 2, 32 - 8);
	colourClamp = BitExtract(data[7], 1, 32 - 1);
	colourOutReg = BitExtract(data[7], 2, 32 - 3);

	// Alpha In:
	alphaInA = BitExtract(data[8], 4, 32 - 4);
	alphaInB = BitExtract(data[8], 4, 32 - 8);
	alphaInC = BitExtract(data[9], 4, 32 - 4);
	alphaInD = BitExtract(data[9], 4, 32 - 8);

	// Alpha Op:
	alphaOp = BitExtract(data[10], 4, 32 - 4);
	alphaBias = BitExtract(data[10], 2, 32 - 6);
	alphaScale = BitExtract(data[10], 2, 32 - 8);
	alphaClamp = BitExtract(data[11], 1, 32 - 1);
	alphaOutReg = BitExtract(data[11], 2, 32 - 3);

	// Constants:
	colourConst = BitExtract(data[7], 5, 32 - 8);
	alphaConst = BitExtract(data[11], 5, 32 - 8);

	// Indirect:
	indStage = data[12];
	indFormat = BitExtract(data[15], 2, 32 - 2);
	indBias = BitExtract(data[13], 3, 32 - 3);
	indMatrix = BitExtract(data[13], 4, 32 - 7);
	indWrapS = BitExtract(data[14], 3, 32 - 3);
	indWrapT = BitExtract(data[14], 3, 32 - 6);
	indAddPrev = BitExtract(data[15], 1, 32 - 3);
	indUtcLod = BitExtract(data[15], 1, 32 - 4);
	indAlphaSel = BitExtract(data[15], 2, 32 - 6);
}



