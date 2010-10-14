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

#ifndef WIIARCHIVEU8_H
#define WIIARCHIVEU8_H

#include "common.h"
#include "filesystem.h"
#include "stringtablebuilder.h"


struct U8ReadInfo {
	qint64 startPos;
	QString stringTable;
	int currentNode;
};

struct U8WriteInfo {
	qint64 startPos;
	WiiStringTableBuilder stringTableBuilder;
	int currentRecursionLevel;
	int currentNode;
	int nextDataOffset;
};


class WiiArchiveU8 {
public:
	WiiArchiveU8();
	WiiArchiveU8(QDataStream &stream);

	WiiDirectory root;

	void writeToDataStream(QDataStream &out);

private:
	void readDir(QDataStream &in, WiiDirectory &dir, int lastChild, U8ReadInfo &info);

	void addNodeToStringTable(WiiFSObject &node, WiiStringTableBuilder &table);
	void countNode(WiiFSObject &node, int *countPtr);

	void writeDir(QDataStream &out, WiiDirectory &dir, U8WriteInfo &info);

	void writeNodeData(QDataStream &out, WiiFSObject &node);
};

#endif // WIIARCHIVEU8_H
