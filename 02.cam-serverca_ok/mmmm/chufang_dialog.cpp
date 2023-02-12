#include "chufang_dialog.h"
#include "ui_chufang_dialog.h"

Chufang_Dialog::Chufang_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Chufang_Dialog)
{
    ui->setupUi(this);
    ui->frame->setObjectName("wframe");
    ui->frame->setStyleSheet("QFrame#wframe{border-image:url(:/chufa);}");
}

Chufang_Dialog::~Chufang_Dialog()
{
    delete ui;
}

void Chufang_Dialog::on_chu_back_clicked()
{
    qDebug()<<"chufang back...";
    this->hide();
    emit back_sig();
}
