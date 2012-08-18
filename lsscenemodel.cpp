#include "lsscenemodel.h"
#include "lsglobals.h"

LSSceneModel::LSSceneModel(LYTLayout *layout, bool exposeVisibility, QObject *parent) :
	QAbstractItemModel(parent), m_exposesVisibility(exposeVisibility)
{
	m_layout = layout;

	m_paneIcons[LYTPane::PaneType] = LSGlobals::getIcon("pane");
	m_paneIcons[LYTPane::PictureType] = LSGlobals::getIcon("picture");
	m_paneIcons[LYTPane::TextBoxType] = LSGlobals::getIcon("textbox");
	m_paneIcons[LYTPane::WindowType] = LSGlobals::getIcon("window");
	m_paneIcons[LYTPane::BoundingType] = LSGlobals::getIcon("bounding");

	m_movingPaneParent = 0;
}

LSSceneModel::~LSSceneModel() {
	if (m_movingPaneParent)
		delete m_movingPaneParent;
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

		if (m_exposesVisibility && role == Qt::CheckStateRole) {
			return pane->visible ? Qt::Checked : Qt::Unchecked;
		}
	}
	return QVariant();
}

bool LSSceneModel::setData(const QModelIndex &index, const QVariant &value, int role) {
	if (m_exposesVisibility && role == Qt::CheckStateRole) {
		LYTPane *pane = (LYTPane*)index.internalPointer();

		bool newVisible = value.toBool();
		if (pane->visible != newVisible) {
			pane->visible = newVisible;
			emit dataChanged(index, index);
			emit paneVisibilityChanged();
		}
	}

	return false;
}


Qt::ItemFlags LSSceneModel::flags(const QModelIndex &index) const {
	Qt::ItemFlags flag;
	flag = Qt::ItemIsEnabled | Qt::ItemIsSelectable |
			Qt::ItemIsDropEnabled;

	if (index.isValid() && index.parent().isValid())
		flag |= Qt::ItemIsDragEnabled;

	if (m_exposesVisibility)
		flag |= Qt::ItemIsUserCheckable;

	return flag;
}


Qt::DropActions LSSceneModel::supportedDropActions() const {
	return Qt::MoveAction;
}


// I am doing a terrible, terrible, terrible thing here.
// I hate drag-and-drop.

// As far as I can see, insertRows and removeRows are ONLY called by Qt when
// dragging something. So instead of doing a real insertion/removal... I'll
// store the intended destination, and once I'm told what pane needs to be
// removed (moved) I do the whole thing at once.

bool LSSceneModel::insertRows(int row, int count, const QModelIndex &parent) {
	qDebug("LSSceneModel::insertRows(%d, %d, something)", row, count);

	if (m_movingPaneParent) {
		qWarning("huh, already moving something? dunno");
		return false;
	}

	if (!parent.isValid()) {
		qWarning("can't move stuff to the top level");
		return false;
	}

	m_movingPaneParent = new QPersistentModelIndex(parent);
	m_movingPaneRow = row;
	m_movingPaneCount = count;

	return true;
}

bool LSSceneModel::removeRows(int row, int count, const QModelIndex &parent) {
	qDebug("LSSceneModel::removeRows(%d, %d, something)", row, count);

	if (!m_movingPaneParent) {
		qWarning("huh, not moving anything / nothing left to move? dunno");
		return false;
	}

	// detach all the existing panes
	LYTPane *parentPane = (LYTPane*)parent.internalPointer();

	QVector<LYTPane *> removingPanes;
	removingPanes.reserve(count);

	beginRemoveRows(parent, row, row + count - 1);
	for (int i = 0; i < count; i++) {
		removingPanes.append(parentPane->children.at(row));
		parentPane->children.removeAt(row);
	}
	endRemoveRows();

	// now add them in their new homes!
	LYTPane *newParentPane = (LYTPane*)m_movingPaneParent->internalPointer();

	// note: compensate for the offset: if we're moving the thing within the
	// same parent and the destination row is higher than the source row, then
	// removing the source row will have changed the index of the row the user
	// actually wanted to move the moved row to... what a terrible sentence :|
	if (*m_movingPaneParent == parent && m_movingPaneRow > row)
		m_movingPaneRow -= count;

	beginInsertRows(*m_movingPaneParent, m_movingPaneRow, m_movingPaneRow + count - 1);

	for (int i = 0; i < count; i++) {
		LYTPane *pane = removingPanes.at(i);

		pane->parent = newParentPane;
		newParentPane->children.insert(m_movingPaneRow, pane);

		m_movingPaneRow++;
	}

	endInsertRows();

	// clean up if needed
	m_movingPaneCount -= count;

	if (m_movingPaneCount <= 0) {
		delete m_movingPaneParent;
		m_movingPaneParent = 0;
	}

	return true;
}

