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

void LYTTevStage::dumpToDebug() const {
	qDebug() << "LYTTevStage @" << (void*)this;
	qDebug() << "TEV order: texCoord" << texCoord << ", colour" << colour << ", texMap" << texMap;
	qDebug() << "SwapMode: ras" << rasSwapMode << ", tex" << texSwapMode;

	qDebug() << "Colour In:" << colourInA << colourInB << colourInC << colourInD;
	qDebug() << "Op:" << colourOp << ", Bias:" << colourBias << ", Scale:" << colourScale;
	qDebug() << "Clamp:" << colourClamp << ", OutReg:" << colourOutReg;

	qDebug() << "Alpha In:" << alphaInA << alphaInB << alphaInC << alphaInD;
	qDebug() << "Op:" << alphaOp << ", Bias:" << alphaBias << ", Scale:" << alphaScale;
	qDebug() << "Clamp:" << alphaClamp << ", OutReg:" << alphaOutReg;

	qDebug() << "Colour Const:" << colourConst << ", Alpha Const:" << alphaConst;

	qDebug() << "Indirect Stage:" << indStage << ", Format:" << indFormat;
	qDebug() << "Bias:" << indBias << ", Matrix:" << indMatrix;
	qDebug() << "WrapS:" << indWrapS << ", WrapT:" << indWrapT << ", AddPrev:" << indAddPrev;
	qDebug() << "UtcLod:" << indUtcLod << ", AlphaSel:" << indAlphaSel;
}


void LYTTevStage::writeToDataStream(QDataStream &out) const {
	char data[0x10];
	qMemSet(data, 0, 0x10);

	// TEV order:
	data[0] = texCoord;
	data[1] = colour;
	data[2] = texMap & 0xFF;
	data[3] = BitInsert(data[3], texMap >> 8, 1, 32 - 1);

	// SwapMode:
	data[3] = BitInsert(data[3], rasSwapMode, 2, 32 - 3);
	data[3] = BitInsert(data[3], texSwapMode, 2, 32 - 5);

	// Colour In:
	data[4] = BitInsert(data[4], colourInA, 4, 32 - 4);
	data[4] = BitInsert(data[4], colourInB, 4, 32 - 8);
	data[5] = BitInsert(data[5], colourInC, 4, 32 - 4);
	data[5] = BitInsert(data[5], colourInD, 4, 32 - 8);

	// Colour Op:
	data[6] = BitInsert(data[6], colourOp, 4, 32 - 4);
	data[6] = BitInsert(data[6], colourBias, 2, 32 - 6);
	data[6] = BitInsert(data[6], colourScale, 2, 32 - 8);
	data[7] = BitInsert(data[7], colourClamp, 1, 32 - 1);
	data[7] = BitInsert(data[7], colourOutReg, 2, 32 - 3);

	// Alpha In:
	data[8] = BitInsert(data[8], alphaInA, 4, 32 - 4);
	data[8] = BitInsert(data[8], alphaInB, 4, 32 - 8);
	data[9] = BitInsert(data[9], alphaInC, 4, 32 - 4);
	data[9] = BitInsert(data[9], alphaInD, 4, 32 - 8);

	// Alpha Op:
	data[10] = BitInsert(data[10], alphaOp, 4, 32 - 4);
	data[10] = BitInsert(data[10], alphaBias, 2, 32 - 6);
	data[10] = BitInsert(data[10], alphaScale, 2, 32 - 8);
	data[11] = BitInsert(data[11], alphaClamp, 1, 32 - 1);
	data[11] = BitInsert(data[11], alphaOutReg, 2, 32 - 3);

	// Constants:
	data[7] = BitInsert(data[7], colourConst, 5, 32 - 8);
	data[11] = BitInsert(data[11], alphaConst, 5, 32 - 8);

	// Indirect:
	data[12] = indStage;
	data[15] = BitInsert(data[15], indFormat, 2, 32 - 2);
	data[13] = BitInsert(data[13], indBias, 3, 32 - 3);
	data[13] = BitInsert(data[13], indMatrix, 4, 32 - 7);
	data[14] = BitInsert(data[14], indWrapS, 3, 32 - 3);
	data[14] = BitInsert(data[14], indWrapT, 3, 32 - 6);
	data[15] = BitInsert(data[15], indAddPrev, 1, 32 - 3);
	data[15] = BitInsert(data[15], indUtcLod, 1, 32 - 4);
	data[15] = BitInsert(data[15], indAlphaSel, 2, 32 - 6);

	out.writeRawData(data, 0x10);
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



