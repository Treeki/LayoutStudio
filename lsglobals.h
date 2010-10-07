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

#ifndef LSGLOBALS_H
#define LSGLOBALS_H

#include <QtGui/QIcon>
#include <QtCore/QHash>

class LSGlobals {
public:
	static bool setup();

	static QIcon getIcon(QString name);
	static QIcon getIconForPaneType(QString type);

private:
	static bool m_loaded;

	static QHash<QString, QIcon> m_icons;
	static QHash<QString, QString> m_pane_icon_names;

	static bool loadIcon(QString name);
};

#endif // LSGLOBALS_H
