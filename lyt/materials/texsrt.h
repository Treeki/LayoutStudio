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

#ifndef LYTTEXSRT_H
#define LYTTEXSRT_H

#include "../common.h"
#include <QtCore/QDataStream>

class LYTTexSRT {
public:
	LYTTexSRT();

	void writeToDataStream(QDataStream &out) const;
	void readFromDataStream(QDataStream &in);

	void dumpToDebug() const;

	float xTrans;
	float yTrans;
	float rotate;
	float xScale;
	float yScale;
};

#endif // LYTTEXSRT_H
