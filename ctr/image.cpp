#include "image.h"
#include "../lyt/binaryfile.h"
#include "rg_etc1.h"
quint64 switchEndianness(quint64 v)
{
quint64 ret;
quint8* retA=(quint8*)&ret;
quint8* vA=(quint8*)&v;

retA[0]=vA[7];
retA[1]=vA[6];
retA[2]=vA[5];
retA[3]=vA[4];
retA[4]=vA[3];
retA[5]=vA[2];
retA[6]=vA[1];
retA[7]=vA[0];

return ret;
}

CTRImageCollection::CTRImageCollection() { }

CTRImageCollection::CTRImageCollection(QDataStream &stream) {
	stream.setByteOrder(QDataStream::LittleEndian);
	int i = 0;

	for(;;) {
		char checkMagic[4];
		if (!stream.device()->seek(i))
			break;
		stream.readRawData(checkMagic, 4);

		if (memcmp(checkMagic, "CLIM", 4) == 0) {
			images.resize(1);

			// our header! we have it! \o/
			// this is a bit of a kludge...but whatever.
			stream.skipRawData(8);
			quint32 fullFileSize;
			stream >> fullFileSize;

			QByteArray rawBinFile;
			rawBinFile.resize(fullFileSize - i);
			stream.device()->seek(i);
			stream.readRawData(rawBinFile.data(), rawBinFile.size());

			LYTBinaryFile binFile(rawBinFile);
			LYTBinaryFileSection &sect = binFile.sections.first();

			QDataStream *sectStream = sect.createReadStream();

			stream.device()->seek(0);

			readTexture(stream, *sectStream, images[0]);

			delete sectStream;

			break;
		}

		i += 0x20;
	}
}

static const int TexelWidths[] = {
	-1, -1, -1, 8, -1, -1, 8, 8, 8, 8, 8, 8
};

static const int TexelHeights[] = {
	-1, -1, -1, 8, -1, -1, 8, 8, 8, 8, 8, 8
};

static const int BitsPerPixel[] = {
	-1, -1, -1, 16, -1, -1, 24, 16, 16, 32, 4, 8
};

static const char *FormatNames[] = {
	"_unk 0", "_unk 1", "_unk 2", "IA8maybe",
	"_unk 4", "_unk 5", "RGB8", "RGB555",
	"RGBA4", "RGBA8", "ETC1", "ETC1A4"
};

// This bit shamelessly stolen from Dolphin, but it didn't QUITE work right...
/*inline uchar _3to8(uchar v) { return (v << 5) | (v << 2) | (v >> 1); }
inline uchar _4to8(uchar v) { return (v << 4) | v; }
inline uchar _5to8(uchar v) { return (v << 3) | (v << 2); }
inline uchar _6to8(uchar v) { return (v << 2) | (v >> 4); }*/
inline uchar _3to8(uchar v) { return (v << 5); }
inline uchar _4to8(uchar v) { return (v << 4) | v; }
inline uchar _5to8(uchar v) { return (v << 3); }
inline uchar _6to8(uchar v) { return (v << 2); }

static const int crap[] = {
	0,1,4,5,16,17,20,21,
	2,3,6,7,18,19,22,23,
	8,9,12,13,24,25,28,29,
	10,11,14,15,26,27,30,31,
	32,33,36,37,48,49,52,53,
	34,35,38,39,50,51,54,55,
	40,41,44,45,56,57,60,61,
	42,43,46,47,58,59,62,63
};

uint padToPowerOf2(uint value) {
	value--;
	value = (value >> 1) | value;
	value = (value >> 2) | value;
	value = (value >> 4) | value;
	value = (value >> 8) | value;
	value = (value >> 16) | value;
	return value + 1;
}


