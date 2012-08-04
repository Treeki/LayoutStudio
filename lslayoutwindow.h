#ifndef LSLAYOUTWINDOW_H
#define LSLAYOUTWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QDoubleSpinBox>
#include "lyt/packagebase.h"

class LSLayoutWindow : public QMainWindow {
	Q_OBJECT
public:
	explicit LSLayoutWindow(LYTPackageBase *pkg, const QString &layoutName, QWidget *parent = 0);

	QString layoutName() const { return m_layoutName; }
	// TODO: set layoutname method

private:
	QString m_layoutName;
	LYTPackageBase *m_package;

	QTabWidget *m_tabWidget;

	// settings
	QDoubleSpinBox *m_widthBox, *m_heightBox;
	
signals:
	
public slots:
	
};

#endif // LSLAYOUTWINDOW_H
