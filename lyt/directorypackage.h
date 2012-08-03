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

// Currently unmaintained.
// What this needs to be fixed up:
// -- Implement rename()
// -- Make it emit signals
// -- Add support to the LayoutStudio UI

#ifndef LYTDIRECTORYPACKAGE_H
#define LYTDIRECTORYPACKAGE_H

#include "packagebase.h"

class LYTDirectoryPackage : public LYTPackageBase {
	Q_OBJECT
public:
	LYTDirectoryPackage(QString path, QObject *parent = 0);

	QStringList list(ItemType type) const;
	QByteArray get(ItemType type, const QString &name) const;
	bool write(ItemType type, const QString &name, const QByteArray &data);
	bool remove(ItemType type, const QString &name);

	bool needsExplicitSave() const { return false; }
	bool savePackage();
	QString description() const;
	QString path() const;

protected:
	QString m_path;
};

#endif // LYTDIRECTORYPACKAGE_H
