#include "lspaneeditor.h"

LSPaneEditor::LSPaneEditor(QWidget *parent) :
	QWidget(parent) {

	QGridLayout *layout = new QGridLayout(this);

	m_headingLabel = new QLabel(this);

	m_addChildButton = new QToolButton(this);
	m_addChildButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	m_addChildButton->setPopupMode(QToolButton::InstantPopup);
	m_addChildButton->setText("Add Child Pane");

	m_addChildMenu = new QMenu(m_addChildButton);
	m_addChildMenu->addSeparator();
	m_addChildButton->setMenu(m_addChildMenu);

	m_removeButton = new QToolButton(this);
	m_removeButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
	m_removeButton->setText("Remove This Pane");

	layout->addWidget(m_headingLabel, 0, 0, 1, 1);
	layout->setColumnStretch(0, 1);
	layout->addWidget(m_addChildButton, 0, 1, 1, 1);
	layout->addWidget(m_removeButton, 0, 2, 1, 1);
}
