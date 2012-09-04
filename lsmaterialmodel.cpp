#include "lsmaterialmodel.h"

LSMaterialModel::LSMaterialModel(LYTMaterialContainer *container, QObject *parent) :
	QAbstractListModel(parent)
{
	m_container = container;
}


int LSMaterialModel::rowCount(const QModelIndex &parent) const {
	return m_container->count();
}

QVariant LSMaterialModel::data(const QModelIndex &index, int role) const {
	if (index.isValid()) {
		switch (role) {
		case Qt::DisplayRole:
			return m_container->getNameOfIndex(index.row());
		}
	}

	return QVariant();
}
