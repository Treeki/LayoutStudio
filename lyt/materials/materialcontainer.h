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

#ifndef LYTMATERIALCONTAINER_H
#define LYTMATERIALCONTAINER_H

#include <QtCore/QList>
#include <QtCore/QPair>
#include "material.h"

typedef QPair<QString, LYTMaterial *> LYTMaterialContainerEntry;

class LYTMaterialContainer {
public:
	LYTMaterialContainer() { };

	QList<LYTMaterialContainerEntry> list;


	void addMaterial(QString name, LYTMaterial *material) {
		LYTMaterialContainerEntry entry;
		entry.first = name;
		entry.second = material;
		this->list.append(entry);
	}

	void clear() { this->list.clear(); }
	int count() const { return this->list.count(); }


	LYTMaterial *getMaterialByName(QString name) const {
		foreach (LYTMaterialContainerEntry entry, this->list) {
			if (entry.first == name)
				return entry.second;
		}

		return 0;
	}

	LYTMaterial *getMaterialByIndex(int index) const {
		return this->list.at(index).second;
	}

	int getIndexOfName(QString name) const {
		int i = 0;

		foreach (LYTMaterialContainerEntry entry, this->list) {
			if (entry.first == name)
				return i;
			i += 1;
		}

		return -1;
	}


	QString getNameOfIndex(int index) const {
		return this->list.at(index).first;
	}
};

#endif // LYTMATERIALCONTAINER_H
