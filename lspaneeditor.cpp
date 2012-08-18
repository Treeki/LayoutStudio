#include "lspaneeditor.h"
#include <QGroupBox>

#include "lyt/bounding.h"
#include "lyt/picture.h"
#include "lyt/textbox.h"
#include "lyt/window.h"

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
	m_tabs->setVisible(false);

	QGridLayout *layout = new QGridLayout(this);

	layout->addWidget(m_headingLabel, 0, 0, 1, 1);
	layout->setColumnStretch(0, 1);
	layout->addWidget(m_addChildButton, 0, 1, 1, 1);
	layout->addWidget(m_removeButton, 0, 2, 1, 1);

	layout->addWidget(m_tabs, 1, 0, 1, 3);


	createPaneTab();

	m_currentlyLoadingPane = false;
	m_pane = 0;
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
	connect(m_nameEntry, SIGNAL(textChanged(QString)), SLOT(handleNameChanged(QString)));

	m_userDataEntry = new QLineEdit(propBox);
	m_userDataEntry->setMaxLength(8);
	connect(m_userDataEntry, SIGNAL(textChanged(QString)), SLOT(handleUserDataChanged(QString)));

	m_alpha = new QSpinBox(propBox);
	m_alpha->setRange(0, 255);
	connect(m_alpha, SIGNAL(valueChanged(int)), SLOT(handleAlphaChanged(int)));

	m_influencedAlpha = new QCheckBox("Influenced Alpha", propBox);
	connect(m_influencedAlpha, SIGNAL(toggled(bool)), SLOT(handleInfluencedAlphaChanged(bool)));

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
	connect(m_width, SIGNAL(valueChanged(double)), SLOT(handleWidthChanged(double)));
	m_height = new QDoubleSpinBox(geoBox);
	m_height->setRange(0.0, 100000.0);
	connect(m_height, SIGNAL(valueChanged(double)), SLOT(handleHeightChanged(double)));

	m_horzOrigin = new QComboBox(geoBox);
	m_horzOrigin->addItem("Left");
	m_horzOrigin->addItem("Center");
	m_horzOrigin->addItem("Right");
	connect(m_horzOrigin, SIGNAL(currentIndexChanged(int)), SLOT(handleHorzOriginChanged(int)));

	m_vertOrigin = new QComboBox(geoBox);
	m_vertOrigin->addItem("Top");
	m_vertOrigin->addItem("Center");
	m_vertOrigin->addItem("Bottom");
	connect(m_vertOrigin, SIGNAL(currentIndexChanged(int)), SLOT(handleVertOriginChanged(int)));

	m_widescreen = new QCheckBox("Widescreen", this);
	connect(m_widescreen, SIGNAL(toggled(bool)), SLOT(handleWidescreenChanged(bool)));
	m_visible = new QCheckBox("Visible", this);
	connect(m_visible, SIGNAL(toggled(bool)), SLOT(handleVisibleChanged(bool)));

	// Column 0
	gLayout->addWidget(new QLabel("Width:", geoBox), 0, 0, 1, 1);
	gLayout->addWidget(m_width, 0, 1, 1, 1);
	gLayout->addWidget(new QLabel("Height:", geoBox), 1, 0, 1, 1);
	gLayout->addWidget(m_height, 1, 1, 1, 1);

	// Column 1
	gLayout->addWidget(new QLabel("Horizontal Origin:", geoBox), 0, 2, 1, 1);
	gLayout->addWidget(m_horzOrigin, 0, 3, 1, 1);
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

	connect(m_transX, SIGNAL(valueChanged(double)), SLOT(handleTransXChanged(double)));
	connect(m_transY, SIGNAL(valueChanged(double)), SLOT(handleTransYChanged(double)));
	connect(m_transZ, SIGNAL(valueChanged(double)), SLOT(handleTransZChanged(double)));
	connect(m_rotX, SIGNAL(valueChanged(double)), SLOT(handleRotXChanged(double)));
	connect(m_rotY, SIGNAL(valueChanged(double)), SLOT(handleRotYChanged(double)));
	connect(m_rotZ, SIGNAL(valueChanged(double)), SLOT(handleRotZChanged(double)));
	connect(m_scaleX, SIGNAL(valueChanged(double)), SLOT(handleScaleXChanged(double)));
	connect(m_scaleY, SIGNAL(valueChanged(double)), SLOT(handleScaleYChanged(double)));

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
	gLayout->addWidget(m_scaleY, 2, 2, 1, 1);


	// put it all together into one

	QVBoxLayout *layout = new QVBoxLayout(m_paneTab);
	layout->addWidget(propBox);
	layout->addWidget(geoBox);
	layout->addWidget(posBox);
	layout->addStretch(1);
}