void CTRImageCollection::readTexture(QDataStream &in, QDataStream &header, CTRImage &tex) {
	quint16 width, height;
	quint32 format;
	quint32 imageDataSize;
	header >> width;
	header >> height;
	header >> format;
	header >> imageDataSize;

	tex.format = format;

	bool formatValid =
				(format == 3) || ((format >= 6) && (format <= 11));

	if (!formatValid) {
		qWarning("unknown texture format (%d)", format);
		return;
	}

	qDebug() << "format:" << FormatNames[format] << "(" << format << "); size:" << width << "x" << height;

	int texelWidth = TexelWidths[format];
	int texelHeight = TexelHeights[format];
	int bpp = BitsPerPixel[format];

	// how much needs to be added on to get this texture aligned?
	//int padWidth = width % 16;
	//int padHeight = height % 16;

	// get the fully padded width
	//int paddedWidth = width + ((padWidth > 0) ? (16 - padWidth) : 0);
	//int paddedHeight = height + ((padHeight > 0) ? (16 - padHeight) : 0);
	int paddedWidth = padToPowerOf2(width);
	int paddedHeight = padToPowerOf2(height);

	// TESTING
	//width = paddedWidth;
	//height = paddedHeight;
	// END TESTING

	tex.image = QImage(width, height, QImage::Format_ARGB32);

	// decode the thing
	QImage &image = tex.image;

	switch (tex.format) {
	case 10:
	{
		QByteArray texel(texelWidth * texelHeight / 2, 0);

		for (int texelY = 0; texelY < paddedHeight; texelY += texelHeight) {
			for (int texelX = 0; texelX < paddedWidth; texelX += texelWidth) {
				quint64 block;
				quint32 pixels[4*4];
				quint32 *ptr;

				in.readRawData((char*)(&block), 8);
				block = switchEndianness(block);
				rg_etc1::unpack_etc1_block(&block, pixels, false);
				ptr = pixels;
				for (int y = texelY; y < (texelY + 4); y++) {
					for (int x = texelX; x < (texelX + 4); x++) {
						if (x < width && y < height) {
							((QRgb*)image.scanLine(y))[x] = *ptr;
						}
						++ptr;
					}
				}

				in.readRawData((char*)(&block), 8);
				block = switchEndianness(block);
				rg_etc1::unpack_etc1_block(&block, pixels, false);
				ptr = pixels;
				for (int y = texelY; y < (texelY + 4); y++) {
					for (int x = texelX + 4; x < (texelX + 8); x++) {
						if (x < width && y < height) {
							((QRgb*)image.scanLine(y))[x] = *ptr;
						}
						++ptr;
					}
				}

				in.readRawData((char*)(&block), 8);
				block = switchEndianness(block);
				rg_etc1::unpack_etc1_block(&block, pixels, false);
				ptr = pixels;
				for (int y = texelY + 4; y < (texelY + 8); y++) {
					for (int x = texelX; x < (texelX + 4); x++) {
						if (x < width && y < height) {
							((QRgb*)image.scanLine(y))[x] = *ptr;
						}
						++ptr;
					}
				}

				in.readRawData((char*)(&block), 8);
				block = switchEndianness(block);
				rg_etc1::unpack_etc1_block(&block, pixels, false);
				ptr = pixels;
				for (int y = texelY + 4; y < (texelY + 8); y++) {
					for (int x = texelX + 4; x < (texelX + 8); x++) {
						if (x < width && y < height) {
							((QRgb*)image.scanLine(y))[x] = *ptr;
						}
						++ptr;
					}
				}
			}
		}
	}
		break;
	case 11:
	{
		quint8 alphaData[8];
		quint64 block;
		quint32 pixels[4*4];
		quint32 *ptr;

		static const int offsX[] = {0,4,0,4};
		static const int offsY[] = {0,0,4,4};

		for (int texelY = 0; texelY < paddedHeight; texelY += texelHeight) {
			for (int texelX = 0; texelX < paddedWidth; texelX += texelWidth) {

				for (int i = 0; i < 4; i++) {
					in.readRawData((char*)(alphaData), 8);
					in.readRawData((char*)(&block), 8);
					block = switchEndianness(block);
					rg_etc1::unpack_etc1_block(&block, pixels, false);
					ptr = pixels;
					for (int y = texelY + offsY[i]; y < (texelY + offsY[i] + 4); y++) {
						QRgb *scanline = (QRgb*)image.scanLine(y);

						for (int x = texelX + offsX[i]; x < (texelX + offsX[i] + 4); x++) {
							if (x < width && y < height) {
								int si = (y-texelY-offsY[i])+((x-texelX-offsX[i])*4);
								quint8 byte = alphaData[si >> 1];

								quint8 alpha = _4to8((si & 1) ? (byte >> 4) : (byte & 15));

								scanline[x] = (*ptr & 0xFFFFFF) | (alpha << 24);
							}
							++ptr;
						}
					}
				}
			}
		}
	}
		break;
	case 3:
	{
		QByteArray texel(texelWidth * texelHeight * 2, 0);

		for (int texelY = 0; texelY < paddedHeight; texelY += texelHeight) {
			for (int texelX = 0; texelX < paddedWidth; texelX += texelWidth) {
				in.readRawData(texel.data(), texel.size());

				for (int y = texelY; y < (texelY + texelHeight); y++) {
					QRgb *scanline = (QRgb*)image.scanLine(y);

					for (int x = texelX; x < (texelX + texelWidth); x++) {
						if (x < width && y < height) {
							int si = crap[((y-texelY)*8)+(x-texelX)] * 2;
							quint8 v = texel.at(si+1);
							quint8 a = texel.at(si);
							scanline[x] = qRgba(v,v,v,a);
						}
					}
				}
			}
		}
	}
		break;
	case 6:
	{
		QByteArray texel(texelWidth * texelHeight * 3, 0);

		for (int texelY = 0; texelY < paddedHeight; texelY += texelHeight) {
			for (int texelX = 0; texelX < paddedWidth; texelX += texelWidth) {
				in.readRawData(texel.data(), texel.size());

				for (int y = texelY; y < (texelY + texelHeight); y++) {
					QRgb *scanline = (QRgb*)image.scanLine(y);

					for (int x = texelX; x < (texelX + texelWidth); x++) {
						if (x < width && y < height) {
							int si = crap[((y-texelY)*8)+(x-texelX)] * 3;
							scanline[x] = qRgb(texel.at(si+2), texel.at(si+1), texel.at(si));
						}
					}
				}
			}
		}
	}
		break;
	case 7:
	{
		QByteArray texel(texelWidth * texelHeight * 2, 0);

		for (int texelY = 0; texelY < paddedHeight; texelY += texelHeight) {
			for (int texelX = 0; texelX < paddedWidth; texelX += texelWidth) {
				in.readRawData(texel.data(), texel.size());

				for (int y = texelY; y < (texelY + texelHeight); y++) {
					QRgb *scanline = (QRgb*)image.scanLine(y);

					for (int x = texelX; x < (texelX + texelWidth); x++) {
						if (x < width && y < height) {
							int si = crap[((y-texelY)*8)+(x-texelX)] * 2;
							uchar _a = texel.at(si);
							uchar _b = texel.at(si+1);
							quint16 v = (_b << 8) | _a;
							if ((v & 1) == 0)
								scanline[x] = qRgba(0,0,0,0);
							else
								scanline[x] = qRgb(_5to8((v & 0xF800) >> 11), _5to8((v & 0x7C0) >> 6), _5to8((v & 0x2E) >> 1));
						}
					}
				}
			}
		}
	}
		break;
	case 8:
	{
		QByteArray texel(texelWidth * texelHeight * 2, 0);

		for (int texelY = 0; texelY < paddedHeight; texelY += texelHeight) {
			for (int texelX = 0; texelX < paddedWidth; texelX += texelWidth) {
				in.readRawData(texel.data(), texel.size());

				for (int y = texelY; y < (texelY + texelHeight); y++) {
					QRgb *scanline = (QRgb*)image.scanLine(y);

					for (int x = texelX; x < (texelX + texelWidth); x++) {
						if (x < width && y < height) {
							int si = crap[((y-texelY)*8)+(x-texelX)] * 2;
							uchar _a = texel.at(si);
							uchar _b = texel.at(si+1);
							scanline[x] = qRgba(_4to8(_b >> 4), _4to8(_b & 15), _4to8(_a >> 4), _4to8(_a & 15));
						}
					}
				}
			}
		}
	}
		break;
	case 9:
	{
		QByteArray texel(texelWidth * texelHeight * 4, 0);

		for (int texelY = 0; texelY < paddedHeight; texelY += texelHeight) {
			for (int texelX = 0; texelX < paddedWidth; texelX += texelWidth) {
				in.readRawData(texel.data(), texel.size());

				for (int y = texelY; y < (texelY + texelHeight); y++) {
					QRgb *scanline = (QRgb*)image.scanLine(y);

					for (int x = texelX; x < (texelX + texelWidth); x++) {
						if (x < width && y < height) {
							int si = crap[((y-texelY)*8)+(x-texelX)] * 4;
							scanline[x] = qRgba(texel.at(si+3), texel.at(si+2), texel.at(si+1), texel.at(si));
						}
					}
				}
			}
		}
	}
		break;
	default:
		qWarning("unhandled texture format (%d)", format);
	}
}


void CTRImageCollection::writeToDataStream(QDataStream &out) const {
}
