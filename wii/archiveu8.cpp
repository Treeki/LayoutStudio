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

#include "archiveu8.h"






WiiArchiveU8::WiiArchiveU8() { }

WiiArchiveU8::WiiArchiveU8(QDataStream &stream) {
	U8ReadInfo info;
	info.startPos = stream.device()->pos();

	quint32 magic;
	stream >> (quint32&)magic;

	if (magic != 0x55AA382D)
		qWarning() << "WiiArchiveU8: tried to load an archive without the U8 magic";

	quint32 fstStart, fstSize, dataOffset;
	stream >> (quint32&)fstStart;
	stream >> (quint32&)fstSize;
	stream >> (quint32&)dataOffset;

	// read the FST
	stream.device()->seek(info.startPos + fstStart);

	// read the root node
	quint32 rootNodeLastChild;

	stream.skipRawData(8); // ignore type, nameOffset and dataOffset
	stream >> (quint32&)rootNodeLastChild;

	// but before we do this, read the string table
	qint64 savePos = stream.device()->pos();
	stream.device()->seek(savePos + ((rootNodeLastChild - 1) * 12));

	int stringTableLength = fstSize - (rootNodeLastChild * 12);

	QByteArray rawStringTable;
	rawStringTable.resize(stringTableLength);
	stream.readRawData(rawStringTable.data(), stringTableLength);

	info.stringTable = QString::fromAscii(rawStringTable.constData(), stringTableLength);

	// now read the root node
	stream.device()->seek(savePos);

	qDebug() << "Reading root node: last child is" << rootNodeLastChild;
	info.currentNode = 1;
	this->readDir(stream, this->root, rootNodeLastChild, info);
}


void WiiArchiveU8::readDir(QDataStream &in, WiiDirectory &dir, int lastChild, U8ReadInfo &info) {
	// read every node in this directory

	while (info.currentNode < lastChild) {
		info.currentNode++;

		//qDebug() << "Reading @ pos" << in.device()->pos();

		quint32 value, dataOffset, size;
		in >> (quint32&)value;
		in >> (quint32&)dataOffset;
		in >> (quint32&)size;

		int nameOffset = value & 0xFFFFFF;
		int type = value >> 24;

		WiiFSObject *newObj;
		if (type == 0)
			newObj = new WiiFile;
		else if (type == 1)
			newObj = new WiiDirectory;
		else
			qFatal("WiiArchiveU8::readDir: Unknown fs obj type %d", type);

		// get the name
		int nameSize = info.stringTable.indexOf(QChar::Null, nameOffset) - nameOffset;
		newObj->name = info.stringTable.mid(nameOffset, nameSize);

		qDebug() << "Reading node" << info.currentNode << newObj->name << "- type:" << type << "size:" << size << "offset:" << dataOffset;

		// read the contents
		if (newObj->isFile()) {
			// get the file data
			qint64 savePos = in.device()->pos();
			in.device()->seek(info.startPos + dataOffset);

			WiiFile *file = (WiiFile*)newObj;
			file->data.resize(size);
			in.readRawData(file->data.data(), size);

			in.device()->seek(savePos);

		} else if (newObj->isDirectory()) {
			// read the children
			this->readDir(in, *((WiiDirectory*)newObj), size, info);

		}

		qDebug() << "Adding" << newObj->name << "to" << dir.name;
		dir.addChild(newObj);
	};
}



