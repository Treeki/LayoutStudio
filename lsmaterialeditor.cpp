#include "lsmaterialeditor.h"

LSMaterialEditor::LSMaterialEditor(QWidget *parent) :
	QWidget(parent) {

	QGridLayout *grid = new QGridLayout(this);

	m_nameEntry = new QLineEdit(this);

	grid->addWidget(new QLabel("Name:", this), 0, 0, 1, 1);
	grid->addWidget(m_nameEntry, 0, 1, 1, 1);

	m_currentlyLoadingMaterial = false;
	m_material = 0;
}

void LSMaterialEditor::setMaterial(LYTMaterial *mat) {
	m_currentlyLoadingMaterial = true;
	m_material = mat;

	m_currentlyLoadingMaterial = false;
}


void LSMaterialEditor::handleNameChanged(QString value) {

}

void LSMaterialEditor::handleSaveChangedName() {

}
