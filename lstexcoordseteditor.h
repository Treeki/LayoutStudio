#ifndef LSTEXCOORDSETEDITOR_H
#define LSTEXCOORDSETEDITOR_H

#include <QWidget>
#include <QComboBox>
#include <QDoubleSpinBox>
#include "lyt/common.h"

class LSTexCoordSetEditor : public QWidget
{
	Q_OBJECT
public:
	explicit LSTexCoordSetEditor(QWidget *parent = 0);

	void setCoordPtr(QVector<LYTTexCoords> *coords);

private:
	QVector<LYTTexCoords> *m_targetCoords;
	QSpinBox *m_coordCount;
	QComboBox *m_chooser;
	QDoubleSpinBox *m_coordEntry[8];

	int m_loadingThings;

	void changeChooserCountTo(int count);
	void showCoordSet(int index);

private slots:
	void handleCoordCountChanged(int count);
	void handleSetSelected(int index);
	void handleCoordChanged(double value);

signals:
	void coordsEdited();
	
public slots:
	
};

#endif // LSTEXCOORDSETEDITOR_H