void WiiArchiveU8::writeToDataStream(QDataStream &out) {
	U8WriteInfo info;

	// first off, before we do anything else, create the string table
	this->addNodeToStringTable(this->root, info.stringTableBuilder);

	QByteArray stringTable = info.stringTableBuilder.pack();

	// calculate various fun offsets/sizes
	int nodeCount = 0;
	this->countNode(this->root, &nodeCount);

	info.startPos = out.device()->pos();

	quint32 fstStart = 0x20;
	quint32 nodeDataSize = nodeCount * 12;
	quint32 stringTableSize = stringTable.length();
	quint32 fstSize = nodeDataSize + stringTableSize;
	quint32 dataOffset = AlignUp(fstStart + fstSize, 0x20);

	qDebug() << "Writing: fstStart" << fstStart << "nodeCount" << nodeCount;
	qDebug() << "nodeDataSize" << nodeDataSize << "stringTableSize" << stringTableSize;
	qDebug() << "fstSize" << fstSize << "dataOffset" << dataOffset;

	// now write the header
	out << (quint32)0x55AA382D;
	out << (quint32)fstStart;
	out << (quint32)fstSize;
	out << (quint32)dataOffset;

	WritePadding(0x10, out);

	// write root node
	info.currentNode = 1; // root node is 1
	info.currentRecursionLevel = 0;
	info.nextDataOffset = dataOffset;

	out << (quint32)(0x01000000 << info.stringTableBuilder.add(""));
	out << (quint32)0;
	out << (quint32)nodeCount;

	this->writeDir(out, this->root, info);

	// write string table
	out.writeRawData(stringTable.constData(), stringTable.length());

	// write data (after padding)
	WritePadding(dataOffset - fstSize - fstStart, out);

	this->writeNodeData(out, this->root);

	// looks like we are finally done
}


void WiiArchiveU8::addNodeToStringTable(WiiFSObject &node, WiiStringTableBuilder &table) {
	table.add(node.name);

	if (node.isDirectory()) {
		WiiDirectory *dir = (WiiDirectory*)&node;

		foreach (WiiFSObject *p, dir->children)
			this->addNodeToStringTable(*p, table);
	}
}


void WiiArchiveU8::countNode(WiiFSObject &node, int *countPtr) {
	(*countPtr)++;

	if (node.isDirectory()) {
		WiiDirectory *dir = (WiiDirectory*)&node;

		foreach (WiiFSObject *p, dir->children)
			this->countNode(*p, countPtr);
	}
}


void WiiArchiveU8::writeDir(QDataStream &out, WiiDirectory &dir, U8WriteInfo &info) {
	foreach (WiiFSObject *p, dir.children) {
		info.currentNode++;

		if (p->isDirectory()) {
			// write directory
			WiiDirectory *thisDir = (WiiDirectory*)p;

			out << (quint32)(0x01000000 | info.stringTableBuilder.add(thisDir->name));
			out << (quint32)info.currentRecursionLevel;

			qint64 lastChildFieldPos = out.device()->pos();
			out << (quint32)0; // placeholder

			info.currentRecursionLevel++;

			this->writeDir(out, *thisDir, info);

			info.currentRecursionLevel--;

			// write lastChild field
			qint64 dirEndPos = out.device()->pos();

			out.device()->seek(lastChildFieldPos);
			out << (quint32)info.currentNode;
			out.device()->seek(dirEndPos);

		} else if (p->isFile()) {
			// write file
			WiiFile *thisFile = (WiiFile*)p;

			out << (quint32)info.stringTableBuilder.add(thisFile->name);
			out << (quint32)info.nextDataOffset;
			out << (quint32)thisFile->data.size();

			info.nextDataOffset = AlignUp(info.nextDataOffset + thisFile->data.size(), 0x20);
		}
	}
}


void WiiArchiveU8::writeNodeData(QDataStream &out, WiiFSObject &node) {
	if (node.isDirectory()) {
		// write all the children's data
		WiiDirectory *thisDir = (WiiDirectory*)&node;

		foreach (WiiFSObject *p, thisDir->children)
			this->writeNodeData(out, *p);

	} else if (node.isFile()) {
		// write this file's data
		WiiFile *thisFile = (WiiFile*)&node;

		int len = thisFile->data.length();
		out.writeRawData(thisFile->data.constData(), len);
		WritePadding(AlignUp(len, 0x20) - len, out);
	}
}
