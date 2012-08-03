#ifndef WIDGET_H
#define WIDGET_H

#include <QGLWidget>

#include "lyt/layout.h"
#include "layoutgl/texturemanager.h"

class LGLWidget : public QGLWidget
{
	Q_OBJECT
public:
	explicit LGLWidget(QWidget *parent = 0);

	void setLayout(LYTLayout *layout);

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

	LYTLayout *m_layout;

	void renderPane(const LYTPane *pane);

	void drawPicture(const LYTPicture *pic);
	void drawWindow(const LYTWindow *wnd);

	const LYTMaterial &getMaterial(const QString &materialName) {
		return *m_layout->materials.getMaterialByName(materialName);
	}

	void useMaterial(const QString &materialName);
	void useMaterial(const LYTMaterial &material);

	void drawQuad(float x, float y, float w, float h, const QVector<LYTTexCoords> &texCoords, const QColor *colours, uchar alpha);
	void drawQuad(float x, float y, float w, float h, int texCoordCount, const LYTTexCoords *texCoords, const QColor *colours, uchar alpha);

	void getTextureSize(const QString &materialName, float *w, float *h);

	void dealWithWindowFrame(LYTTexCoords &coords, const QString &materialName, int flipType,
							 float pieceWidth, float pieceHeight,
							 int rep1, int rep2, int rep3, int rep4);

	LGLTextureManager m_texMgr;
	
signals:
	
public slots:
	
};

#endif // WIDGET_H
