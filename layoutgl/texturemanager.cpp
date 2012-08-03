#include "texturemanager.h"

LGLTextureManager::LGLTextureManager() {
}

void LGLTextureManager::setup(QGLWidget *gl, const LYTLayout *layout) {
	// TODO: code to cleanup previous stuff

	//m_gl = gl;
	m_layout = layout;
	m_package = &layout->package();

	QStringList textures = layout->generateTextureRefs();

	foreach (const QString &texName, textures) {
		qDebug() << texName;

		QByteArray tplData = m_package->getTexture(texName);

		QDataStream tplStream(tplData);
		WiiTexPalette tpl(tplStream);

		const QImage &image = tpl.textures.first().image;
		image.save(QString("tpl/%2__%1.png").arg(texName).arg((int)tpl.textures.first().format));
		// dirty, dirty hack, TODO: FIXME
		GLuint tex = gl->bindTexture(image, GL_TEXTURE_2D);

		m_textures.insert(texName, tex);
		m_images.insert(texName, image);
	}
}
