#include "lspackagemodel.h"

struct ContentKind {
	LYTPackageBase::ItemType type;
	QString name;
};

static const ContentKind ContentKinds[4] = {
	{LYTPackageBase::Layout, "Layouts"},
	{LYTPackageBase::Animation, "Animations"},
	{LYTPackageBase::Texture, "Textures"},
	{LYTPackageBase::Font, "Fonts"},
};
const int ContentKindCount = 4;

static int ContentKindForType(LYTPackageBase::ItemType type) {
	switch (type) {
	case LYTPackageBase::Layout: return 0;
	case LYTPackageBase::Animation: return 1;
	case LYTPackageBase::Texture: return 2;
	case LYTPackageBase::Font: return 3;
	}
	return -1;
}

LSPackageModel::LSPackageModel(LYTPackageBase *pkg, QObject *parent) :
	QAbstractItemModel(parent)
{
	m_package = pkg;

	m_caches = new QStringList[ContentKindCount];
	for (int i = 0; i < ContentKindCount; i++) {
		m_caches[i] = pkg->list(ContentKinds[i].type);
		m_caches[i].sort();
	}

	connect(pkg, SIGNAL(fileWasAdded(LYTPackageBase::ItemType,QString)), SLOT(handleFileWasAdded(LYTPackageBase::ItemType,QString)));
	//connect(pkg, SIGNAL(fileWasModified(LYTPackageBase::ItemType,QString)), SLOT(handleFileWasModified(LYTPackageBase::ItemType,QString)));
	connect(pkg, SIGNAL(fileWasRemoved(LYTPackageBase::ItemType,QString)), SLOT(handleFileWasRemoved(LYTPackageBase::ItemType,QString)));
	connect(pkg, SIGNAL(fileWasRenamed(LYTPackageBase::ItemType,QString,QString)), SLOT(handleFileWasRenamed(LYTPackageBase::ItemType,QString,QString)));
}

LSPackageModel::~LSPackageModel() {
	delete[] m_caches;
}


QModelIndex LSPackageModel::index(int row, int column, const QModelIndex &parent) const {
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	if (!parent.isValid())
		return createIndex(row, column, 0);

	return createIndex(row, column, parent.row() + 1);
}

QModelIndex LSPackageModel::parent(const QModelIndex &child) const {
	if (!child.isValid())
		return QModelIndex();

	if (child.internalId() > 0)
		return createIndex(child.internalId() - 1, 0, 0);
	else
		return QModelIndex();
}

int LSPackageModel::rowCount(const QModelIndex &parent) const {
	if (!parent.isValid())
		return ContentKindCount;
	else if (parent.internalId() > 0)
		return 0; // an actual item
	else
		return m_caches[parent.row()].count();
}

int LSPackageModel::columnCount(const QModelIndex &parent) const {
	return 1;
}

QVariant LSPackageModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid())
		return QVariant();
	if (role != Qt::DisplayRole)
		return QVariant();

	int whatIs = index.internalId();
	if (whatIs == 0)
		return ContentKinds[index.row()].name;
	else
		return m_caches[whatIs - 1].at(index.row());
}


void LSPackageModel::handleFileWasAdded(LYTPackageBase::ItemType type, QString name) {
	int kind = ContentKindForType(type);

	QStringList newCache = m_caches[kind];
	newCache.append(name);
	newCache.sort();

	// where was this added?
	int idx = newCache.indexOf(name);
	beginInsertRows(createIndex(kind, 0, 0), idx, idx);
	m_caches[kind] = newCache;
	endInsertRows();
}

void LSPackageModel::handleFileWasRemoved(LYTPackageBase::ItemType type, QString name) {
	int kind = ContentKindForType(type);

	int idx = m_caches[kind].indexOf(name);
	beginRemoveRows(createIndex(kind, 0, 0), idx, idx);
	m_caches[kind].removeAt(idx);
	endRemoveRows();
}

void LSPackageModel::handleFileWasRenamed(LYTPackageBase::ItemType type, QString from, QString to) {
	int kind = ContentKindForType(type);

	QStringList newCache = m_caches[kind];
	int fromIdx = newCache.indexOf(from);

	// this is really a mess, but I cannot think of a better way to do it
	// first, make a new cache, but don't store it to m_caches yet...
	newCache[fromIdx] = to;
	newCache.sort();

	// and now, get the to Index...
	int toIdx = newCache.indexOf(to);
	// if the new index is less than the old one, then leave it that way
	// if the new index is the same as the old one, that's fine, do nothing
	// if the new index is more than the old one... add 1 to it to take into
	// account that the old one is no longer around when it was computed BUT Qt
	// expects it to be
	// did that make sense? probably not, oh well
	if (toIdx > fromIdx)
		toIdx++;

	QModelIndex whatChanged = createIndex(fromIdx, 0, kind + 1);
	emit dataChanged(whatChanged, whatChanged);

	QModelIndex parent = createIndex(kind, 0, 0);

	if (toIdx != fromIdx)
		beginMoveRows(parent, fromIdx, fromIdx, parent, toIdx);
	m_caches[kind] = newCache;
	if (toIdx != fromIdx)
		endMoveRows();
}


QString LSPackageModel::itemNameForIndex(const QModelIndex &index) const {
	if (index.internalId() > 0)
		return m_caches[index.internalId() - 1].at(index.row());
	return QString();
}

LYTPackageBase::ItemType LSPackageModel::itemTypeForIndex(const QModelIndex &index) const {
	if (index.internalId() > 0)
		return ContentKinds[index.internalId() - 1].type;
	return (LYTPackageBase::ItemType)-1;
}
