#ifndef CTRIMAGE_H
#define CTRIMAGE_H

#include "../wii/common.h"
#include <QImage>

class CTRImage {
public:
	int format;
	QImage image;
};

class CTRImageCollection {
public:
	CTRImageCollection();
	CTRImageCollection(QDataStream &stream);

	void writeToDataStream(QDataStream &out) const;

	QVector<CTRImage> images;

private:
	void readTexture(QDataStream &in, QDataStream &header, CTRImage &tex);
};

#endif // CTRIMAGE_H
