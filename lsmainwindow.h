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

#ifndef LSMAINWINDOW_H
#define LSMAINWINDOW_H

#include "lyt/packagebase.h"
#include "lspackagemodel.h"
#include <QMainWindow>
#include <QTreeView>
#include <QSignalMapper>

class LSMainWindow : public QMainWindow {
    Q_OBJECT
public:
    LSMainWindow(QWidget *parent = 0);
    ~LSMainWindow();

protected:
    bool ensureSaved();

public:
    void updateTitleBar();

public slots:
    void newArchive();
    void openArchive();
    void save();
    void saveArchiveAs();
	void openDir();

private slots:
	void handleAddSomething(int whatToAdd);
	void handleRenameItem();
	void handleRemoveItem();
	void handleItemActivated(const QModelIndex &index);

private:
    LYTPackageBase *m_package;
    void setCurrentPackage(LYTPackageBase *pkg);

	void createActions();
	QAction *m_newArchiveAction, *m_openArchiveAction, *m_saveArchiveAsAction;
	QAction *m_openDirAction;
	QAction *m_saveAction;

	QAction *m_addLayoutAction, *m_addAnimationAction;
	QAction *m_addTextureAction;
	QSignalMapper *m_addActionMapper;
	QAction *m_importNewAction, *m_importAction;
	QAction *m_renameAction, *m_removeAction, *m_exportAction;

	LSPackageModel *m_model;
	QTreeView *m_view;

    bool m_dirty;
    bool m_isSaved;

protected:
	QString selectedItem() const;
	LYTPackageBase::ItemType selectedItemType() const;
};

#endif // LSMAINWINDOW_H
