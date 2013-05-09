#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "wii/texpalette.h"
#include "lyt/layout.h"
#include <QGLContext>
#include <QGLFunctions>
#include <QHash>

class LGLTextureManager : protected QGLFunctions {
public:
	LGLTextureManager();

	void setup(QGLWidget *gl, const LYTLayout *layout);

private:
	const QGLContext *m_gl;
	const LYTLayout *m_layout;
	const LYTPackageBase *m_package;

	QHash<QString, GLuint> m_textures;
	QHash<QString, QImage> m_images;

public:
	GLuint glTextureForName(const QString name) const {
		return m_textures.value(name);
	}

	QImage imageForName(const QString name) const {
		return m_images.value(name);
	}
};

#endif // TEXTUREMANAGER_H
