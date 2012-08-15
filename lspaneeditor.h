#ifndef LSPANEEDITOR_H
#define LSPANEEDITOR_H

#include <QWidget>
#include <QGridLayout>
#include <QTabWidget>
#include <QMenu>
#include <QToolButton>
#include <QLabel>

class LSPaneEditor : public QWidget {
	Q_OBJECT
public:
	explicit LSPaneEditor(QWidget *parent = 0);

private:
	QLabel *m_headingLabel;
	QTabWidget *m_tabs;

	QToolButton *m_addChildButton;
	QMenu *m_addChildMenu;
	QToolButton *m_removeButton;
	
signals:
	
public slots:
	
};

#endif // LSPANEEDITOR_H
