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
