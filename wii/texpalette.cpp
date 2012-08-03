#include "texpalette.h"

WiiTexPalette::WiiTexPalette() { }

WiiTexPalette::WiiTexPalette(QDataStream &stream) {
	quint32 magic;
	stream >> (quint32&)magic;

	if (magic != 0x20AF30)
		qWarning() << "WiiTexPalette: tried to load a TPL without the proper magic";

	quint32 textureCount, headerSize;
	stream >> textureCount;
	stream >> headerSize;

	qDebug() << textureCount << "textures";
	textures.resize(textureCount);

	for (int i = 0; i < textureCount; i++) {
		quint32 textureOffs, paletteOffs;
		stream >> textureOffs;
		stream >> paletteOffs;

		int savePos = stream.device()->pos();

		readTexture(stream, textureOffs, paletteOffs, textures[i]);

		stream.device()->seek(savePos);
	}
}

static const int TexelWidths[] = {
	8, 8, 8, 4, 4, 4, 4, -1, 8, 8, 4
};

static const int TexelHeights[] = {
	8, 4, 4, 4, 4, 4, 4, -1, 8, 4, 4
};

static const int BitsPerPixel[] = {
	4, 8, 8, 16, 16, 16, 32, -1, 8, 16
};

// This bit shamelessly stolen from Dolphin, but it didn't QUITE work right...
/*inline uchar _3to8(uchar v) { return (v << 5) | (v << 2) | (v >> 1); }
inline uchar _4to8(uchar v) { return (v << 4) | v; }
inline uchar _5to8(uchar v) { return (v << 3) | (v << 2); }
inline uchar _6to8(uchar v) { return (v << 2) | (v >> 4); }*/
inline uchar _3to8(uchar v) { return (v << 5); }
inline uchar _4to8(uchar v) { return (v << 4); }
inline uchar _5to8(uchar v) { return (v << 3); }
inline uchar _6to8(uchar v) { return (v << 2); }

