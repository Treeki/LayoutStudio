#include "lspaneeditor.h"
#include <QGroupBox>

LSPaneEditor::LSPaneEditor(QWidget *parent) :
	QWidget(parent) {

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

	m_tabs = new QTabWidget(this);

	QGridLayout *layout = new QGridLayout(this);

	layout->addWidget(m_headingLabel, 0, 0, 1, 1);
	layout->setColumnStretch(0, 1);
	layout->addWidget(m_addChildButton, 0, 1, 1, 1);
	layout->addWidget(m_removeButton, 0, 2, 1, 1);

	layout->addWidget(m_tabs, 1, 0, 1, 3);


	createPaneTab();
}

void LSPaneEditor::createPaneTab() {
	m_paneTab = new QWidget(this);
	m_tabs->addTab(m_paneTab, "Pane");


	QGridLayout *gLayout;

	// Properties Box
	QGroupBox *propBox = new QGroupBox("Basic Properties", m_paneTab);
	gLayout = new QGridLayout(propBox);

	m_nameEntry = new QLineEdit(propBox);
	m_nameEntry->setMaxLength(16);

	m_userDataEntry = new QLineEdit(propBox);
	m_userDataEntry->setMaxLength(8);

	m_alpha = new QSpinBox(propBox);
	m_alpha->setRange(0, 255);

	m_influencedAlpha = new QCheckBox("Influenced Alpha", propBox);

	// Column 0
	gLayout->addWidget(new QLabel("Name:", propBox), 0, 0, 1, 1);
	gLayout->addWidget(m_nameEntry, 0, 1, 1, 1);
	gLayout->addWidget(new QLabel("User Data:", propBox), 1, 0, 1, 1);
	gLayout->addWidget(m_userDataEntry, 1, 1, 1, 1);

	// Colunm 1
	gLayout->addWidget(new QLabel("Alpha:", propBox), 0, 2, 1, 1);
	gLayout->addWidget(m_alpha, 0, 3, 1, 1);
	gLayout->addWidget(m_influencedAlpha, 1, 3, 1, 1);


	// Geometry Box
	QGroupBox *geoBox = new QGroupBox("Geometry", m_paneTab);
	gLayout = new QGridLayout(geoBox);

	m_width = new QDoubleSpinBox(geoBox);
	m_width->setRange(0.0, 100000.0);
	m_height = new QDoubleSpinBox(geoBox);
	m_height->setRange(0.0, 100000.0);

	m_horzOrigin = new QComboBox(geoBox);
	m_horzOrigin->addItem("Left");
	m_horzOrigin->addItem("Center");
	m_horzOrigin->addItem("Right");

	m_vertOrigin = new QComboBox(geoBox);
	m_vertOrigin->addItem("Top");
	m_vertOrigin->addItem("Center");
	m_vertOrigin->addItem("Bottom");

	m_widescreen = new QCheckBox("Widescreen", this);
	m_visible = new QCheckBox("Visible", this);

	// Row 0
	gLayout->addWidget(new QLabel("Width:", geoBox), 0, 0, 1, 1);
	gLayout->addWidget(m_width, 0, 1, 1, 1);
	gLayout->addWidget(new QLabel("Height:", geoBox), 0, 2, 1, 1);
	gLayout->addWidget(m_height, 0, 3, 1, 1);

	// Row 1
	gLayout->addWidget(new QLabel("Horizontal Origin:", geoBox), 1, 0, 1, 1);
	gLayout->addWidget(m_horzOrigin, 1, 1, 1, 1);
	gLayout->addWidget(new QLabel("Vertical Origin:", geoBox), 1, 2, 1, 1);
	gLayout->addWidget(m_vertOrigin, 1, 3, 1, 1);

	// Row 2
	gLayout->addWidget(m_widescreen, 2, 0, 1, 2);
	gLayout->addWidget(m_visible, 2, 2, 1, 2);


	// Positioning Box
	QGroupBox *posBox = new QGroupBox("Positioning", m_paneTab);
	gLayout = new QGridLayout(posBox);

	m_transX = new QDoubleSpinBox(posBox);
	m_transY = new QDoubleSpinBox(posBox);
	m_transZ = new QDoubleSpinBox(posBox);
	m_rotX = new QDoubleSpinBox(posBox);
	m_rotY = new QDoubleSpinBox(posBox);
	m_rotZ = new QDoubleSpinBox(posBox);
	m_scaleX = new QDoubleSpinBox(posBox);
	m_scaleY = new QDoubleSpinBox(posBox);

	gLayout->addWidget(new QLabel("Translation:", posBox), 0, 0, 1, 1);
	gLayout->addWidget(m_transX, 0, 1, 1, 1);
	gLayout->addWidget(m_transY, 0, 2, 1, 1);
	gLayout->addWidget(m_transZ, 0, 3, 1, 1);
	gLayout->addWidget(new QLabel("Rotation:", posBox), 1, 0, 1, 1);
	gLayout->addWidget(m_rotX, 1, 1, 1, 1);
	gLayout->addWidget(m_rotY, 1, 2, 1, 1);
	gLayout->addWidget(m_rotZ, 1, 3, 1, 1);
	gLayout->addWidget(new QLabel("Scale:", posBox), 2, 0, 1, 1);
	gLayout->addWidget(m_scaleX, 2, 1, 1, 1);
	gLayout->addWidget(m_scaleY, 2, 1, 1, 1);


	// put it all together into one

	QVBoxLayout *layout = new QVBoxLayout(m_paneTab);
	layout->addWidget(propBox);
	layout->addWidget(geoBox);
	layout->addWidget(posBox);
	layout->addStretch(1);
}
