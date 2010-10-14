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


void LYTMaterial::dumpToDebug() const {
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



void LYTMaterial::writeToDataStream(QDataStream &out) const {
	WriteFixedLengthASCII(out, name, 0x14);

	for (int i = 0; i < 3; i++)
		WriteS10Color(this->colours[i], out);

	for (int i = 0; i < 4; i++)
		WriteRGBA8Color(this->tevKColour[i], out);


	LYTMaterialResourceNum resourceNum;
	resourceNum.setTexMapNum(texMaps.count());
	resourceNum.setTexSRTNum(texSRTs.count());
	resourceNum.setTexCoordGenNum(texCoordGens.count());
	resourceNum.setHasChanCtrl(hasChanCtrl);
	resourceNum.setHasMatCol(hasMatCol);
	resourceNum.setHasTevSwapTable(hasTevSwapTable);
	resourceNum.setIndTexSRTNum(indTexSRTs.count());
	resourceNum.setIndTexStageNum(indTexStages.count());
	resourceNum.setTevStageNum(tevStages.count());
	resourceNum.setHasAlphaCompare(hasAlphaCompare);
	resourceNum.setHasBlendMode(hasBlendMode);
	out << (quint32)resourceNum.value();


	// TexMap
	for (int i = 0; i < texMaps.count(); i++) {
		texMaps.at(i).writeToDataStream(out, this->m_layout);
	}

	// TexSRT
	for (int i = 0; i < texSRTs.count(); i++) {
		texSRTs.at(i).writeToDataStream(out);
	}

	// TexCoordGen
	for (int i = 0; i < texCoordGens.count(); i++) {
		texCoordGens.at(i).writeToDataStream(out);
	}

	// ChanCtrl
	if (hasChanCtrl) {
		chanCtrl.writeToDataStream(out);
	}

	// MatCol
	if (hasMatCol) {
		WriteRGBA8Color(this->matCol, out);
	}

	// TevSwapTable
	if (hasTevSwapTable) {
		tevSwapTable.writeToDataStream(out);
	}

	// IndTexSRT
	for (int i = 0; i < indTexSRTs.count(); i++) {
		indTexSRTs.at(i).writeToDataStream(out);
	}

	// IndTexStage
	for (int i = 0; i < indTexStages.count(); i++) {
		indTexStages.at(i).writeToDataStream(out);
	}

	// TevStage
	for (int i = 0; i < tevStages.count(); i++) {
		tevStages.at(i).writeToDataStream(out);
	}

	// AlphaCompare
	if (hasAlphaCompare) {
		alphaCompare.writeToDataStream(out);
	}

	// BlendMode
	if (hasBlendMode) {
		blendMode.writeToDataStream(out);
	}

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


