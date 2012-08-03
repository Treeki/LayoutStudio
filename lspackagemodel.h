#ifndef LSPACKAGEMODEL_H
#define LSPACKAGEMODEL_H

#include <QAbstractItemModel>
#include "lyt/packagebase.h"

class LSPackageModel : public QAbstractItemModel {
	Q_OBJECT
public:
	explicit LSPackageModel(LYTPackageBase *pkg, QObject *parent = 0);
	~LSPackageModel();

	LYTPackageBase *package() const { return m_package; }

	QModelIndex index(int row, int column, const QModelIndex &parent) const;
	QModelIndex parent(const QModelIndex &child) const;
	int rowCount(const QModelIndex &parent) const;
	int columnCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;
	
signals:
	
private slots:
	//void handleAboutToAddFile(LYTPackageBase::ItemType type, QString name);
	//void handleAboutToRemoveFile(LYTPackageBase::ItemType type, QString name);
	//void handleAboutToRenameFile(LYTPackageBase::ItemType type, QString from, QString to);
	//void handleAboutToModifyFile(LYTPackageBase::ItemType type, QString name);

	void handleFileWasAdded(LYTPackageBase::ItemType type, QString name);
	void handleFileWasRemoved(LYTPackageBase::ItemType type, QString name);
	void handleFileWasRenamed(LYTPackageBase::ItemType type, QString from, QString to);
	//void handleFileWasModified(LYTPackageBase::ItemType type, QString name);

public slots:
	
protected:
	LYTPackageBase *m_package;

	QStringList *m_caches;
};

#endif // LSPACKAGEMODEL_H
