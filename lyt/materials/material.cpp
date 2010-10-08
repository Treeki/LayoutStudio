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
	// todo: move dumpToDebug calls for the various Material structs out of
	// LYTMaterial::read...() and put them here, since it makes more sense
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


	LYTMaterialResourceNum resourceNum;
	in >> (quint32&)resourceNum.value;

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
		this->readChanCtrl(in);
	} else {
		this->hasChanCtrl = false;
	}

	// MatCol
	if (resourceNum.hasMatCol()) {
		this->hasMatCol = true;
		this->readMatCol(in);
	} else {
		this->hasMatCol = false;
	}

	// TevSwapTable
	if (resourceNum.hasTevSwapTable()) {
		this->hasTevSwapTable = true;
		this->readTevSwapTable(in);
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
		this->readAlphaCompare(in);
	} else {
		this->hasAlphaCompare = false;
	}

	// BlendMode
	if (resourceNum.hasBlendMode()) {
		this->hasBlendMode = true;
		this->readBlendMode(in);
	} else {
		this->hasBlendMode = false;
	}
}



void LYTMaterial::readTexMap(QDataStream &in) {
	this->texMaps.append(LYTTexMap());
	this->texMaps.last().readFromDataStream(in, m_layout);
	this->texMaps.last().dumpToDebug();
}

void LYTMaterial::readTexSRT(QDataStream &in) {
	this->texSRTs.append(LYTTexSRT());
	this->texSRTs.last().readFromDataStream(in);
	this->texSRTs.last().dumpToDebug();
}

void LYTMaterial::readTexCoordGen(QDataStream &in) {
	this->texCoordGens.append(LYTTexCoordGen());
	this->texCoordGens.last().readFromDataStream(in);
	this->texCoordGens.last().dumpToDebug();
}

void LYTMaterial::readChanCtrl(QDataStream &in) {
	this->chanCtrl.readFromDataStream(in);
	this->chanCtrl.dumpToDebug();
}

void LYTMaterial::readMatCol(QDataStream &in) {
	ReadRGBA8Color(this->matCol, in);
}

void LYTMaterial::readTevSwapTable(QDataStream &in) {
	this->tevSwapTable.readFromDataStream(in);
	this->tevSwapTable.dumpToDebug();
}

void LYTMaterial::readIndTexSRT(QDataStream &in) {
	this->indTexSRTs.append(LYTTexSRT());
	this->indTexSRTs.last().readFromDataStream(in);
	this->indTexSRTs.last().dumpToDebug();
}

void LYTMaterial::readIndirectStage(QDataStream &in) {
	this->indTexStages.append(LYTIndirectStage());
	this->indTexStages.last().readFromDataStream(in);
	this->indTexStages.last().dumpToDebug();
}

void LYTMaterial::readTevStage(QDataStream &in) {
	this->tevStages.append(LYTTevStage());
	this->tevStages.last().readFromDataStream(in);
	this->tevStages.last().dumpToDebug();
}

void LYTMaterial::readAlphaCompare(QDataStream &in) {
	this->alphaCompare.readFromDataStream(in);
	this->alphaCompare.dumpToDebug();
}

void LYTMaterial::readBlendMode(QDataStream &in) {
	this->blendMode.readFromDataStream(in);
	this->blendMode.dumpToDebug();
}

