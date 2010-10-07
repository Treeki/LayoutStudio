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
	/*texSRTs.clear();

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
	}*/
}



void LYTMaterial::readTexMap(QDataStream &in) {
	this->texMaps.append(LYTTexMap());
	this->texMaps.last().readFromDataStream(in, m_layout);
}
