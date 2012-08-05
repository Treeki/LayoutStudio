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
#include "lyt/packagebase.h"
#include "lyt/layout.h"

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

	QTabWidget *m_tabWidget;

	// settings
	QDoubleSpinBox *m_widthBox, *m_heightBox;

	// scene graph
	QStackedLayout *m_sceneListSwitcher;
	QLineEdit *m_searchBox;
	QPushButton *m_clearSearchButton;
	QTreeView *m_sceneGraph;
	QListView *m_sceneSearchList;

private slots:
	void handleWidthChanged(double v);
	void handleHeightChanged(double v);

signals:
	
public slots:
	
};

#endif // LSLAYOUTWINDOW_H
