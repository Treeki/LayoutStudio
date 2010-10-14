#ifndef WIICOMMON_H
#define WIICOMMON_H

#include <QtGlobal>
#include <QtCore/QByteArray>
#include <QtGui/QColor>
#include <QtCore/QPointF>
#include <QtCore/QDataStream>
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <QtCore/QVector>

inline quint32 AlignUp(quint32 value, quint32 alignTo) {
	return (value + alignTo - 1) & ~(alignTo - 1);
}

inline quint32 AlignDown(quint32 value, quint32 alignTo) {
	return value & ~(alignTo - 1);
}


inline quint32 BitExtract(quint32 value, int count, int start) {
	// this function relies on heavy compiler optimisation to be efficient :p
	quint32 mask = 0;
	for (int i = start; i < start+count; i++) {
		mask |= (0x80000000 >> i);
	}

	return (value & mask) >> (32 - (start + count));
}

inline quint32 BitInsert(quint32 value, int newValue, int count, int start) {
	quint32 mask = 0;
	for (int i = start; i < start+count; i++) {
		mask |= (0x80000000 >> i);
	}

	value &= ~mask;
	value |= (newValue << (32 - (start + count))) & mask;
	return value;
}

inline void WritePadding(int num, QDataStream &out) {
	for (int i = 0; i < num; i++)
		out << (quint8)0;
}




QByteArray PadByteArray(QByteArray original, int newLength, char padWith='\0');

inline quint32 ColorToRGBA(QColor col) {
	return (col.red() << 24) | (col.green() << 16) | (col.blue() << 8) | (col.alpha());
}

inline QColor RGBAToColor(quint32 col) {
	return QColor(col >> 24, (col >> 16) & 0xFF, (col >> 8) & 0xFF, col & 0xFF);
}

inline void ReadRGBA8Color(QColor &out, QDataStream &in) {
	quint32 col;
	in >> (quint32&)col;
	out = RGBAToColor(col);
}

inline void WriteRGBA8Color(QColor &in, QDataStream &out) {
	out << (quint32)ColorToRGBA(in);
}

inline void ReadS10Color(QColor &out, QDataStream &in) {
	quint16 r, g, b, a;
	in >> (quint16&)r;
	in >> (quint16&)g;
	in >> (quint16&)b;
	in >> (quint16&)a;
	out.setRgb(r, g, b, a);
}

inline void WriteS10Color(QColor &in, QDataStream &out) {
	out << (quint16)in.red();
	out << (quint16)in.green();
	out << (quint16)in.blue();
	out << (quint16)in.alpha();
}

inline void ReadPointF(QDataStream &stream, QPointF &point) {
	float x, y;
	stream >> x;
	stream >> y;
	point.setX(x);
	point.setY(y);
}

inline void WritePointF(QDataStream &stream, const QPointF &point) {
	stream << (float)point.x();
	stream << (float)point.y();
}

inline void InitDataStream(QDataStream &stream) {
	stream.setByteOrder(QDataStream::BigEndian);
	stream.setVersion(QDataStream::Qt_4_5);
}

QStringList ReadStringList(QDataStream &in);

QString ReadFixedLengthASCII(QDataStream &in, int length);
void WriteFixedLengthASCII(QDataStream &out, QString str, int length);

#endif // WIICOMMON_H
