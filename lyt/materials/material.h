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

class LYTLayout;


class LYTMaterialResourceNum {
public:
	quint32 value;

	inline  int getTexMapNum()		{ return BitExtract(value, 4, 28); }
	inline  int getTexSRTNum()		{ return BitExtract(value, 4, 24); }
	inline  int getTexCoordGenNum()	{ return BitExtract(value, 4, 20); }
	inline bool hasChanCtrl()		{ return BitExtract(value, 1, 6);  }
	inline bool hasMatCol()			{ return BitExtract(value, 1, 4);  }
	inline bool hasTevSwapTable()	{ return BitExtract(value, 1, 19); }
	inline bool hasAlphaCompare()	{ return BitExtract(value, 1, 8);  }
	inline bool hasBlendMode()		{ return BitExtract(value, 1, 7);  }
	inline  int getIndTexSRTNum()	{ return BitExtract(value, 2, 17); }
	inline  int getIndTexStageNum()	{ return BitExtract(value, 3, 14); }
	inline  int getTevStageNum()	{ return BitExtract(value, 5, 9);  }
};



class LYTChanCtrl {
public:
	void writeToDataStream(QDataStream &out);
	void readFromDataStream(QDataStream &in);

	void dumpToDebug();

	quint8 colourMatSrc;
	quint8 alphaMatSrc;
};

class LYTTevSwapMode {
public:
	void writeToDataStream(QDataStream &out);
	void readFromDataStream(QDataStream &in);

	void dumpToDebug();

	int red;
	int green;
	int blue;
	int alpha;
};

class LYTIndirectStage {
public:
	void writeToDataStream(QDataStream &out);
	void readFromDataStream(QDataStream &in);

	void dumpToDebug();

};

class LYTTevStage {
public:
	void writeToDataStream(QDataStream &out);
	void readFromDataStream(QDataStream &in);

	void dumpToDebug();

};

class LYTAlphaCompare {
public:
	void writeToDataStream(QDataStream &out);
	void readFromDataStream(QDataStream &in);

	void dumpToDebug();

};

class LYTBlendMode {
public:
	void writeToDataStream(QDataStream &out);
	void readFromDataStream(QDataStream &in);

	void dumpToDebug();

};




class LYTMaterial {
public:
	LYTMaterial(LYTLayout &layout);
	~LYTMaterial();


	void writeToDataStream(QDataStream &out);
	void readFromDataStream(QDataStream &in);

	void dumpToDebug();

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
	LYTTevSwapMode tevSwapTable;

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
	void readChanCtrl(QDataStream &in);
	void readMatCol(QDataStream &in);
	void readTevSwapTable(QDataStream &in);
	void readAlphaCompare(QDataStream &in);
	void readBlendMode(QDataStream &in);
	void readIndirectStage(QDataStream &in);
	void readIndTexSRT(QDataStream &in);
	void readTevStage(QDataStream &in);
};

#endif // LYTMATERIAL_H
