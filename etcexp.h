#ifndef ETCEXP_H
#define ETCEXP_H

#include <QWidget>

namespace Ui {
class EtcExp;
}

class EtcExp : public QWidget
{
	Q_OBJECT

public:
	explicit EtcExp(QWidget *parent = 0);
	~EtcExp();

private:
	Ui::EtcExp *ui;
};

#endif // ETCEXP_H
