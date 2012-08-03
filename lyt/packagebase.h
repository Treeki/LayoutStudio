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

#ifndef LYTPACKAGEBASE_H
#define LYTPACKAGEBASE_H

#include <QStringList>
#include <QByteArray>
#include <QObject>


class LYTPackageBase : public QObject {
	Q_OBJECT
public:
	LYTPackageBase(QObject *parent = 0);
	virtual ~LYTPackageBase();

	enum ItemType {
		Layout = 1,
		Animation,
		Texture,
		Font
	};

	static QString defaultPathForItemType(ItemType type, bool withArc=false);
	static QByteArray createSkeletonItem(ItemType type);

	virtual QStringList list(ItemType type) const = 0;
	virtual QByteArray get(ItemType type, const QString &name) const = 0;
	virtual bool write(ItemType type, const QString &name, const QByteArray &data) = 0;
	virtual bool rename(ItemType type, const QString &from, const QString &to) = 0;
	virtual bool remove(ItemType type, const QString &name) = 0;

	// Shortcuts
#define MakeThing(THING, ENUMVAL) \
	QStringList list##THING##s () const { return list(ENUMVAL); } \
	QByteArray get##THING (const QString &name) const \
	{ return get(ENUMVAL, name); } \
	\
	bool write##THING (const QString &name, const QByteArray &data) \
	{ return write(ENUMVAL, name, data); } \
	\
	bool remove##THING(const QString &name) { return remove(ENUMVAL, name); }

	// Use it
	MakeThing(Layout, Layout)
	MakeThing(Anim, Animation)
	MakeThing(Texture, Texture)
	MakeThing(Font, Font)

#undef MakeThing

	virtual bool needsExplicitSave() const = 0;
	virtual bool savePackage() = 0;
	virtual QString description() const = 0;

signals:
	void aboutToAddFile(LYTPackageBase::ItemType type, QString name);
	void aboutToRemoveFile(LYTPackageBase::ItemType type, QString name);
	void aboutToRenameFile(LYTPackageBase::ItemType type, QString from, QString to);
	void aboutToModifyFile(LYTPackageBase::ItemType type, QString name);

	void fileWasAdded(LYTPackageBase::ItemType type, QString name);
	void fileWasRemoved(LYTPackageBase::ItemType type, QString name);
	void fileWasRenamed(LYTPackageBase::ItemType type, QString from, QString to);
	void fileWasModified(LYTPackageBase::ItemType type, QString name);
};

#endif // LYTPACKAGEBASE_H
