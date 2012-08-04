/*******************************************************************************
  This file is part of LayoutStudio (http://github.com/Treeki/LayoutStudio)
  Copyright (c) 2010 Treeki (treeki@gmail.com)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, version 2.0.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License 2.0 for more details.

  You should have received a copy of the GNU General Public License 2.0
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/

#include "lsmainwindow.h"
#include <QAction>
#include <QMenuBar>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include "lyt/archivepackage.h"
#include "lslayoutwindow.h"

LSMainWindow::LSMainWindow(QWidget *parent) : QMainWindow(parent) {
	m_package = 0;
	m_dirty = false;
	m_isSaved = false;

	createActions();

	m_view = new QTreeView(this);
	setCentralWidget(m_view);

	connect(m_view, SIGNAL(activated(QModelIndex)), SLOT(handleItemActivated(QModelIndex)));

	newArchive();
}

LSMainWindow::~LSMainWindow() {
	if (m_package)
		delete m_package;
}


void LSMainWindow::createActions() {
	m_newArchiveAction = new QAction("&New Archive", this);
	m_openArchiveAction = new QAction("&Open Archive...", this);
	m_saveAction = new QAction("&Save", this);
	m_saveArchiveAsAction = new QAction("Save Archive &As...", this);

	connect(m_newArchiveAction, SIGNAL(triggered()), SLOT(newArchive()));
	connect(m_openArchiveAction, SIGNAL(triggered()), SLOT(openArchive()));
	connect(m_saveAction, SIGNAL(triggered()), SLOT(save()));
	connect(m_saveArchiveAsAction, SIGNAL(triggered()), SLOT(saveArchiveAs()));

	m_addLayoutAction = new QAction("New &Layout...", this);
	m_addAnimationAction = new QAction("New &Animation...", this);
	m_addTextureAction = new QAction("New &Texture...", this);
	m_importNewAction = new QAction("&New Item...", this);
	m_importAction = new QAction("&Replace This Item...", this);
	m_exportAction = new QAction("&Export to File...", this);
	m_renameAction = new QAction("Re&name Item...", this);
	m_removeAction = new QAction("&Remove Item...", this);

	m_addActionMapper = new QSignalMapper(this);
	m_addActionMapper->setMapping(m_addLayoutAction, (int)LYTPackageBase::Layout);
	m_addActionMapper->setMapping(m_addAnimationAction, (int)LYTPackageBase::Animation);
	m_addActionMapper->setMapping(m_addTextureAction, (int)LYTPackageBase::Texture);

	connect(m_addLayoutAction, SIGNAL(triggered()), m_addActionMapper, SLOT(map()));
	connect(m_addAnimationAction, SIGNAL(triggered()), m_addActionMapper, SLOT(map()));
	connect(m_addTextureAction, SIGNAL(triggered()), m_addActionMapper, SLOT(map()));
	connect(m_addActionMapper, SIGNAL(mapped(int)), SLOT(handleAddSomething(int)));

	connect(m_renameAction, SIGNAL(triggered()), SLOT(handleRenameItem()));
	connect(m_removeAction, SIGNAL(triggered()), SLOT(handleRemoveItem()));

	QMenuBar *bar = menuBar();
	QMenu *m;

	m = bar->addMenu("&File");
	m->addAction(m_newArchiveAction);
	m->addAction(m_openArchiveAction);
	m->addAction(m_saveAction);
	m->addAction(m_saveArchiveAsAction);

	m = bar->addMenu("&Edit");
	m->addAction(m_addLayoutAction);
	m->addAction(m_addAnimationAction);
	m->addAction(m_addTextureAction);
	m->addSeparator();
	QMenu *importMenu = m->addMenu("&Import from File");
	importMenu->addAction(m_importNewAction);
	importMenu->addAction(m_importAction);
	m->addAction(m_exportAction);
	m->addSeparator();
	m->addAction(m_renameAction);
	m->addAction(m_removeAction);
}


void LSMainWindow::handleAddSomething(int whatToAdd) {
	LYTPackageBase::ItemType what = (LYTPackageBase::ItemType)whatToAdd;

	const char *typeName;
	QString extension;
	switch (what) {
	case LYTPackageBase::Layout:
		typeName = "layout";
		extension = ".brlyt";
		break;
	case LYTPackageBase::Animation:
		typeName = "animation";
		extension = ".brlan";
		break;
	case LYTPackageBase::Texture:
		typeName = "texture";
		extension = ".tpl";
		break;
	default: return;
	}

	QString name = QInputDialog::getText(this,
						  "Add Item",
						  QString("Name this %1:").arg(typeName));

	if (!name.isEmpty()) {
		QByteArray data = LYTPackageBase::createSkeletonItem(what);
		if (!name.endsWith(extension))
			name.append(extension);

		m_package->write(what, name, data);
	}
}


void LSMainWindow::handleRemoveItem() {
	QString what = selectedItem();
	LYTPackageBase::ItemType whatType = selectedItemType();

	if (!what.isEmpty()) {
		QString confirmText = QString("Are you sure you want to delete '%1'? You'll lose it forever...!").arg(what);

		int confirm = QMessageBox::question(this,
											 "Confirm Removal",
											 confirmText,
											 QMessageBox::Yes,
											 QMessageBox::No);

		if (confirm == QMessageBox::Yes) {
			m_package->remove(whatType, what);
		}
	}
}


void LSMainWindow::handleRenameItem() {
	QString what = selectedItem();
	LYTPackageBase::ItemType whatType = selectedItemType();

	if (!what.isEmpty()) {
		QString newName = QInputDialog::getText(this,
												"Rename Item",
												QString("Enter a new name:"),
												QLineEdit::NoEcho,
												what);

		if (!newName.isEmpty()) {
			m_package->rename(whatType, what, newName);
		}
	}
}


QString LSMainWindow::selectedItem() const {
	QModelIndex idx = m_view->currentIndex();
	return m_model->itemNameForIndex(idx);
}

LYTPackageBase::ItemType LSMainWindow::selectedItemType() const {
	QModelIndex idx = m_view->currentIndex();
	return m_model->itemTypeForIndex(idx);
}


void LSMainWindow::handleItemActivated(const QModelIndex &index) {
	QString what = selectedItem();
	LYTPackageBase::ItemType whatType = selectedItemType();

	if (what.isEmpty())
		return;

	switch (whatType) {
	case LYTPackageBase::Layout:
		LSLayoutWindow *w = new LSLayoutWindow(m_package, what);
		w->show();
		break;
	}
}


bool LSMainWindow::ensureSaved() {
	// TODO
	return false;
}

void LSMainWindow::updateTitleBar() {
	QString title;
	if (m_isSaved)
		title = m_package->description();
	else
		title = "[Unsaved]";

	if (m_dirty)
		title.append('*');
	title.append(" - LayoutStudio");

	setWindowTitle(title);
	setWindowModified(m_dirty);
}

void LSMainWindow::newArchive() {
	if (ensureSaved())
		return;

	LYTArchivePackage *pkg = new LYTArchivePackage;
	setCurrentPackage(pkg);

	m_dirty = false;
	m_isSaved = false;
	updateTitleBar();
}

void LSMainWindow::openArchive() {
	if (ensureSaved())
		return;

	QString path = QFileDialog::getOpenFileName(this,
												"Open a Layout Archive",
												QString(),
												"Wii Archives (*.arc)"
												);

	if (path.isEmpty())
		return;

	LYTArchivePackage *pkg = new LYTArchivePackage(path);
	setCurrentPackage(pkg);

	m_dirty = false;
	m_isSaved = false;
	updateTitleBar();
}

void LSMainWindow::save() {
	// TODO: check that m_package is an arc
	if (m_isSaved) {
		m_dirty = false;
		m_package->savePackage();
		updateTitleBar();
	} else {
		saveArchiveAs();
	}
}

void LSMainWindow::saveArchiveAs() {
	// TODO: check that m_package is an arc
	QString newPath = QFileDialog::getSaveFileName(this,
												   "Save Layout Archive",
												   QString(),
												   "Wii Archives (*.arc)"
												   );

	if (newPath.isEmpty())
		return;

	LYTArchivePackage *pkg = (LYTArchivePackage*)m_package;
	pkg->setFilename(newPath);
	pkg->savePackage();

	m_dirty = false;
	m_isSaved = true;
	updateTitleBar();
}


void LSMainWindow::setCurrentPackage(LYTPackageBase *pkg) {
	m_package = pkg;

	QItemSelectionModel *oldSel = m_view->selectionModel();
	QAbstractItemModel *model = m_view->model();

	m_model = new LSPackageModel(pkg, this);
	m_view->setModel(m_model);

	if (model)
		delete model;
	if (oldSel)
		delete oldSel;
}
