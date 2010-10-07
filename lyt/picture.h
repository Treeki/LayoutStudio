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

#ifndef LYTPICTURE_H
#define LYTPICTURE_H

#include <QtGui/QColor>
#include <QtCore/QVector>

#include "common.h"
#include "pane.h"

class LYTPicture : public LYTPane {
public:
	LYTPicture(LYTLayout &layout);


	void writeToDataStream(QDataStream &out);
	void readFromDataStream(QDataStream &in);

	void dumpToDebug(bool showHeading=true);

	QColor vtxColours[4];
	QString materialName;

	QVector<LYTTexCoords> texCoords;
};


#endif // LYTPICTURE_H
