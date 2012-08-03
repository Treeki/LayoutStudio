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

#ifndef WIIFILESYSTEM_H
#define WIIFILESYSTEM_H

#include "common.h"


class WiiFSObject {
	// abstract base class of all filesystem objects
public:
	virtual ~WiiFSObject();

	WiiFSObject *parent;

	QString name;


	void unlinkFromParent();
	bool nameIsEqual(QString check) const;

	virtual bool isFile() const;
	virtual bool isDirectory() const;


protected:
	WiiFSObject(); // don't instantiate this class directly!
};


/******************************************************************************/

class WiiFile : public WiiFSObject {
public:
	QByteArray data;


	bool isFile() const;
};

/******************************************************************************/

class WiiDirectory : public WiiFSObject {
public:
	~WiiDirectory();

	QList<WiiFSObject *> children;


	bool isDirectory() const;

	WiiFSObject *findByName(QString name, bool recursive) const;
	WiiFSObject *resolvePath(QString path);
	bool addChild(WiiFSObject *obj);
	bool removeChild(WiiFSObject *obj);
};


#endif // FILESYSTEM_H
