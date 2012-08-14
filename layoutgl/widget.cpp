#include "widget.h"

LGLWidget::LGLWidget(QWidget *parent) :
	QGLWidget(parent), m_layout(0) {
}


void LGLWidget::setLayout(LYTLayout *layout) {
	// TODO: cleanup stuff for previous layout

	m_layout = layout;
	resize(layout->width + 64, layout->height + 64);
}


void LGLWidget::initializeGL() {
	qDebug() << "initialising GL";
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);

	// this makes texture transparency work
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// makes glColor value blend with textures
	glEnable(GL_COLOR_MATERIAL);

	m_texMgr.setup(this, m_layout);
}


void LGLWidget::resizeGL(int w, int h) {
	glViewport(0, 0, (GLint)w, (GLint)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float halfW = w / 2.0f, halfH = h / 2.0f;

	glOrtho(-halfW, halfW, -halfH, halfH, -100, 100);
	glMatrixMode(GL_MODELVIEW);
}


void LGLWidget::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (m_layout == 0)
		return;

	glLoadIdentity();

	renderPane(m_layout->rootPane);

	glColor3ub(255, 0, 0);
	float halfW = m_layout->width / 2.0f;
	float halfH = m_layout->height / 2.0f;

	const float lineExtension = 12.0f;
	glBegin(GL_QUADS);

	// top line
	glVertex2f(-halfW - 1.0f - lineExtension, halfH + 1.0f);
	glVertex2f(halfW + 1.0f + lineExtension, halfH + 1.0f);
	glVertex2f(halfW + 1.0f + lineExtension, halfH);
	glVertex2f(-halfW - 1.0f - lineExtension, halfH);
	// bottom line
	glVertex2f(-halfW - 1.0f - lineExtension, -halfH);
	glVertex2f(halfW + 1.0f + lineExtension, -halfH);
	glVertex2f(halfW + 1.0f + lineExtension, -halfH - 1.0f);
	glVertex2f(-halfW - 1.0f - lineExtension, -halfH - 1.0f);
	// left line
	glVertex2f(-halfW - 1.0f, halfH + 1.0f + lineExtension);
	glVertex2f(-halfW, halfH + 1.0f + lineExtension);
	glVertex2f(-halfW, -halfH - 1.0f - lineExtension);
	glVertex2f(-halfW - 1.0f, -halfH - 1.0f - lineExtension);
	// right line
	glVertex2f(halfW, halfH + 1.0f + lineExtension);
	glVertex2f(halfW + 1.0f, halfH + 1.0f + lineExtension);
	glVertex2f(halfW + 1.0f, -halfH - 1.0f - lineExtension);
	glVertex2f(halfW, -halfH - 1.0f - lineExtension);

	// centre: horizontal
	glVertex2f(-lineExtension, 0.5f);
	glVertex2f(lineExtension, 0.5f);
	glVertex2f(lineExtension, -0.5f);
	glVertex2f(-lineExtension, -0.5f);
	// centre: vertical
	glVertex2f(-0.5f, lineExtension);
	glVertex2f(0.5f, lineExtension);
	glVertex2f(0.5f, -lineExtension);
	glVertex2f(-0.5f, -lineExtension);

	glEnd();
}

