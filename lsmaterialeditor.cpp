#include "lsmaterialeditor.h"
#include <QGroupBox>

LSMaterialEditor::LSMaterialEditor(QWidget *parent) :
	QWidget(parent) {

	QGridLayout *grid = new QGridLayout(this);

	m_nameEntry = new QLineEdit(this);

	grid->addWidget(new QLabel("Name:", this), 0, 0, 1, 1);
	grid->addWidget(m_nameEntry, 0, 1, 1, 1);

	// TEV colour box
	QGroupBox *tevCBox = new QGroupBox("TEV Colours", this);
	QGridLayout *tevCLayout = new QGridLayout(tevCBox);

	tevCLayout->addWidget(new QLabel("Registers:", tevCBox), 0, 0, 1, 4);
	tevCLayout->addWidget(new QLabel("Constant:", tevCBox), 2, 0, 1, 4);

	for (int i = 0; i < 7; i++) {
		m_colourPickers[i] = new LSColorPicker(tevCBox);
		bool isKonst = (i >= 3);
		tevCLayout->addWidget(m_colourPickers[i], isKonst?3:1, isKonst?(i-3):i, 1, 1);
	}

	grid->addWidget(tevCBox, 1, 0, 1, 2);


	grid->setRowStretch(2, 1);


	m_currentlyLoadingMaterial = false;
	m_material = 0;
}

void LSMaterialEditor::setMaterial(LYTMaterial *mat) {
	m_currentlyLoadingMaterial = true;
	m_material = mat;

	m_nameEntry->setText(mat->name);

	for (int i = 0; i < 3; i++)
		m_colourPickers[i]->setColor(mat->colours[i]);
	for (int i = 0; i < 4; i++)
		m_colourPickers[i+3]->setColor(mat->tevKColour[i]);

	m_currentlyLoadingMaterial = false;
}


void LSMaterialEditor::handleNameChanged(QString value) {

}

void LSMaterialEditor::handleSaveChangedName() {

}

void LSMaterialEditor::handleColourPicked(QColor value) {

}