void WiiTexPalette::readTexture(QDataStream &in, int textureOffs, int paletteOffs, WiiTPLTexture &tex) {
	in.device()->seek(textureOffs);

	quint16 width, height;
	quint32 rawFormat, dataOffs, rawWrapS, rawWrapT, rawMinFilter, rawMagFilter;
	quint8 rawEdgeLODEnable;

	in >> height;
	in >> width;
	in >> rawFormat;
	in >> dataOffs;
	in >> rawWrapS;
	in >> rawWrapT;
	in >> rawMinFilter;
	in >> rawMagFilter;
	in >> tex.lodBias;
	in >> rawEdgeLODEnable;
	in >> tex.minLOD;
	in >> tex.maxLOD;

	tex.format = (GX::TextureFormat)rawFormat;
	tex.wrapS = (GX::WrapType)rawWrapS;
	tex.wrapT = (GX::WrapType)rawWrapT;
	tex.minFilter = (GX::TextureFilter)rawMinFilter;
	tex.magFilter = (GX::TextureFilter)rawMagFilter;
	tex.edgeLODEnable = (rawEdgeLODEnable > 0);

	bool formatValid = (
				(rawFormat <= 6) ||
				(rawFormat >= 8 && rawFormat <= 10) ||
				rawFormat == 14);

	if (!formatValid) {
		qWarning("unknown texture format (%d)", rawFormat);
		return;
	}

	tex.image = QImage(width, height, QImage::Format_ARGB32);

	int texelWidth = TexelWidths[rawFormat];
	int texelHeight = TexelHeights[rawFormat];
	int bpp = BitsPerPixel[rawFormat];

	// how much needs to be added on to get this texture aligned?
	int padWidth = width % texelWidth;
	int padHeight = height % texelHeight;

	// get the fully padded width
	int paddedWidth = width + ((padWidth > 0) ? (texelWidth - padWidth) : 0);
	int paddedHeight = height + ((padHeight > 0) ? (texelHeight - padHeight) : 0);

	int texDataSize = (paddedWidth * paddedHeight * bpp) / 8;

	// decode the thing
	in.device()->seek(dataOffs);

	QImage &image = tex.image;

	switch (tex.format) {
	case GX::I4:
	{
		for (int texelY = 0; texelY < paddedHeight; texelY += 8) {
			for (int texelX = 0; texelX < paddedWidth; texelX += 8) {
				for (int y = texelY; y < (texelY + 8); y++) {
					QRgb *scanline = (QRgb*)image.scanLine(y);

					for (int x = texelX; x < (texelX + 8); x += 2) {
						quint8 v;
						in >> v;

						if (x < width && y < height) {
							int _v = _4to8(v >> 4);
							scanline[x] = qRgb(_v, _v, _v);
						}
						if ((x+1) < width && y < height) {
							int _v = _4to8(v & 0xF);
							scanline[x+1] = qRgb(_v, _v, _v);
						}
					}
				}
			}
		}
	}
		break;
	case GX::I8:
	{
		for (int texelY = 0; texelY < paddedHeight; texelY += 4) {
			for (int texelX = 0; texelX < paddedWidth; texelX += 8) {
				for (int y = texelY; y < (texelY + 4); y++) {
					QRgb *scanline = (QRgb*)image.scanLine(y);

					for (int x = texelX; x < (texelX + 8); x++) {
						quint8 v;
						in >> v;

						if (x < width && y < height)
							scanline[x] = qRgb(v, v, v);
					}
				}
			}
		}
	}
		break;
	case GX::IA4:
	{
		for (int texelY = 0; texelY < paddedHeight; texelY += 4) {
			for (int texelX = 0; texelX < paddedWidth; texelX += 8) {
				for (int y = texelY; y < (texelY + 4); y++) {
					QRgb *scanline = (QRgb*)image.scanLine(y);

					for (int x = texelX; x < (texelX + 8); x++) {
						quint8 v;
						in >> v;

						if (x < width && y < height) {
							int _i = _4to8(v & 0xF);
							int _a = _4to8(v >> 4);
							scanline[x] = qRgba(_i, _i, _i, _a);
						}
					}
				}
			}
		}
	}
		break;
	case GX::IA8:
	{
		for (int texelY = 0; texelY < paddedHeight; texelY += 4) {
			for (int texelX = 0; texelX < paddedWidth; texelX += 4) {
				for (int y = texelY; y < (texelY + 4); y++) {
					QRgb *scanline = (QRgb*)image.scanLine(y);

					for (int x = texelX; x < (texelX + 4); x++) {
						quint16 v;
						in >> v;

						if (x < width && y < height)
							scanline[x] = qRgba((v&0xFF00)>>8, (v&0xFF00)>>8, (v&0xFF00)>>8, v&0xFF);
					}
				}
			}
		}
	}
		break;
	case GX::RGB565:
	{
		for (int texelY = 0; texelY < paddedHeight; texelY += 4) {
			for (int texelX = 0; texelX < paddedWidth; texelX += 4) {
				for (int y = texelY; y < (texelY + 4); y++) {
					QRgb *scanline = (QRgb*)image.scanLine(y);

					for (int x = texelX; x < (texelX + 4); x++) {
						quint16 v;
						in >> v;

						if (x < width && y < height) {
							scanline[x] = qRgb(
										_5to8((v >> 11) & 0x1F),
										_6to8((v >> 5) & 0x3F),
										_5to8(v & 0x1F));
						}
					}
				}
			}
		}
	}
		break;
	case GX::RGB5A3:
	{
		for (int texelY = 0; texelY < paddedHeight; texelY += 4) {
			for (int texelX = 0; texelX < paddedWidth; texelX += 4) {
				for (int y = texelY; y < (texelY + 4); y++) {
					QRgb *scanline = (QRgb*)image.scanLine(y);

					for (int x = texelX; x < (texelX + 4); x++) {
						quint16 v;
						in >> v;

						if (x < width && y < height) {
							if (v & 0x8000) {
								scanline[x] = qRgb(
											_5to8((v >> 10) & 0x1F),
											_5to8((v >> 5) & 0x1F),
											_5to8(v & 0x1F));
							} else {
								scanline[x] = qRgba(
											_4to8((v >> 8) & 0xF),
											_4to8((v >> 4) & 0xF),
											_4to8(v & 0xF),
											_3to8((v >> 12) & 0x7));
							}
						}
					}
				}
			}
		}
	}
		break;
	case GX::RGBA8:
	{
		QByteArray texel1(32, 0), texel2(32, 0);

		for (int texelY = 0; texelY < paddedHeight; texelY += 4) {
			for (int texelX = 0; texelX < paddedWidth; texelX += 4) {
				in.readRawData(texel1.data(), 32);
				in.readRawData(texel2.data(), 32);

				int offs = 0;

				for (int y = texelY; y < (texelY + 4); y++) {
					QRgb *scanline = (QRgb*)image.scanLine(y);

					for (int x = texelX; x < (texelX + 4); x++) {
						if (x < width && y < height) {
							scanline[x] = qRgba(texel1.at(offs+1), texel2.at(offs), texel2.at(offs+1), texel1.at(offs));
						}
						offs += 2;
					}
				}
			}
		}
	}
		break;
	default:
		qWarning("unhandled texture format (%d)", rawFormat);
	}
}


void WiiTexPalette::writeToDataStream(QDataStream &out) const {
}
