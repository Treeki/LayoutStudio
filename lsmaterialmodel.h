#ifndef LSMATERIALMODEL_H
#define LSMATERIALMODEL_H

#include <QAbstractListModel>
#include "lyt/materials/materialcontainer.h"

class LSMaterialModel : public QAbstractListModel
{
	Q_OBJECT
public:
	explicit LSMaterialModel(LYTMaterialContainer *container, QObject *parent = 0);

	int rowCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;

private:
	LYTMaterialContainer *m_container;
	
signals:
	
public slots:
	
};

#endif // LSMATERIALMODEL_H