void LGLWidget::renderPane(const LYTPane *pane) {
	glPushMatrix();

	glScalef(pane->xScale, pane->yScale, 1.0f);
	glRotatef(pane->xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(pane->yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(pane->zRot, 0.0f, 0.0f, 1.0f);
	glTranslatef(pane->xTrans, pane->yTrans, pane->zTrans);
	//qDebug() << "Translating by" << pane->xTrans << pane->yTrans << pane->zTrans;

	switch (pane->type()) {
	case LYTPane::PictureType:
		drawPicture((LYTPicture*)pane);
		break;
	case LYTPane::WindowType:
		drawWindow((LYTWindow*)pane);
		break;
	}

	foreach (const LYTPane *childPane, pane->children)
		renderPane(childPane);

	//qDebug() << "Popping";
	glPopMatrix();
}

void LGLWidget::drawPicture(const LYTPicture *pic) {
	float dX = pic->drawnVertexX();
	float dY = pic->drawnVertexY();
	//qDebug() << "Drawing" << dX << dY << pic->width << pic->height;

	useMaterial(pic->materialName);

	drawQuad(dX, dY, pic->width, pic->height, pic->texCoords, pic->vtxColours, pic->alpha);

	//glColor3ub(255, 255, 255);
	//renderText(dX, (dY-pic->height)+10, 0, pic->name);
}

struct TexFlipBit {
	quint8 bits[8];
	quint8 one, two;
};

static const TexFlipBit TextureFlipInfo[6] = {
	{{0,0,1,0,0,1,1,1},0,1},
	{{1,0,0,0,1,1,0,1},0,1},
	{{0,1,1,1,0,0,1,0},0,1},
	{{0,1,0,0,1,1,1,0},1,0},
	{{1,1,0,1,1,0,0,0},0,1},
	{{1,0,1,1,0,0,0,1},1,0}
};

void LGLWidget::dealWithWindowFrame(LYTTexCoords &coords, const QString &materialName,
							   int flipType, float pieceWidth, float pieceHeight,
							   int rep1, int rep2, int rep3, int rep4) {
	// What are you DOING Nintendo.. the code for this is a mess!
	// I seriously don't get it.
	// I am also using a terrible hack here.
	const TexFlipBit &info = TextureFlipInfo[flipType];

	qreal *hack = (qreal*)(&coords.coord[0]);

	float assign1 = info.bits[rep1 + info.one];
	hack[rep1 + info.one] = assign1;
	hack[rep3 + info.one] = assign1;

	float assign2 = info.bits[rep1 + info.two];
	hack[rep1 + info.two] = assign2;
	hack[rep2 + info.two] = assign2;

	float texSize[2];
	getTextureSize(materialName, &texSize[0], &texSize[1]);

	float assign3 = info.bits[rep1 + info.one] + (pieceWidth / ((info.bits[rep2 + info.one] - info.bits[rep1 + info.one]) * texSize[info.one]));
	hack[rep2 + info.one] = assign3;
	hack[rep4 + info.one] = assign3;

	float assign4 = info.bits[rep1 + info.two] + (pieceHeight / ((info.bits[rep3 + info.two] - info.bits[rep1 + info.two]) * texSize[info.two]));
	hack[rep3 + info.two] = assign4;
	hack[rep4 + info.two] = assign4;
}

void LGLWidget::drawWindow(const LYTWindow *wnd) {
	float dX = wnd->drawnVertexX();
	float dY = wnd->drawnVertexY();

	// get the frame size
	float frameLeft, frameTop, frameRight, frameBottom;

	switch (wnd->frames.count()) {
	case 1:
		float oneW, oneH;
		getTextureSize(wnd->frames.at(0)->materialName, &oneW, &oneH);
		frameLeft = frameRight = oneW;
		frameTop = frameBottom = oneH;
		break;
	case 4: case 8:
		getTextureSize(wnd->frames.at(0)->materialName, &frameLeft, &frameTop);
		getTextureSize(wnd->frames.at(3)->materialName, &frameRight, &frameBottom);
		break;
	}

	// draw the content
	useMaterial(wnd->contentMaterialName);
	//qDebug() << "content material:" << wnd->contentMaterialName;
	drawQuad(
				dX + frameLeft - wnd->contentOverflowLeft,
				dY - frameTop + wnd->contentOverflowTop,
				((wnd->contentOverflowLeft + (wnd->width - frameLeft)) - frameRight) + wnd->contentOverflowRight,
				((wnd->contentOverflowTop + (wnd->height - frameTop)) - frameBottom) + wnd->contentOverflowBottom,
				wnd->contentTexCoords, wnd->contentVtxColours, wnd->alpha);

	// deal with the frame
	LYTTexCoords texCoords;

	switch (wnd->frames.count()) {
	case 1:
	{
		const LYTWindowFrame &frame = *wnd->frames.first();
		const LYTMaterial &mat = getMaterial(frame.materialName);

		if (!mat.texMaps.empty()) {
			useMaterial(mat);

			// top left
			float pieceWidth = wnd->width - frameRight;
			float pieceHeight = frameTop;

			dealWithWindowFrame(texCoords, frame.materialName, 0, pieceWidth, pieceHeight, 0, 2, 4, 6);
			drawQuad(dX, dY, pieceWidth, pieceHeight, 1, &texCoords, 0, wnd->alpha);

			// top right
			pieceWidth = frameRight;
			pieceHeight = wnd->height - frameBottom;

			dealWithWindowFrame(texCoords, frame.materialName, 1, pieceWidth, pieceHeight, 2, 0, 6, 4);

			drawQuad(dX + wnd->width - frameRight, dY, pieceWidth, pieceHeight,
					 1, &texCoords, 0, wnd->alpha);

			// bottom left
			pieceWidth = frameLeft;
			pieceHeight = wnd->height - frameTop;

			dealWithWindowFrame(texCoords, frame.materialName, 2, pieceWidth, pieceHeight, 4, 6, 0, 2);

			drawQuad(dX, dY - frameTop, pieceWidth, pieceHeight,
					 1, &texCoords, 0, wnd->alpha);

			// bottom right
			pieceWidth = wnd->width - frameLeft;
			pieceHeight = frameBottom;

			dealWithWindowFrame(texCoords, frame.materialName, 4, pieceWidth, pieceHeight, 6, 4, 2, 0);

			drawQuad(dX + frameLeft, dY - wnd->height + frameBottom, pieceWidth, pieceHeight,
					 1, &texCoords, 0, wnd->alpha);
		}
	}
		break;
	case 4:
	{
		// top left
		const LYTWindowFrame &fTL = *wnd->frames.at(0);
		const LYTMaterial &mTL = getMaterial(fTL.materialName);

		if (!mTL.texMaps.empty()) {
			useMaterial(mTL);

			float pieceWidth = wnd->width - frameRight;
			float pieceHeight = frameTop;

			dealWithWindowFrame(texCoords, fTL.materialName, fTL.type, pieceWidth, pieceHeight, 0, 2, 4, 6);
			drawQuad(dX, dY, pieceWidth, pieceHeight, 1, &texCoords, 0, wnd->alpha);
		}

		// top right
		const LYTWindowFrame &fTR = *wnd->frames.at(1);
		const LYTMaterial &mTR = getMaterial(fTR.materialName);

		if (!mTR.texMaps.empty()) {
			useMaterial(mTR);

			float pieceWidth = frameRight;
			float pieceHeight = wnd->height - frameBottom;

			dealWithWindowFrame(texCoords, fTR.materialName, fTR.type, pieceWidth, pieceHeight, 2, 0, 6, 4);

			drawQuad(dX + wnd->width - frameRight, dY, pieceWidth, pieceHeight,
					 1, &texCoords, 0, wnd->alpha);
		}

		// bottom left
		const LYTWindowFrame &fBL = *wnd->frames.at(2);
		const LYTMaterial &mBL = getMaterial(fBL.materialName);

		if (!mBL.texMaps.empty()) {
			useMaterial(mBL);

			float pieceWidth = frameLeft;
			float pieceHeight = wnd->height - frameTop;

			dealWithWindowFrame(texCoords, fBL.materialName, fBL.type, pieceWidth, pieceHeight, 4, 6, 0, 2);

			drawQuad(dX, dY - frameTop, pieceWidth, pieceHeight,
					 1, &texCoords, 0, wnd->alpha);
		}

		const LYTWindowFrame &fBR = *wnd->frames.at(3);
		const LYTMaterial &mBR = getMaterial(fBR.materialName);

		// bottom right
		if (!mBR.texMaps.empty()) {
			useMaterial(mBR);

			float pieceWidth = frameRight;
			float pieceHeight = wnd->height - frameBottom;

			dealWithWindowFrame(texCoords, fBR.materialName, fBR.type, pieceWidth, pieceHeight, 6, 4, 0, 2);

			drawQuad(dX + frameLeft, dY - wnd->height + frameBottom, pieceWidth, pieceHeight,
					 1, &texCoords, 0, wnd->alpha);
		}
	}
		break;
	default:
		qDebug() << "unhandled window frame count" << wnd->frames.count();
	}
}

static GLint GLWrapModes[] = {
	GL_CLAMP_TO_EDGE, // Clamp
	GL_REPEAT, // Repeat
	GL_MIRRORED_REPEAT, // Mirror
};

void LGLWidget::useMaterial(const QString &materialName) {
	const LYTMaterial *mat = m_layout->materials.getMaterialByName(materialName);

	useMaterial(*mat);
}

void LGLWidget::useMaterial(const LYTMaterial &mat) {
	int cutAt = mat.texMaps.count();

	for (int i = 0; i < cutAt; i++) {
		const LYTTexMap &texMap = mat.texMaps.at(i);

		const QString &texName = texMap.textureName;
		GLuint texID = m_texMgr.glTextureForName(texName);

		glActiveTexture(GL_TEXTURE0 + i);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLWrapModes[texMap.wrap_s]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLWrapModes[texMap.wrap_t]);

		// is there a matching TexSRT?
		// TODO: fix this
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();

		if (mat.texSRTs.count() > i) {
			const LYTTexSRT &srt = mat.texSRTs.at(i);

			glScalef(srt.xScale, srt.yScale, 1.0f);
			// rotate?
			glTranslatef(srt.xTrans, -srt.yTrans, 0.0f);
		}

		glMatrixMode(GL_MODELVIEW);
	}

	for (int i = cutAt; i < 8; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glDisable(GL_TEXTURE_2D);
	}

	glActiveTexture(GL_TEXTURE0);
}

void LGLWidget::drawQuad(float x, float y, float w, float h, const QVector<LYTTexCoords> &texCoords, const QColor *colours, uchar alpha) {
	drawQuad(x, y, w, h, texCoords.count(), texCoords.constData(), colours, alpha);
}

void LGLWidget::drawQuad(float x, float y, float w, float h, int texCoordCount, const LYTTexCoords *texCoords, const QColor *colours, uchar alpha) {
	if (!colours)
		glColor4ub(255, 255, 255, 255);

	glBegin(GL_QUADS);

	for (int i = 0; i < texCoordCount; i++)
		glMultiTexCoord2f(GL_TEXTURE0_ARB+i, texCoords[i].coord[0].x(), 1.0f-texCoords[i].coord[0].y());

	if (colours)
		qglColor(colours[0]);
	glVertex2f(x, y);

	for (int i = 0; i < texCoordCount; i++)
		glMultiTexCoord2f(GL_TEXTURE0_ARB+i, texCoords[i].coord[1].x(), 1.0f-texCoords[i].coord[1].y());

	if (colours)
		qglColor(colours[1]);
	glVertex2f(x + w, y);

	for (int i = 0; i < texCoordCount; i++)
		glMultiTexCoord2f(GL_TEXTURE0_ARB+i, texCoords[i].coord[3].x(), 1.0f-texCoords[i].coord[3].y());

	if (colours)
		qglColor(colours[2]);
	glVertex2f(x + w, y - h);

	for (int i = 0; i < texCoordCount; i++)
		glMultiTexCoord2f(GL_TEXTURE0_ARB+i, texCoords[i].coord[2].x(), 1.0f-texCoords[i].coord[2].y());

	if (colours)
		qglColor(colours[3]);
	glVertex2f(x, y - h);

	glEnd();
}


void LGLWidget::getTextureSize(const QString &materialName, float *w, float *h) {
	const LYTMaterial *mat = m_layout->materials.getMaterialByName(materialName);

	if (mat->texMaps.count() == 0) {
		*w = 0.0f;
		*h = 0.0f;
	} else {
		const LYTTexMap &tm = mat->texMaps.first();

		QImage img = m_texMgr.imageForName(tm.textureName);
		*w = img.width();
		*h = img.height();
	}
}

