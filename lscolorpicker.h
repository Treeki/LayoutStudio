#ifndef LSCOLORPICKER_H
#define LSCOLORPICKER_H

#include <QToolButton>

class LSColorPicker : public QToolButton
{
	Q_OBJECT
public:
	explicit LSColorPicker(QWidget *parent = 0);

	QColor color() const { return m_color; }
	void setColor(QColor newCol);

private:
	QColor m_color;

private slots:
	void tryAndChoose();
	void refreshButton();
	
signals:
	void colorPicked(QColor newCol, QColor oldCol);
	
};

#endif // LSCOLORPICKER_H
