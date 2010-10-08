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

#ifndef LYTTEVSTAGE_H
#define LYTTEVSTAGE_H

#include "../common.h"
#include <QtCore/QDataStream>

class LYTTevStage {
public:
	LYTTevStage();

	void writeToDataStream(QDataStream &out);
	void readFromDataStream(QDataStream &in);

	void dumpToDebug();

	// TEV order:
	int texCoord;
	int colour;
	int texMap;

	// SwapMode;
	int rasSwapMode;
	int texSwapMode;

	// Colour In:
	int colourInA;
	int colourInB;
	int colourInC;
	int colourInD;

	// Colour Op:
	int colourOp;
	int colourBias;
	int colourScale;
	int colourClamp;
	int colourOutReg;

	// Alpha In:
	int alphaInA;
	int alphaInB;
	int alphaInC;
	int alphaInD;

	// Alpha Op:
	int alphaOp;
	int alphaBias;
	int alphaScale;
	int alphaClamp;
	int alphaOutReg;

	// Constants:
	int colourConst;
	int alphaConst;

	// Indirect:
	int indStage;
	int indFormat;
	int indBias;
	int indMatrix;
	int indWrapS;
	int indWrapT;
	int indAddPrev;
	int indUtcLod;
	int indAlphaSel;
};


#endif // LYTTEVSAGE_H
