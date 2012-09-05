#ifndef LSSETEDITOR_H
#define LSSETEDITOR_H

#include <QWidget>
#include <QSpinBox>
#include <QComboBox>

class _LSSetEntryEditorBaseBase : public QWidget {
	Q_OBJECT
protected:
	explicit _LSSetEntryEditorBaseBase(QWidget *parent = 0) :
		QWidget(parent) { }

signals:
	void dataEdited();
};

template <typename TData>
class LSSetEntryEditorBase : public _LSSetEntryEditorBaseBase {
protected:
	explicit LSSetEntryEditorBase(QWidget *parent = 0) :
		_LSSetEntryEditorBaseBase(parent) { }

private:
	void setCurrentEntry(TData &entry);
	// what??
	//friend class LSSetEditor<TData, LSSetEntryEditorBase<TData> >;

	TData *m_currentEntry;

protected:
	virtual void loadEntryFrom(const TData &entry) = 0;
	TData *currentEntry() const { return m_currentEntry; }
};




class _LSSetEditorBase : public QWidget {
	Q_OBJECT
public:
	explicit _LSSetEditorBase(int maxEntries, QWidget *parent = 0);

protected:
	int m_maxEntries;
	QSpinBox *m_entryCount;
	QComboBox *m_chooser;

	QWidget *m_setEditorWidget;

	int m_loadingThings;

	void setup(QWidget *eWidget);

	virtual void changeEntryCountTo(int count) = 0;
	virtual void showEntry(int index) = 0;
	virtual void resizeDataListTo(int count) = 0;

private slots:
	void handleEntryCountChanged(int count);
	void handleEntrySelected(int index);

signals:
	void dataEdited();

};

template <typename TData, typename TWidget>
class LSSetEditor : public _LSSetEditorBase {
public:
	explicit LSSetEditor(int maxEntries, QWidget *parent = 0) :
		_LSSetEditorBase(maxEntries, parent) {

		TWidget *w = new TWidget(this);
		setup(w);

		LSSetEntryEditorBase<TData> *checkMe = w;
		connect(checkMe, SIGNAL(dataEdited()), SIGNAL(dataEdited()));
	}

	void setData(QList<TData> *newData) {
		m_loadingThings++;

		m_data = newData;
		m_entryCount->setValue(newData->count());
		changeEntryCountTo(newData->count());

		m_chooser->setCurrentIndex(newData->count() ? 0 : -1);
		showEntry(newData->count() ? 0 : -1);

		m_loadingThings--;
	}

protected:
	QList<TData> *m_data;

	void changeEntryCountTo(int count) {
		m_loadingThings++;

		int existingCount = m_chooser->count();

		if (existingCount > count) {
			// remove something
			int nowSelected = m_chooser->currentIndex();

			if (nowSelected >= count) {
				// oops, we'll need to select something else
				showEntry(count - 1);
				m_chooser->setCurrentIndex(count - 1);
			}

			for (int i = (existingCount - 1); i >= count; i--)
				m_chooser->removeItem(i);

		} else if (count > existingCount) {
			// add something

			for (int i = existingCount; i < count; i++)
				m_chooser->addItem(QString("Set %1").arg(i + 1));
		}

		m_loadingThings--;
	}

	void resizeDataListTo(int count) {
		m_data->reserve(count);

		while (m_data->count() < count)
			m_data->append(TData());
		while (m_data->count() > count)
			m_data->removeLast();
	}

	void showEntry(int index) {
		m_loadingThings++;

		if (index == -1) {
			m_setEditorWidget->setEnabled(false);
		} else {
			const TData &entry = m_data->at(index);
		}
	}
};




#endif // LSSETEDITOR_H
