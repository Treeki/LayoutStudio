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

#include "filesystem.h"


/******************************************************************************/

WiiFSObject::WiiFSObject() { parent = 0; }
WiiFSObject::~WiiFSObject() { }



void WiiFSObject::unlinkFromParent() {
	if (this->parent == 0)
		return;

	if (this->parent->isDirectory()) {
		WiiDirectory *p = (WiiDirectory *)this->parent;
		if (p->children.contains(this))
			p->children.removeAt(p->children.indexOf(this));
	}

	this->parent = 0;
}

bool WiiFSObject::nameIsEqual(QString check) {
	return (bool)(QString::compare(this->name, check, Qt::CaseInsensitive));
}

bool WiiFSObject::isFile() { return false; }
bool WiiFSObject::isDirectory() { return false; }

/******************************************************************************/

bool WiiFile::isFile() { return true; }

/******************************************************************************/

WiiDirectory::~WiiDirectory() {
	foreach (WiiFSObject *ptr, children)
		delete ptr;
}

bool WiiDirectory::isDirectory() { return true; }

WiiFSObject *WiiDirectory::findByName(QString name, bool recursive) {
	foreach (WiiFSObject *obj, children) {
		if (obj->nameIsEqual(name))
			return obj;

		if (recursive && obj->isDirectory()) {
			WiiDirectory *dir = (WiiDirectory*)obj;
			WiiFSObject *tryThis = dir->findByName(name, recursive);

			if (tryThis)
				return tryThis;
		}
	}

	return 0;
}

WiiFSObject *WiiDirectory::resolvePath(QString path) {
	QStringList pathComponents = path.split('/');
	WiiDirectory *currentDir = this;

	// special case: handle absolute paths
	if (pathComponents.at(0) == "") {
		while (currentDir->parent != 0)
			currentDir = currentDir->parent;

		pathComponents.removeFirst();
	}

	// now we can loop through the path
	while (!pathComponents.isEmpty()) {
		QString next = pathComponents.takeFirst();

		// get the next object in the path
		if (next == ".") {
			next = currentDir;
		} else if (next == "..") {
			next = currentDir->parent;
		} else {
			WiiFSObject *nextObj = currentDir->findByName(next, false);
		}

		if (nextObj == 0) {
			qWarning() << "Failed to resolve path" << path << ": missing component" << next;
			return 0;
		}

		if (pathComponents.isEmpty()) {
			// we've reached the end \o/
			return nextObj;
		}

		// verify that this object is a directory
		if (!nextObj->isDirectory()) {
			qWarning() << "Failed to resolve path" << path << ": component" << next << "is not a directory";
		}

		// ok, this has to be a directory, so let's just continue
		currentDir = (WiiDirectory *)nextObj;
	}

	// we should not reach here
	return 0;
}

bool WiiDirectory::addChild(WiiFSObject *obj) {
	// verify to make sure an object does not exist with this name
	if (this->findByName(obj->name, false) != 0)
		return false;

	obj->unlinkFromParent();

	this->children.append(obj);
	obj->parent = this;

	return true;
}

