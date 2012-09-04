#include "lscolorpicker.h"
#include <QColorDialog>

LSColorPicker::LSColorPicker(QWidget *parent) :
	QToolButton(parent) {

	connect(this, SIGNAL(clicked()), SLOT(tryAndChoose()));
	setColor(QColor::fromRgb(255,255,255,255));
}

void LSColorPicker::setColor(QColor newCol) {
	m_color = newCol;
	refreshButton();
}

void LSColorPicker::tryAndChoose() {
	QColor newCol = QColorDialog::getColor(
				m_color, this,
				"Pick a Colour",
				QColorDialog::ShowAlphaChannel);

	if (newCol.isValid() && newCol != m_color) {
		QColor oldCol = m_color;
		m_color = newCol;
		refreshButton();
		emit colorPicked(newCol, oldCol);
	}
}


void LSColorPicker::refreshButton() {
	setStyleSheet(QString("QToolButton { background-color: rgba(%1,%2,%3,%4); }")
				  .arg(m_color.red())
				  .arg(m_color.green())
				  .arg(m_color.blue())
				  .arg(m_color.alpha()));

	setText(QString("RGBA (#%5%6%7%8)\n%1,%2,%3,%4")
			.arg(m_color.red())
			.arg(m_color.green())
			.arg(m_color.blue())
			.arg(m_color.alpha())
			.arg(m_color.red(), 2, 16, (QChar)'0')
			.arg(m_color.green(), 2, 16, (QChar)'0')
			.arg(m_color.blue(), 2, 16, (QChar)'0')
			.arg(m_color.alpha(), 2, 16, (QChar)'0'));
}
