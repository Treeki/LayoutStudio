#include "stringtablebuilder.h"

WiiStringTableBuilder::WiiStringTableBuilder() {
	m_nextOffset = 0;
	m_data = "";
}

quint32 WiiStringTableBuilder::add(QString str) {
	if (m_lookup.contains(str))
		return m_lookup.value(str);

	quint32 added = m_nextOffset;
	m_lookup.insert(str, added);

	m_data.append(str.toAscii());
	m_data.append('\0');

	m_nextOffset = m_data.length();

	return added;
}

QByteArray WiiStringTableBuilder::pack() {
	return m_data;
}
