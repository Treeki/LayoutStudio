#include "lsseteditor.h"
#include <QGridLayout>
#include <QLabel>

_LSSetEditorBase::_LSSetEditorBase(int maxEntries, QWidget *parent) :
	QWidget(parent) {

	m_maxEntries = maxEntries;
	m_loadingThings = 0;
}


void _LSSetEditorBase::handleEntrySelected(int index) {
	if (!m_loadingThings)
		showEntry(index);
}


void _LSSetEditorBase::handleEntryCountChanged(int count) {
	if (!m_loadingThings) {
		int oldCount = m_chooser->count();
		if (oldCount == count)
			return;

		changeEntryCountTo(count);
		resizeDataListTo(count);

		// moving from 0 to something...?
		if (oldCount == 0) {
			m_chooser->setCurrentIndex(0);
			showEntry(0);
		}

		emit dataEdited();
	}
}


void _LSSetEditorBase::setup(QWidget *eWidget) {
	m_loadingThings++;

	m_entryCount = new QSpinBox(this);
	m_entryCount->setRange(0, m_maxEntries);
	connect(m_entryCount, SIGNAL(valueChanged(int)), SLOT(handleEntryCountChanged(int)));

	m_chooser = new QComboBox(this);
	connect(m_chooser, SIGNAL(currentIndexChanged(int)), SLOT(handleEntrySelected(int)));

	QGridLayout *layout = new QGridLayout(this);

	layout->addWidget(new QLabel("Count:", this), 0, 0, 1, 1);
	layout->addWidget(m_entryCount, 0, 1, 1, 1);
	layout->setColumnMinimumWidth(2, 10);
	layout->addWidget(m_chooser, 0, 3, 1, 1);
	layout->setColumnStretch(3, 1);

	layout->addWidget(eWidget, 1, 0, 1, 4);

	m_loadingThings--;
}
