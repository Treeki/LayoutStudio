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


class LYTPackageBase {
public:
	LYTPackageBase();
	virtual ~LYTPackageBase();

	virtual QStringList listAnims() const = 0;
	virtual QStringList listLayouts() const = 0;
	virtual QStringList listTextures() const = 0;
	virtual QStringList listFonts() const = 0;

	virtual QByteArray getAnim(QString name) const = 0;
	virtual QByteArray getLayout(QString name) const = 0;
	virtual QByteArray getTexture(QString name) const = 0;
	virtual QByteArray getFont(QString name) const = 0;

	virtual bool writeAnim(QString name, QByteArray data) = 0;
	virtual bool writeLayout(QString name, QByteArray data) = 0;
	virtual bool writeTexture(QString name, QByteArray data) = 0;
	virtual bool writeFont(QString name, QByteArray data) = 0;

	virtual bool savePackage() = 0;
	virtual QString description() const = 0;
};

#endif // LYTPACKAGEBASE_H