void LSPaneEditor::setPane(LYTPane *pane) {
	m_currentlyLoadingPane = true;
	m_pane = pane;

	m_tabs->setVisible(true);


	m_nameEntry->setText(pane->name);
	m_userDataEntry->setText(pane->userdata);

	m_alpha->setValue(pane->alpha);
	m_influencedAlpha->setChecked(pane->influencedAlpha);


	m_width->setValue(pane->width);
	m_height->setValue(pane->height);

	m_horzOrigin->setCurrentIndex((int)pane->horzOrigin);
	m_vertOrigin->setCurrentIndex((int)pane->vertOrigin);

	m_widescreen->setChecked(pane->isWidescreen);
	m_visible->setChecked(pane->visible);


	m_transX->setValue(pane->xTrans);
	m_transY->setValue(pane->yTrans);
	m_transZ->setValue(pane->zTrans);

	m_rotX->setValue(pane->xRot);
	m_rotY->setValue(pane->yRot);
	m_rotZ->setValue(pane->zRot);

	m_scaleX->setValue(pane->xScale);
	m_scaleY->setValue(pane->yScale);


	m_currentlyLoadingPane = false;
}


void LSPaneEditor::handleNameChanged(QString value) {
	if (!m_currentlyLoadingPane) {
		m_pane->name = value;
		emit mustRedrawLayout();
	}
}

void LSPaneEditor::handleUserDataChanged(QString value) {
	if (!m_currentlyLoadingPane) {
		m_pane->userdata = value;
		emit mustRedrawLayout();
	}
}

void LSPaneEditor::handleAlphaChanged(int value) {
	if (!m_currentlyLoadingPane) {
		m_pane->alpha = (quint8)value;
		emit mustRedrawLayout();
	}
}

void LSPaneEditor::handleInfluencedAlphaChanged(bool value) {
	if (!m_currentlyLoadingPane) {
		m_pane->influencedAlpha = value;
		emit mustRedrawLayout();
	}
}

void LSPaneEditor::handleWidthChanged(double value) {
	if (!m_currentlyLoadingPane) {
		m_pane->width = value;
		emit mustRedrawLayout();
	}
}

void LSPaneEditor::handleHeightChanged(double value) {
	if (!m_currentlyLoadingPane) {
		m_pane->width = value;
		emit mustRedrawLayout();
	}
}

void LSPaneEditor::handleHorzOriginChanged(int value) {
	if (!m_currentlyLoadingPane) {
		m_pane->horzOrigin = (LYTPane::OriginType)value;
		emit mustRedrawLayout();
	}
}

void LSPaneEditor::handleVertOriginChanged(int value) {
	if (!m_currentlyLoadingPane) {
		m_pane->vertOrigin = (LYTPane::OriginType)value;
		emit mustRedrawLayout();
	}
}

void LSPaneEditor::handleWidescreenChanged(bool value) {
	if (!m_currentlyLoadingPane) {
		m_pane->isWidescreen = value;
		emit mustRedrawLayout();
	}
}

void LSPaneEditor::handleVisibleChanged(bool value) {
	if (!m_currentlyLoadingPane) {
		m_pane->visible = value;
		emit mustRedrawLayout();
	}
}

void LSPaneEditor::handleTransXChanged(double value) {
	if (!m_currentlyLoadingPane) {
		m_pane->xTrans = value;
		emit mustRedrawLayout();
	}
}

void LSPaneEditor::handleTransYChanged(double value) {
	if (!m_currentlyLoadingPane) {
		m_pane->yTrans = value;
		emit mustRedrawLayout();
	}
}

void LSPaneEditor::handleTransZChanged(double value) {
	if (!m_currentlyLoadingPane) {
		m_pane->zTrans = value;
		emit mustRedrawLayout();
	}
}

void LSPaneEditor::handleRotXChanged(double value) {
	if (!m_currentlyLoadingPane) {
		m_pane->xRot = value;
		emit mustRedrawLayout();
	}
}

void LSPaneEditor::handleRotYChanged(double value) {
	if (!m_currentlyLoadingPane) {
		m_pane->yRot = value;
		emit mustRedrawLayout();
	}
}

void LSPaneEditor::handleRotZChanged(double value) {
	if (!m_currentlyLoadingPane) {
		m_pane->zRot = value;
		emit mustRedrawLayout();
	}
}

void LSPaneEditor::handleScaleXChanged(double value) {
	if (!m_currentlyLoadingPane) {
		m_pane->xScale = value;
		emit mustRedrawLayout();
	}
}

void LSPaneEditor::handleScaleYChanged(double value) {
	if (!m_currentlyLoadingPane) {
		m_pane->yScale = value;
		emit mustRedrawLayout();
	}
}

