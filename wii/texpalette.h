#ifndef WIITEXPALETTE_H
#define WIITEXPALETTE_H

#include "common.h"
#include "gx.h"
#include <QImage>

class WiiTPLTexture {
public:
	// TODO: palette stuff

	QImage image;
	GX::TextureFormat format;

	GX::WrapType wrapS, wrapT;

	GX::TextureFilter minFilter, magFilter;

	float lodBias;
	bool edgeLODEnable;
	quint8 minLOD, maxLOD;
};

class WiiTexPalette {
public:
	WiiTexPalette();
	WiiTexPalette(QDataStream &stream);

	void writeToDataStream(QDataStream &out) const;

	QVector<WiiTPLTexture> textures;

private:
	void readTexture(QDataStream &in, int textureOffs, int paletteOffs, WiiTPLTexture &tex);
};

#endif // WIITEXPALETTE_H
