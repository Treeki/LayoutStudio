#ifndef LSMATERIALEDITOR_H
#define LSMATERIALEDITOR_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include "lyt/materials/materialcontainer.h"
#include "lscolorpicker.h"
#include "lsseteditor.h"

class LSTexMapEditor;
class LSTexSRTEditor;
class LSTexCoordGenEditor;
class LSIndTexSRTEditor;
class LSIndTexStageEditor;
class LSTevStageEditor;

class LSMaterialEditor : public QWidget {
	Q_OBJECT
public:
	explicit LSMaterialEditor(QWidget *parent = 0);

private:
	QLineEdit *m_nameEntry;

	LSColorPicker *m_colourPickers[7];

	bool m_currentlyLoadingMaterial;
	LYTMaterial *m_material;

	LSSetEditor<LYTTevStage, LSTevStageEditor> *m_tevStageSetEditor;

private slots:
	void handleNameChanged(QString value);
	void handleSaveChangedName();

	void handleColourPicked(QColor value);

signals:
	void mustRedrawLayout();

public slots:
	void setMaterial(LYTMaterial *mat);

};

#endif // LSMATERIALEDITOR_H
