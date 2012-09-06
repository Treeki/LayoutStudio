#include "lsmaterialeditor.h"
#include "lsmaterialeditors.h"
#include <QGroupBox>

LSMaterialEditor::LSMaterialEditor(QWidget *parent) :
	QWidget(parent) {

	QGridLayout *grid = new QGridLayout(this);
	int gridRow = 0;

	m_nameEntry = new QLineEdit(this);

	grid->addWidget(new QLabel("Name:", this), gridRow, 0, 1, 1);
	grid->addWidget(m_nameEntry, gridRow, 1, 1, 1);
	gridRow++;

	// TEV colour box
	QGroupBox *tevCBox = new QGroupBox("TEV Colours", this);
	QGridLayout *tevCLayout = new QGridLayout(tevCBox);

	tevCLayout->addWidget(new QLabel("Initial Registers:", tevCBox), 0, 0, 1, 4);
	tevCLayout->addWidget(new QLabel("Constant:", tevCBox), 2, 0, 1, 4);

	for (int i = 0; i < 7; i++) {
		m_colourPickers[i] = new LSColorPicker(tevCBox);
		bool isKonst = (i >= 3);
		tevCLayout->addWidget(m_colourPickers[i], isKonst?3:1, isKonst?(i-3):i, 1, 1);
	}

	grid->addWidget(tevCBox, gridRow, 0, 1, 2);
	gridRow++;

	// TEV stages
	QGroupBox *stageBox = new QGroupBox("TEV Stages", this);
	QVBoxLayout *stageLayout = new QVBoxLayout(stageBox);

	m_tevStageSetEditor = new LSSetEditor<LYTTevStage, LSTevStageEditor>(16, stageBox);
	stageLayout->addWidget(m_tevStageSetEditor);

	grid->addWidget(stageBox, gridRow, 0, 1, 2);
	gridRow++;

	// finish it all up
	grid->setRowStretch(gridRow, 1);


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

	m_tevStageSetEditor->setData(&mat->tevStages);

	m_currentlyLoadingMaterial = false;
}


void LSMaterialEditor::handleNameChanged(QString value) {

}

void LSMaterialEditor::handleSaveChangedName() {

}

void LSMaterialEditor::handleColourPicked(QColor value) {

}
