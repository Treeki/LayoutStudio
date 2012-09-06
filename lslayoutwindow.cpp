#include "lslayoutwindow.h"
#include "lsscenemodel.h"
#include "lsmaterialmodel.h"
#include "lspaneeditor.h"
#include "lsmaterialeditor.h"
#include "layoutgl/widget.h"
#include <QGridLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QScrollArea>

LSLayoutWindow::LSLayoutWindow(LYTPackageBase *pkg, const QString &layoutName, QWidget *parent) :
	QMainWindow(parent) {

	m_layoutName = layoutName;
	m_package = pkg;
	m_loadingSettings = true;

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


	QGroupBox *grpGroup = new QGroupBox("Groups", this);

	sgrid->addWidget(grpGroup, 1, 0, 1, 2);
	sgrid->setRowStretch(1, 1);

	m_tabWidget->addTab(w, "Layout");

	// prepare the Scene Graph tab
	m_sceneSplitter = new QSplitter(this);

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

	QWidget *switcherWidget = new QWidget(this);
	m_paneEditorSwitcher = new QStackedLayout(switcherWidget);

	m_paneEditor = new LSPaneEditor(switcherWidget);

	m_paneEditorSwitcher->addWidget(new QLabel("Choose a pane to edit from the left.", switcherWidget));
	m_paneEditorSwitcher->addWidget(m_paneEditor);

	m_sceneSplitter->addWidget(w);
	m_sceneSplitter->addWidget(switcherWidget);
	m_sceneSplitter->setCollapsible(1, false);

	m_tabWidget->addTab(m_sceneSplitter, "Scene Graph");


	// prepare the materials tab
	m_materialSplitter = new QSplitter(this);

	w = new QWidget(this);
	QGridLayout *mgrid = new QGridLayout(w);

	m_addMaterialButton = new QPushButton("Add", w);
	m_removeMaterialButton = new QPushButton("Remove", w);
	m_materialList = new QListView(w);

	mgrid->addWidget(m_materialList, 0, 0, 1, 2);
	mgrid->addWidget(m_addMaterialButton, 1, 0, 1, 1);
	mgrid->addWidget(m_removeMaterialButton, 1, 1, 1, 1);

	QScrollArea *matScroller = new QScrollArea(m_materialSplitter);
	m_materialEditor = new LSMaterialEditor(matScroller);

	m_materialSplitter->addWidget(w);
	m_materialSplitter->addWidget(matScroller);
	m_materialSplitter->setCollapsible(1, false);

	matScroller->setWidget(m_materialEditor);

	m_tabWidget->addTab(m_materialSplitter, "Materials");


	// get the resource
	m_layout = new LYTLayout(*m_package, m_layoutName);

	m_widthBox->setValue(m_layout->width);
	m_heightBox->setValue(m_layout->height);

	LSSceneModel *scnModel = new LSSceneModel(m_layout, true, this);

	m_sceneGraph->setSelectionMode(QAbstractItemView::ExtendedSelection);
	m_sceneGraph->setDragEnabled(true);
	m_sceneGraph->setAcceptDrops(true);
	m_sceneGraph->setDropIndicatorShown(true);
	m_sceneGraph->setModel(scnModel);
	m_sceneGraph->expandAll();
	connect(m_sceneGraph->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(selectedPaneChanged(QModelIndex,QModelIndex)));

	LSMaterialModel *matModel = new LSMaterialModel(&m_layout->materials, this);

	m_materialList->setSelectionMode(QAbstractItemView::SingleSelection);
	m_materialList->setModel(matModel);
	connect(m_materialList->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(selectedMaterialChanged(QModelIndex,QModelIndex)));

	setWindowTitle(m_layoutName);


	// finally make the widget
	m_renderer = new LGLWidget();
	m_renderer->setLayout(m_layout);
	m_renderer->setWindowTitle(QString("Preview: %1").arg(m_layoutName));
	m_renderer->setWindowFlags(
				Qt::CustomizeWindowHint |
				Qt::WindowTitleHint |
				Qt::WindowMinimizeButtonHint);
	m_renderer->show();

	connect(m_paneEditor, SIGNAL(mustRedrawLayout()), m_renderer, SLOT(updateGL()));
	connect(scnModel, SIGNAL(paneVisibilityChanged()), m_renderer, SLOT(updateGL()));

	// clean up here
	setAttribute(Qt::WA_DeleteOnClose);

	m_loadingSettings = false;
}

LSLayoutWindow::~LSLayoutWindow() {
	m_renderer->close();
	delete m_renderer;
	delete m_layout;
}


void LSLayoutWindow::selectedPaneChanged(const QModelIndex &current, const QModelIndex &previous) {
	(void)previous;
	LYTPane *pane = (LYTPane*)current.internalPointer();

	m_paneEditor->setPane(pane);
	m_paneEditorSwitcher->setCurrentIndex(1);
}

void LSLayoutWindow::selectedMaterialChanged(const QModelIndex &current, const QModelIndex &previous) {
	(void)previous;
	LYTMaterial *mat = m_layout->materials.getMaterialByIndex(current.row());

	m_materialEditor->setMaterial(mat);
}


void LSLayoutWindow::handleWidthChanged(double v) {
	m_layout->width = v;
	if (!m_loadingSettings)
		m_renderer->updateGL();
}

void LSLayoutWindow::handleHeightChanged(double v) {
	m_layout->height = v;
	if (!m_loadingSettings)
		m_renderer->updateGL();
}

