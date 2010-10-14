/*******************************************************************************
  This file is part of LayoutStudio (http://github.com/Treeki/LayoutStudio)
  Copyright (c) 2010 Treeki (treeki@gmail.com)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, version 2.0.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License 2.0 for more details.

  You should have received a copy of the GNU General Public License 2.0
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/

#ifndef LYTTEXTBOX_H
#define LYTTEXTBOX_H

#include <QtGui/QColor>

#include "pane.h"

class LYTTextBox : public LYTPane {
public:
	LYTTextBox(LYTLayout &layout);


	Magic magic() const;

	void writeToDataStream(QDataStream &out) const;
	void readFromDataStream(QDataStream &in);

	void dumpToDebug(bool showHeading=true) const;

	void addFontRefsToList(QStringList &list) const;

	quint16 bufferLength;
	QString text;

	QString materialName;
	QString fontName;

	quint8 alignment;
	quint8 alignmentOverride;

	QColor colour1;
	QColor colour2;

	float fontSizeX;
	float fontSizeY;
	float charSpace;
	float lineSpace;
};


#endif // LYTTEXTBOX_H
