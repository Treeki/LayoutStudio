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

#ifndef LYTPANE_H
#define LYTPANE_H

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QDataStream>
#include <QtCore/QDebug>

#include "common.h"

class LYTLayout;

class LYTPane {
public:
	LYTPane(LYTLayout &layout);
	virtual ~LYTPane();


	virtual Magic magic() const;

	enum PaneTypes {
		PaneType = 0, PictureType, TextBoxType, WindowType, BoundingType, PaneTypeCount
	};

	virtual void writeToDataStream(QDataStream &out) const;
	virtual void readFromDataStream(QDataStream &in);

	virtual void dumpToDebug(bool showHeading=true) const;

	virtual void addFontRefsToList(QStringList &list) const;

	LYTPane *findPaneByName(QString name, bool recursive) const;

	LYTLayout &layout() const;

	LYTPane *parent;
	QList<LYTPane *> children;

	quint8 flags;

	enum OriginType {
		Left = 0, Top = 0,
		Center = 1,
		Right = 2, Bottom = 2
	};
	OriginType horzOrigin;
	OriginType vertOrigin;

	float drawnVertexX() const;
	float drawnVertexY() const;

	quint8 alpha;

	QString name;
	QString userdata;

	float xTrans;
	float yTrans;
	float zTrans;

	float xRot;
	float yRot;
	float zRot;

	float xScale;
	float yScale;

	float width;
	float height;

	PaneTypes type() const { return m_type; }

protected:
	LYTLayout &m_layout;
	PaneTypes m_type;
};



#endif // LYTPANE_H
