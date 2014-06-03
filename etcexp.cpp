#include "etcexp.h"
#include "ui_etcexp.h"

EtcExp::EtcExp(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::EtcExp)
{
	ui->setupUi(this);
}

EtcExp::~EtcExp()
{
	delete ui;
}
