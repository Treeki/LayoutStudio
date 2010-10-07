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

#include "lsglobals.h"

bool LSGlobals::m_loaded = false;
QHash<QString, QIcon> LSGlobals::m_icons;
QHash<QString, QString> LSGlobals::m_pane_icon_names;


bool LSGlobals::setup() {
	if (m_loaded)
		return false;



	// Load icons
	loadIcon("pane");
	loadIcon("picture");
	loadIcon("textbox");
	loadIcon("bounding");
	loadIcon("window");

	// Set up pane type mapping
	m_pane_icon_names["pan1"] = "pane";
	m_pane_icon_names["pic1"] = "picture";
	m_pane_icon_names["txt1"] = "textbox";
	m_pane_icon_names["bnd1"] = "bounding";
	m_pane_icon_names["wnd1"] = "window";



	m_loaded = true;
	return true;
}


bool LSGlobals::loadIcon(QString name) {
	m_icons[name] = QIcon(QString(":icons/%1").arg(name));
	return true;
}


QIcon LSGlobals::getIcon(QString name) {
	return m_icons.value(name);
}

QIcon LSGlobals::getIconForPaneType(QString type) {
	return m_icons.value(m_pane_icon_names.value(type));
}
