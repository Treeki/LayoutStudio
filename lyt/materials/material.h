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

#ifndef LYTMATERIAL_H
#define LYTMATERIAL_H

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QDataStream>
#include <QtCore/QDebug>

#include "../common.h"
#include "texmap.h"
#include "texsrt.h"
#include "texcoordgen.h"
#include "chanctrl.h"
#include "tevswaptable.h"
#include "indirectstage.h"
#include "tevstage.h"
#include "alphacompare.h"
#include "blendmode.h"


class LYTLayout;


class LYTMaterialResourceNum {
public:
	LYTMaterialResourceNum() : m_value(0) { }
	LYTMaterialResourceNum(int initValue) : m_value(initValue) { }

	quint32 value() const { return m_value; }

	int getTexMapNum()		const { return BitExtract(m_value, 2, 30); }
	int getTexSRTNum()		const { return BitExtract(m_value, 2, 28); }
	int getTexCoordGenNum()	const { return BitExtract(m_value, 2, 26); }
	bool hasChanCtrl()		const { return BitExtract(m_value, 1, 6);  }
	bool hasMatCol()		const { return BitExtract(m_value, 1, 4);  }
	bool hasTevSwapTable()	const { return BitExtract(m_value, 1, 19); }
	bool hasAlphaCompare()	const { return BitExtract(m_value, 1, 8);  }
	bool hasBlendMode()		const { return BitExtract(m_value, 1, 7);  }
	int getIndTexSRTNum()	const { return BitExtract(m_value, 2, 17); }
	int getIndTexStageNum()	const { return BitExtract(m_value, 3, 14); }
	int getTevStageNum()	const { return BitExtract(m_value, 5, 9);  }

	void setTexMapNum(int v)		{ m_value = BitInsert(m_value, v, 4, 28); }
	void setTexSRTNum(int v)		{ m_value = BitInsert(m_value, v, 4, 24); }
	void setTexCoordGenNum(int v)	{ m_value = BitInsert(m_value, v, 4, 20); }
	void setHasChanCtrl(bool v)		{ m_value = BitInsert(m_value, v, 1, 6); }
	void setHasMatCol(bool v)		{ m_value = BitInsert(m_value, v, 1, 4); }
	void setHasTevSwapTable(bool v)	{ m_value = BitInsert(m_value, v, 1, 19); }
	void setHasAlphaCompare(bool v)	{ m_value = BitInsert(m_value, v, 1, 8); }
	void setHasBlendMode(bool v)	{ m_value = BitInsert(m_value, v, 1, 7); }
	void setIndTexSRTNum(int v)		{ m_value = BitInsert(m_value, v, 2, 17); }
	void setIndTexStageNum(int v)	{ m_value = BitInsert(m_value, v, 3, 14); }
	void setTevStageNum(int v)		{ m_value = BitInsert(m_value, v, 5, 9); }

private:
	quint32 m_value;
};





class LYTMaterial {
public:
	LYTMaterial(LYTLayout &layout);
	~LYTMaterial();


	void writeToDataStream(QDataStream &out) const;
	void readFromDataStream(QDataStream &in);

	void dumpToDebug() const;

	LYTLayout &layout() const;

	QString name;

	QColor colours[3];
	QColor tevKColour[4];

	QList<LYTTexMap> texMaps;
	QList<LYTTexSRT> texSRTs;
	QList<LYTTexCoordGen> texCoordGens;

	bool hasChanCtrl;
	LYTChanCtrl chanCtrl;

	bool hasMatCol;
	QColor matCol;

	bool hasTevSwapTable;
	LYTTevSwapTable tevSwapTable;

	bool hasAlphaCompare;
	LYTAlphaCompare alphaCompare;

	bool hasBlendMode;
	LYTBlendMode blendMode;

	QList<LYTIndirectStage> indTexStages;
	QList<LYTTexSRT> indTexSRTs;

	QList<LYTTevStage> tevStages;



protected:
	LYTLayout &m_layout;

	void readTexMap(QDataStream &in);
	void readTexSRT(QDataStream &in);
	void readTexCoordGen(QDataStream &in);
	void readIndirectStage(QDataStream &in);
	void readIndTexSRT(QDataStream &in);
	void readTevStage(QDataStream &in);
};

#endif // LYTMATERIAL_H
