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
#include "lyt/pane.h"
class LSTexCoordSetEditor;

class LYTPicture;

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
	QCheckBox *m_widescreen; //, *m_visible;

	union {
		QDoubleSpinBox *m_srtSpinBoxes[8];
		struct {
			QDoubleSpinBox *m_transSpinBoxes[3];
			QDoubleSpinBox *m_rotSpinBoxes[3];
			QDoubleSpinBox *m_scaleSpinBoxes[2];
		};
		struct {
			QDoubleSpinBox *m_transX, *m_transY, *m_transZ;
			QDoubleSpinBox *m_rotX, *m_rotY, *m_rotZ;
			QDoubleSpinBox *m_scaleX, *m_scaleY;
		};
	};

	// Picture tab
	QWidget *m_pictureTab;

	LSTexCoordSetEditor *m_picTexCoordEditor;
	QToolButton *m_picColourButtons[4];


	void createPaneTab();
	void createPictureTab();


	bool m_currentlyLoadingPane;
	union {
		LYTPane *m_pane;
		LYTPicture *m_picture;
	};

private slots:
	void handleNameChanged(QString value);
	void handleUserDataChanged(QString value);
	void handleAlphaChanged(int value);
	void handleInfluencedAlphaChanged(bool value);
	void handleWidthChanged(double value);
	void handleHeightChanged(double value);
	void handleHorzOriginChanged(int value);
	void handleVertOriginChanged(int value);
	void handleWidescreenChanged(bool value);
	//void handleVisibleChanged(bool value);
	void handleTransXChanged(double value);
	void handleTransYChanged(double value);
	void handleTransZChanged(double value);
	void handleRotXChanged(double value);
	void handleRotYChanged(double value);
	void handleRotZChanged(double value);
	void handleScaleXChanged(double value);
	void handleScaleYChanged(double value);

	void handlePicColourClicked();

signals:
	void mustRedrawLayout();
	
public slots:
	void setPane(LYTPane *pane);

};

#endif // LSPANEEDITOR_H
