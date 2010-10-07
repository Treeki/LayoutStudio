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

	foreach (LYTMaterial *material, materials.values())
		delete material;

	this->materials.clear();

	if (this->rootPane != 0)
		delete this->rootPane;
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

	in.skipRawData(4); // unused in newer nw4r::lyt versions - TODO: add support
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

		materials.insert(material->name, material);
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
