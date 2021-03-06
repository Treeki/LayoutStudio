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

#ifndef LYTCOMMON_H
#define LYTCOMMON_H

#include <QtGlobal>

#include "../wii/common.h"

union Magic {
	char str[4];
	quint32 value;

	Magic(quint32 v) : value(v) { }
};

union Version {
	char str[2];
	quint16 value;

	Version(quint16 v) : value(v) { }
};



struct LYTTexCoords {
	QPointF coord[4];

	LYTTexCoords() {
		// Sane defaults
		coord[0] = QPointF(0.0f, 0.0f);
		coord[1] = QPointF(1.0f, 0.0f);
		coord[2] = QPointF(0.0f, 1.0f);
		coord[3] = QPointF(1.0f, 1.0f);
	}
};


#endif // LYTCOMMON_H
