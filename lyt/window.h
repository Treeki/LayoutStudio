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

#ifndef LYTWINDOW_H
#define LYTWINDOW_H

#include <QtGui/QColor>
#include <QtCore/QVector>

#include "common.h"
#include "pane.h"


class LYTWindow; // forward declaration

class LYTWindowFrame {
public:
	LYTWindowFrame(LYTWindow &window);


	void writeToDataStream(QDataStream &out) const;
	void readFromDataStream(QDataStream &in);

	void dumpToDebug() const;

	quint8 type; // 0-5; controls texture flipping; must investigate this more
	QString materialName;

protected:
	LYTWindow &m_window;
};


class LYTWindow : public LYTPane {
public:
	LYTWindow(LYTLayout &layout);
	~LYTWindow();


	Magic magic() const;

	void writeToDataStream(QDataStream &out) const;
	void readFromDataStream(QDataStream &in);

	void dumpToDebug(bool showHeading=true) const;


	float contentOverflowLeft;
	float contentOverflowRight;
	float contentOverflowTop;
	float contentOverflowBottom;

	QColor contentVtxColours[4];
	QString contentMaterialName;
	QVector<LYTTexCoords> contentTexCoords;

	QList<LYTWindowFrame *> frames;


protected:
	void writeContentInfo(QDataStream &out) const;
	void readContentInfo(QDataStream &in);

	friend class LYTWindowFrame;
};


#endif // LYTWINDOW_H
