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
