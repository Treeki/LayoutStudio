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

#include "layout.h"
#include <QtCore/QStack>
#include <QtCore/QVector>


LYTLayout::LYTLayout(LYTPackageBase &package) : rootPane(0), m_package(package) {
	this->clear();
}

LYTLayout::LYTLayout(LYTPackageBase &package, QString name) : rootPane(0), m_package(package) {
	this->loadLayoutFromPackage(name);
}

LYTLayout::~LYTLayout() {
	this->clear();
}


void LYTLayout::clear() {
	this->width = 608.0;
	this->height = 456.0;

	this->m_fontRefs.clear();
	this->m_textureRefs.clear();

	foreach (LYTMaterialContainerEntry materialEntry, materials.list)
		delete materialEntry.second;

	this->materials.clear();

	if (this->rootPane != 0)
		delete this->rootPane;

	foreach (LYTGroup *group, groups)
		delete group;
}


LYTPackageBase &LYTLayout::package() const {
	return m_package;
}


bool LYTLayout::loadLayoutFromPackage(QString name) {
	qDebug() << "Loading layout: " << name;
	qDebug() << "From package: " << m_package.description();

	QStack<LYTPane *> paneStack;
	LYTPane *lastPane, *newPane;

	bool hasGroupContainer = false;
	int groupStackID = 0;

	QByteArray layoutData = m_package.getLayout(name);
	LYTBinaryFile file(layoutData);

	this->clear();

	foreach (LYTBinaryFileSection section, file.sections) {
		// Handle every section in the file

		qDebug("Handling %c%c%c%c", section.magic.str[3], section.magic.str[2], section.magic.str[1], section.magic.str[0]);

		switch (section.magic.value) {
		case 'lyt1':
			this->readLyt1(section);
			break;

		case 'txl1':
			this->readTxl1(section);
			break;

		case 'fnl1':
			this->readFnl1(section);
			break;

		case 'mat1':
			this->readMat1(section);
			break;

		case 'pan1':
		case 'txt1':
		case 'pic1':
		case 'wnd1':
		case 'bnd1':
			newPane = this->createPaneObj(section);
			newPane->dumpToDebug();

			if (rootPane == 0)
				rootPane = newPane;

			if (!paneStack.empty()) {
				newPane->parent = paneStack.last();
				paneStack.last()->children.append(newPane);
			}

			lastPane = newPane;

			break;

		case 'pas1':
			paneStack.push(lastPane);
			break;

		case 'pae1':
			lastPane = paneStack.pop();
			break;

		case 'grp1':
			if (!hasGroupContainer) {
				// nw4r::lyt::Layout::Build actually creates a GroupContainer
				// but we just use a QList so we don't need to do that
				hasGroupContainer = true;
			} else {
				if (groupStackID == 1) {
					// create a group and add it to the list
					QDataStream in(section.data);
					InitDataStream(in);
					groups.append(new LYTGroup());
					groups.last()->readFromDataStream(in, *this->rootPane);
				}
			}

			break;

		case 'grs1':
			groupStackID++;
			break;

		case 'gre1':
			groupStackID--;
			break;
		}
	}

	// todo: usd1

	return true;
}

void LYTLayout::readLyt1(LYTBinaryFileSection &section) {
	// initial info block containing width + height

	QDataStream in(section.data);
	InitDataStream(in);

	in >> (quint8&)flags; // seems to be unused in newer nw4r::lyt versions (but still in file)?
	in.skipRawData(3); // padding
	in >> (float&)width;
	in >> (float&)height;

	qDebug() << "Read lyt1 block: layout size:" << width << "x" << height;
}

void LYTLayout::readTxl1(LYTBinaryFileSection &section) {
	// list of texture references for this layout

	QDataStream in(section.data);
	InitDataStream(in);
	m_textureRefs = ReadStringList(in);
}

void LYTLayout::readFnl1(LYTBinaryFileSection &section) {
	// list of font references for this layout

	QDataStream in(section.data);
	InitDataStream(in);
	m_fontRefs = ReadStringList(in);
}

void LYTLayout::readMat1(LYTBinaryFileSection &section) {
	// the material section for the layout
	// not fun.

	QDataStream in(section.data);
	InitDataStream(in);

	quint16 count;
	in >> (quint16&)count;
	in.skipRawData(2); // padding

	// create a list of every offset
	// these offsets are from the start of the section, not from
	// the first offset entry like StringLists are
	QVector<quint32> materialOffsets(count);

	for (int i = 0; i < count; i++) {
		quint32 offset;
		in >> (quint32&)offset;
		materialOffsets[i] = offset;
	}

	// now read each material
	for (int i = 0; i < count; i++) {
		// subtract 8 from the offset because section.data doesn't contain
		// the nw4r::ut::BinaryBlockHeader whereas these offsets do count it
		in.device()->seek(materialOffsets[i] - 8);

		LYTMaterial *material = new LYTMaterial(*this);
		material->readFromDataStream(in);
		material->dumpToDebug();

		materials.addMaterial(material->name, material);
	}
}

