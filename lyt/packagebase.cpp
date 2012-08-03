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

#include "packagebase.h"

LYTPackageBase::LYTPackageBase(QObject *parent) : QObject(parent) {
	// do nothing
}

LYTPackageBase::~LYTPackageBase() {
}



QString LYTPackageBase::defaultPathForItemType(ItemType type, bool withArc) {
	switch (type) {
	case Layout:
		return withArc ? "arc/blyt" : "blyt";
	case Animation:
		return withArc ? "arc/anim" : "anim";
	case Texture:
		return withArc ? "arc/timg" : "timg";
	case Font:
		return withArc ? "arc/font" : "font";
	default:
		return QString();
	}
}


QByteArray LYTPackageBase::createSkeletonItem(ItemType type) {
	return QByteArray();
}
