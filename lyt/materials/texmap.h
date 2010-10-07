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

#ifndef LYTTEXMAP_H
#define LYTTEXMAP_H

#include "../common.h"
#include <QtCore/QDataStream>

class LYTLayout; // forward declaration

class LYTTexMap {
public:
	LYTTexMap();

	void writeToDataStream(QDataStream &out, LYTLayout &layout);
	void readFromDataStream(QDataStream &in, LYTLayout &layout);

	void dumpToDebug();

	QString textureName;
	int wrap_s;
	int wrap_t;
	int mag_filter;
	int min_filter;
};

#endif // LYTTEXMAP_H
