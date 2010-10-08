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

#include "material.h"
#include "../layout.h"

LYTMaterial::LYTMaterial(LYTLayout &layout) : m_layout(layout) {
}

LYTMaterial::~LYTMaterial() {
}

LYTLayout &LYTMaterial::layout() const {
	return m_layout;
}


void LYTMaterial::dumpToDebug() {
	qDebug() << "LYTMaterial" << name << "@" << (void*)this;

	for (int i = 0; i < 3; i++)
		qDebug() << "Colour" << i << ":" << colours[i];

	for (int i = 0; i < 4; i++)
		qDebug() << "TEV Constant Colour" << i << ":" << tevKColour[i];

	qDebug() << "TexMaps:" << texMaps.count();
	foreach (LYTTexMap texMap, texMaps)
		texMap.dumpToDebug();

	qDebug() << "TexSRTs:" << texSRTs.count();
	foreach (LYTTexSRT texSRT, texSRTs)
		texSRT.dumpToDebug();

	qDebug() << "TexCoordGens:" << texCoordGens.count();
	foreach (LYTTexCoordGen texCoordGen, texCoordGens)
		texCoordGen.dumpToDebug();

	if (hasChanCtrl)
		chanCtrl.dumpToDebug();
	else
		qDebug() << "ChanCtrl: none";

	if (hasMatCol)
		qDebug() << "MatCol:" << matCol;
	else
		qDebug() << "MatCol: none";

	if (hasTevSwapTable)
		tevSwapTable.dumpToDebug();
	else
		qDebug() << "TevSwapTable: none";

	qDebug() << "IndTexSRTs:" << indTexSRTs.count();
	foreach (LYTTexSRT indTexSRT, indTexSRTs)
		indTexSRT.dumpToDebug();

	qDebug() << "IndirectStages:" << indTexStages.count();
	foreach (LYTIndirectStage indTexStage, indTexStages)
		indTexStage.dumpToDebug();

	qDebug() << "TevStages:" << tevStages.count();
	foreach (LYTTevStage tevStage, tevStages)
		tevStage.dumpToDebug();

	if (hasAlphaCompare)
		alphaCompare.dumpToDebug();
	else
		qDebug() << "AlphaCompare: none";

	if (hasBlendMode)
		blendMode.dumpToDebug();
	else
		qDebug() << "BlendMode: none";
}



void LYTMaterial::writeToDataStream(QDataStream &out) {
	WriteFixedLengthASCII(out, name, 0x14);
}

void LYTMaterial::readFromDataStream(QDataStream &in) {
	name = ReadFixedLengthASCII(in, 0x14);

	for (int i = 0; i < 3; i++)
		ReadS10Color(this->colours[i], in);

	for (int i = 0; i < 4; i++)
		ReadRGBA8Color(this->tevKColour[i], in);


	quint32 resNumValue;
	in >> (quint32&)resNumValue;
	LYTMaterialResourceNum resourceNum(resNumValue);

	// this is really complicated -_-
	// first off: TexMap
	texMaps.clear();

	for (int i = 0; i < resourceNum.getTexMapNum(); i++) {
		this->readTexMap(in);
	}

	// TexSRT
	texSRTs.clear();

	for (int i = 0; i < resourceNum.getTexSRTNum(); i++) {
		this->readTexSRT(in);
	}

	// TexCoordGen
	texCoordGens.clear();

	for (int i = 0; i < resourceNum.getTexCoordGenNum(); i++) {
		this->readTexCoordGen(in);
	}

	// ChanCtrl
	if (resourceNum.hasChanCtrl()) {
		this->hasChanCtrl = true;
		this->chanCtrl.readFromDataStream(in);
	} else {
		this->hasChanCtrl = false;
	}

	// MatCol
	if (resourceNum.hasMatCol()) {
		this->hasMatCol = true;
		ReadRGBA8Color(this->matCol, in);
	} else {
		this->hasMatCol = false;
	}

	// TevSwapTable
	if (resourceNum.hasTevSwapTable()) {
		this->hasTevSwapTable = true;
		this->tevSwapTable.readFromDataStream(in);
	} else {
		this->hasTevSwapTable = false;
	}

	// IndTexSRT
	indTexSRTs.clear();

	for (int i = 0; i < resourceNum.getIndTexSRTNum(); i++) {
		this->readIndTexSRT(in);
	}

	// IndTexStage
	indTexStages.clear();

	for (int i = 0; i < resourceNum.getIndTexStageNum(); i++) {
		this->readIndirectStage(in);
	}

	// TevStage
	tevStages.clear();

	for (int i = 0; i < resourceNum.getTevStageNum(); i++) {
		this->readTevStage(in);
	}

	// AlphaCompare
	if (resourceNum.hasAlphaCompare()) {
		this->hasAlphaCompare = true;
		this->alphaCompare.readFromDataStream(in);
	} else {
		this->hasAlphaCompare = false;
	}

	// BlendMode
	if (resourceNum.hasBlendMode()) {
		this->hasBlendMode = true;
		this->blendMode.readFromDataStream(in);
	} else {
		this->hasBlendMode = false;
	}
}



void LYTMaterial::readTexMap(QDataStream &in) {
	this->texMaps.append(LYTTexMap());
	this->texMaps.last().readFromDataStream(in, m_layout);
}

void LYTMaterial::readTexSRT(QDataStream &in) {
	this->texSRTs.append(LYTTexSRT());
	this->texSRTs.last().readFromDataStream(in);
}

void LYTMaterial::readTexCoordGen(QDataStream &in) {
	this->texCoordGens.append(LYTTexCoordGen());
	this->texCoordGens.last().readFromDataStream(in);
}

void LYTMaterial::readIndTexSRT(QDataStream &in) {
	this->indTexSRTs.append(LYTTexSRT());
	this->indTexSRTs.last().readFromDataStream(in);
}

void LYTMaterial::readIndirectStage(QDataStream &in) {
	this->indTexStages.append(LYTIndirectStage());
	this->indTexStages.last().readFromDataStream(in);
}

void LYTMaterial::readTevStage(QDataStream &in) {
	this->tevStages.append(LYTTevStage());
	this->tevStages.last().readFromDataStream(in);
}


