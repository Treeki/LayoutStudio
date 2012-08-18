#include "lstexcoordseteditor.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QGridLayout>

LSTexCoordSetEditor::LSTexCoordSetEditor(QWidget *parent) :
	QWidget(parent) {

	m_loadingThings = 1;

	m_coordCount = new QSpinBox(this);
	m_coordCount->setRange(0, 8);
	connect(m_coordCount, SIGNAL(valueChanged(int)), SLOT(handleCoordCountChanged(int)));

	m_chooser = new QComboBox(this);
	connect(m_chooser, SIGNAL(currentIndexChanged(int)), SLOT(handleSetSelected(int)));

	for (int i = 0; i < 8; i++) {
		m_coordEntry[i] = new QDoubleSpinBox(this);
		m_coordEntry[i]->setRange(-10000000.0, 10000000.0);
		connect(m_coordEntry[i], SIGNAL(valueChanged(double)), SLOT(handleCoordChanged(double)));
	}


	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addWidget(new QLabel("Count:", this));
	topLayout->addWidget(m_coordCount);
	topLayout->addSpacing(10);
	topLayout->addWidget(m_chooser, 1);

	// Layout:
	//   0 1 2 3 4
	// 0 <Chooser>
	// 1 x y   x y
	// 2
	// 3 x y   x y

	QGridLayout *layout = new QGridLayout(this);
	layout->addLayout(topLayout, 0, 0, 1, 5);

	layout->addWidget(m_coordEntry[0], 1, 0, 1, 1);
	layout->addWidget(m_coordEntry[1], 1, 1, 1, 1);

	layout->addWidget(m_coordEntry[2], 1, 3, 1, 1);
	layout->addWidget(m_coordEntry[3], 1, 4, 1, 1);

	layout->addWidget(m_coordEntry[4], 3, 0, 1, 1);
	layout->addWidget(m_coordEntry[5], 3, 1, 1, 1);

	layout->addWidget(m_coordEntry[6], 3, 3, 1, 1);
	layout->addWidget(m_coordEntry[7], 3, 4, 1, 1);

	layout->setColumnMinimumWidth(2, 25);
	layout->setRowMinimumHeight(2, 10);

	m_loadingThings--;
}


void LSTexCoordSetEditor::setCoordPtr(QVector<LYTTexCoords> *coords) {
	m_loadingThings++;

	m_targetCoords = coords;

	m_coordCount->setValue(coords->count());

	changeChooserCountTo(coords->count());

	m_chooser->setCurrentIndex(coords->count() ? 0 : -1);
	showCoordSet(coords->count() ? 0 : -1);

	m_loadingThings--;
}

void LSTexCoordSetEditor::changeChooserCountTo(int count) {
	m_loadingThings++;

	int existingCount = m_chooser->count();

	if (existingCount > count) {
		// remove something
		int nowSelected = m_chooser->currentIndex();

		if (nowSelected >= count) {
			// oops, we'll need to select something else
			showCoordSet(count - 1);
			m_chooser->setCurrentIndex(count - 1);
		}

		for (int i = (existingCount - 1); i >= count; i--)
			m_chooser->removeItem(i);

	} else if (count > existingCount) {
		// add something

		for (int i = existingCount; i < count; i++)
			m_chooser->addItem(QString("Set %1").arg(i + 1));
	}

	m_loadingThings--;
}

void LSTexCoordSetEditor::showCoordSet(int index) {
	m_loadingThings++;

	bool doesExist = (index != -1);

	for (int i = 0; i < 8; i++)
		m_coordEntry[i]->setEnabled(doesExist);

	if (doesExist) {
		const LYTTexCoords &set = m_targetCoords->at(index);

		for (int i = 0; i < 4; i++) {
			m_coordEntry[i*2]->setValue(set.coord[i].x());
			m_coordEntry[i*2+1]->setValue(set.coord[i].y());
		}
	}

	m_loadingThings--;
}


void LSTexCoordSetEditor::handleSetSelected(int index) {
	if (!m_loadingThings)
		showCoordSet(index);
}

void LSTexCoordSetEditor::handleCoordCountChanged(int count) {
	if (!m_loadingThings) {
		int oldCount = m_targetCoords->count();
		if (oldCount == count)
			return;

		changeChooserCountTo(count);
		m_targetCoords->resize(count);

		// moving from 0 to something...?
		if (oldCount == 0) {
			m_chooser->setCurrentIndex(0);
			showCoordSet(0);
		}

		emit coordsEdited();
	}
}

void LSTexCoordSetEditor::handleCoordChanged(double value) {
	if (!m_loadingThings) {
		// this code is.. kind of hacky.

		int whatID = -1;
		for (int i = 0; i < 8; i++)
			if (m_coordEntry[i] == sender()) {
				whatID = i;
				break;
			}

		if (whatID >= 0) {
			int coordIdx = m_chooser->currentIndex();
			if (coordIdx == -1)
				return;

			LYTTexCoords &coord = (*m_targetCoords)[coordIdx];

			if ((whatID % 2) == 1)
				coord.coord[whatID / 2].setY(value);
			else
				coord.coord[whatID / 2].setX(value);

			emit coordsEdited();
		}
	}
}
