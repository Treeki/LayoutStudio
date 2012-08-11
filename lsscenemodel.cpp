#include "lsscenemodel.h"
#include "lsglobals.h"

LSSceneModel::LSSceneModel(LYTLayout *layout, QObject *parent) :
	QAbstractItemModel(parent)
{
	m_layout = layout;

	m_paneIcons[LYTPane::PaneType] = LSGlobals::getIcon("pane");
	m_paneIcons[LYTPane::PictureType] = LSGlobals::getIcon("picture");
	m_paneIcons[LYTPane::TextBoxType] = LSGlobals::getIcon("textbox");
	m_paneIcons[LYTPane::WindowType] = LSGlobals::getIcon("window");
	m_paneIcons[LYTPane::BoundingType] = LSGlobals::getIcon("bounding");
}


QModelIndex LSSceneModel::index(int row, int column, const QModelIndex &parent) const {
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	if (!parent.isValid())
		return createIndex(row, column, m_layout->rootPane);

	// what's the parent..?
	LYTPane *parentPane = (LYTPane*)parent.internalPointer();
	LYTPane *pane = parentPane->children.at(row);
	return createIndex(row, column, pane);
}

QModelIndex LSSceneModel::parent(const QModelIndex &child) const {
	if (!child.isValid())
		return QModelIndex();

	LYTPane *childPane = (LYTPane*)child.internalPointer();
	LYTPane *parentPane = childPane->parent;
	if (parentPane) {
		LYTPane *parentParentPane = parentPane->parent;
		int index = parentParentPane ? parentParentPane->children.indexOf(parentPane) : 0;
		return createIndex(index, 0, parentPane);
	} else {
		return QModelIndex();
	}
}

int LSSceneModel::rowCount(const QModelIndex &parent) const {
	if (!parent.isValid()) {
		return 1; // the root pane
	}

	LYTPane *parentPane = (LYTPane*)parent.internalPointer();
	return parentPane->children.count();
}

int LSSceneModel::columnCount(const QModelIndex &parent) const {
	(void)parent;
	return 1;
}

QVariant LSSceneModel::data(const QModelIndex &index, int role) const {
	LYTPane *pane = (LYTPane*)index.internalPointer();
	if (pane) {
		switch (role) {
		case Qt::DisplayRole:
			return pane->name;
		case Qt::DecorationRole:
			return m_paneIcons[pane->type()];
		}
	}
	return QVariant();
}


Qt::ItemFlags LSSceneModel::flags(const QModelIndex &index) const {
	Qt::ItemFlags flag;
	flag = Qt::ItemIsEnabled | Qt::ItemIsSelectable |
			Qt::ItemIsDropEnabled | Qt::ItemIsEditable;

	if (index.isValid() && index.parent().isValid())
		flag |= Qt::ItemIsDragEnabled;

	return flag;
}


Qt::DropActions LSSceneModel::supportedDropActions() const {
	return Qt::CopyAction | Qt::MoveAction;
}