LYTPane *LYTLayout::createPaneObj(LYTBinaryFileSection &section) {
	LYTPane *pane;

	if (section.magic.value == 'pan1')
		pane = new LYTPane(*this);
	else if (section.magic.value == 'txt1')
		pane = new LYTTextBox(*this);
	else if (section.magic.value == 'pic1')
		pane = new LYTPicture(*this);
	else if (section.magic.value == 'wnd1')
		pane = new LYTWindow(*this);
	else if (section.magic.value == 'bnd1')
		pane = new LYTBounding(*this);

	QDataStream in(section.data);
	InitDataStream(in);

	pane->readFromDataStream(in);

	return pane;
}



QByteArray LYTLayout::pack() {
	qDebug() << "Writing layout";

	// before we start writing the file, find out a bunch of info
	qDebug() << "Compiling texture ref list";
	m_textureRefs = this->generateTextureRefs();

	qDebug() << "Compiling font ref list";
	m_fontRefs = this->generateFontRefs();

	// now start writing the file
	Magic magic('RLYT');
	Version version(0x000A);

	LYTBinaryFile file(magic, version);

	QDataStream *stream;

	// write lyt1
	file.sections.append(LYTBinaryFileSection(Magic('lyt1')));
	stream = file.sections.last().createWriteStream();
	(*stream) << (quint8)flags;
	WritePadding(3, *stream);
	(*stream) << (float)this->width;
	(*stream) << (float)this->height;
	delete stream;

	// write txl1
	file.sections.append(LYTBinaryFileSection(Magic('txl1')));
	stream = file.sections.last().createWriteStream();
	WriteStringList(*stream, this->m_textureRefs);
	delete stream;

	// write fnl1
	file.sections.append(LYTBinaryFileSection(Magic('fnl1')));
	stream = file.sections.last().createWriteStream();
	WriteStringList(*stream, this->m_fontRefs);
	delete stream;

	// write mat1
	file.sections.append(LYTBinaryFileSection(Magic('mat1')));
	this->writeMat1(file.sections.last());

	// next up: write the pane tree
	this->writePane(file, this->rootPane);

	// now write groups
	this->writeGroups(file);

	// and we're done!
	return file.pack();
}



QStringList LYTLayout::generateTextureRefs() const {
	QStringList out;

	foreach (LYTMaterialContainerEntry matEntry, materials.list) {
		foreach (LYTTexMap texMap, matEntry.second->texMaps) {
			if (!out.contains(texMap.textureName))
				out.append(texMap.textureName);
		}
	}

	out.sort();

	return out;
}


QStringList LYTLayout::generateFontRefs() const {
	QStringList out;

	this->rootPane->addFontRefsToList(out);

	out.sort();

	return out;
}



void LYTLayout::writeMat1(LYTBinaryFileSection &section) const {
	QDataStream *stream = section.createWriteStream();

	(*stream) << (quint16)this->materials.count();
	WritePadding(2, *stream);

	// now, assign space for the mat1 offsets, we'll set them later
	qint64 matOffsetPos = stream->device()->pos();

	WritePadding(4 * this->materials.count(), *stream);
	QVector<quint32> materialOffsets(this->materials.count());

	// write them
	for (int i = 0; i < this->materials.count(); i++) {
		// add 8 to the offset to account for nw4r::ut::BinaryBlockHeader
		materialOffsets[i] = stream->device()->pos() + 8;
		this->materials.getMaterialByIndex(i)->writeToDataStream(*stream);

		// pad it if needed
		qint64 checkPadding = stream->device()->pos();
		int padding = AlignUp(checkPadding, 4) - checkPadding;
		WritePadding(padding, *stream);
	}

	// and now write the offsets
	qint64 endMatPos = stream->device()->pos();
	stream->device()->seek(matOffsetPos);

	for (int i = 0; i < materialOffsets.count(); i++) {
		(*stream) << (quint32)materialOffsets[i];
	}

	stream->device()->seek(endMatPos);

	delete stream;
}


void LYTLayout::writePane(LYTBinaryFile &file, LYTPane *pane) const {
	file.sections.append(LYTBinaryFileSection(pane->magic()));

	QDataStream *stream = file.sections.last().createWriteStream();
	pane->writeToDataStream(*stream);
	delete stream;


	if (!pane->children.isEmpty()) {
		file.sections.append(LYTBinaryFileSection(Magic('pas1')));

		foreach (LYTPane *p, pane->children) {
			this->writePane(file, p);
		}

		file.sections.append(LYTBinaryFileSection(Magic('pae1')));
	}
}


void LYTLayout::writeGroups(LYTBinaryFile &file) const {
	QDataStream *stream;

	// first, write a dummy group for the root
	file.sections.append(LYTBinaryFileSection(Magic('grp1')));
	stream = file.sections.last().createWriteStream();

	WriteFixedLengthASCII(*stream, "RootGroup", 0x10);
	WritePadding(4, *stream);

	delete stream;

	// now write the children
	if (!this->groups.isEmpty()) {
		file.sections.append(LYTBinaryFileSection(Magic('grs1')));

		foreach (LYTGroup *p, this->groups) {
			file.sections.append(LYTBinaryFileSection(Magic('grp1')));

			stream = file.sections.last().createWriteStream();
			p->writeToDataStream(*stream);
			delete stream;
		}

		file.sections.append(LYTBinaryFileSection('gre1'));
	}
}
