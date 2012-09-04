#ifndef LSLAYOUTWINDOW_H
#define LSLAYOUTWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QDoubleSpinBox>
#include <QTreeView>
#include <QStackedLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QListView>
#include <QSplitter>
#include "lyt/packagebase.h"
#include "lyt/layout.h"
class LSPaneEditor;
class LSMaterialEditor;
class LGLWidget;

class LSLayoutWindow : public QMainWindow {
	Q_OBJECT
public:
	explicit LSLayoutWindow(LYTPackageBase *pkg, const QString &layoutName, QWidget *parent = 0);
	~LSLayoutWindow();

	QString layoutName() const { return m_layoutName; }
	// TODO: set layoutname method

	LYTLayout *editedLayout() const { return m_layout; }

private:
	QString m_layoutName;
	LYTPackageBase *m_package;
	LYTLayout *m_layout;

	LGLWidget *m_renderer;

	QTabWidget *m_tabWidget;

	// settings
	QDoubleSpinBox *m_widthBox, *m_heightBox;

	// scene graph
	QSplitter *m_sceneSplitter;

	QStackedLayout *m_sceneListSwitcher;
	QLineEdit *m_searchBox;
	QPushButton *m_clearSearchButton;
	QTreeView *m_sceneGraph;
	QListView *m_sceneSearchList;

	QStackedLayout *m_paneEditorSwitcher;
	LSPaneEditor *m_paneEditor;

	bool m_loadingSettings;

	// material things
	QSplitter *m_materialSplitter;

	QListView *m_materialList;
	QPushButton *m_addMaterialButton;
	QPushButton *m_removeMaterialButton;

	LSMaterialEditor *m_materialEditor;

private slots:
	void handleWidthChanged(double v);
	void handleHeightChanged(double v);
	void selectedPaneChanged(const QModelIndex &current, const QModelIndex &previous);
	void selectedMaterialChanged(const QModelIndex &current, const QModelIndex &previous);

signals:
	
public slots:
	
};

#endif // LSLAYOUTWINDOW_H
