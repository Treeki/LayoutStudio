#include "lslayoutwindow.h"
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


	QGroupBox *matGroup = new QGroupBox("Materials", this);
	QGroupBox *grpGroup = new QGroupBox("Groups", this);

	sgrid->addWidget(matGroup, 1, 0, 1, 1);
	sgrid->addWidget(grpGroup, 1, 1, 1, 1);
	sgrid->setRowStretch(1, 1);

	m_tabWidget->addTab(w, "Settings");
}


