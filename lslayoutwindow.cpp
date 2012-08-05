#include "lslayoutwindow.h"
#include "lsscenemodel.h"
#include <QGridLayout>
#include <QFormLayout>
#include <QGroupBox>

LSLayoutWindow::LSLayoutWindow(LYTPackageBase *pkg, const QString &layoutName, QWidget *parent) :
	QMainWindow(parent) {

	m_layoutName = layoutName;
	m_package = pkg;


	m_tabWidget = new QTabWidget(this);
	setCentralWidget(m_tabWidget);

	QWidget *w;

	// prepare the Settings tab
	w = new QWidget(this);
	QGridLayout *sgrid = new QGridLayout(w);

	QGroupBox *sizeGroup = new QGroupBox("Layout Size", this);
	sgrid->addWidget(sizeGroup, 0, 0, 1, 2);

	QFormLayout *sizeForm = new QFormLayout(sizeGroup);

	m_widthBox = new QDoubleSpinBox(this);
	m_widthBox->setRange(0.0, 1000.0);
	m_heightBox = new QDoubleSpinBox(this);
	m_heightBox->setRange(0.0, 1000.0);

	sizeForm->addRow("Width", m_widthBox);
	sizeForm->addRow("Height", m_heightBox);

	connect(m_widthBox, SIGNAL(valueChanged(double)), SLOT(handleWidthChanged(double)));
	connect(m_heightBox, SIGNAL(valueChanged(double)), SLOT(handleHeightChanged(double)));


	QGroupBox *matGroup = new QGroupBox("Materials", this);
	QGroupBox *grpGroup = new QGroupBox("Groups", this);

	sgrid->addWidget(matGroup, 1, 0, 1, 1);
	sgrid->addWidget(grpGroup, 1, 1, 1, 1);
	sgrid->setRowStretch(1, 1);

	m_tabWidget->addTab(w, "Layout");

	// prepare the Scene Graph tab
	w = new QWidget(this);
	QGridLayout *ggrid = new QGridLayout(w);

	m_searchBox = new QLineEdit(this);
	m_searchBox->setPlaceholderText("Search panes...");

	m_clearSearchButton = new QPushButton("Clear", this);

	m_sceneGraph = new QTreeView(this);
	m_sceneGraph->setHeaderHidden(true);
	m_sceneSearchList = new QListView(this);
	m_sceneListSwitcher = new QStackedLayout;
	m_sceneListSwitcher->addWidget(m_sceneGraph);
	m_sceneListSwitcher->addWidget(m_sceneSearchList);

	ggrid->addWidget(m_searchBox, 0, 0, 1, 1);
	ggrid->addWidget(m_clearSearchButton, 0, 1, 1, 1);
	ggrid->addLayout(m_sceneListSwitcher, 1, 0, 1, 2);

	m_tabWidget->addTab(w, "Scene Graph");


	// get the resource
	m_layout = new LYTLayout(*m_package, m_layoutName);

	m_widthBox->setValue(m_layout->width);
	m_heightBox->setValue(m_layout->height);

	m_sceneGraph->setModel(new LSSceneModel(m_layout, this));
	m_sceneGraph->expandAll();

	setWindowTitle(m_layoutName);
}

LSLayoutWindow::~LSLayoutWindow() {
	delete m_layout;
}


void LSLayoutWindow::handleWidthChanged(double v) {
	m_layout->width = v;
}

void LSLayoutWindow::handleHeightChanged(double v) {
	m_layout->height = v;
}

