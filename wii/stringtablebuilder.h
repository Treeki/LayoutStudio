#ifndef STRINGTABLEBUILDER_H
#define STRINGTABLEBUILDER_H

#include "common.h"
#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtCore/QByteArray>

class WiiStringTableBuilder {
public:
    WiiStringTableBuilder();

	quint32 add(QString str);
	QByteArray pack() const;

protected:
	int m_nextOffset;
	QByteArray m_data;
	QHash<QString, quint32> m_lookup;
};

#endif // STRINGTABLEBUILDER_H
