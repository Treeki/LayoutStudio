#ifndef LSPANEEDITOR_H
#define LSPANEEDITOR_H

#include <QWidget>
#include <QGridLayout>
#include <QTabWidget>
#include <QMenu>
#include <QToolButton>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QCheckBox>

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


	// Main pane tab
	QWidget *m_paneTab;

	QLineEdit *m_nameEntry, *m_userDataEntry;
	QSpinBox *m_alpha;
	QCheckBox *m_influencedAlpha;

	QDoubleSpinBox *m_width, *m_height;
	QComboBox *m_horzOrigin, *m_vertOrigin;
	QCheckBox *m_widescreen, *m_visible;

	QDoubleSpinBox *m_transX, *m_transY, *m_transZ;
	QDoubleSpinBox *m_rotX, *m_rotY, *m_rotZ;
	QDoubleSpinBox *m_scaleX, *m_scaleY;

	void createPaneTab();
	
signals:
	
public slots:
	
};

#endif // LSPANEEDITOR_H
