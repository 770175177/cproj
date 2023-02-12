#include "woshi_dialog.h"
#include "ui_woshi_dialog.h"

Woshi_Dialog::Woshi_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Woshi_Dialog)
{
    ui->setupUi(this);
    //ui->deng->setIcon(QIcon(":/den"));
    ui->frame->setObjectName("wframe");
    ui->frame->setStyleSheet("QFrame#wframe{border-image:url(:/wosh2);}");
}

Woshi_Dialog::~Woshi_Dialog()
{
    delete ui;
}

void Woshi_Dialog::on_deng_clicked()
{
    static int flage=0;
    if(0 == flage)
    {
        ui->frame->setStyleSheet("QFrame#wframe{border-image:url(:/wos);}");
        flage=1;
    }
    else
    {
        ui->frame->setStyleSheet("QFrame#wframe{border-image:url(:/wosh2);}");
        flage=0;
    }
}

void Woshi_Dialog::on_wos_back_clicked()
{
    qDebug()<<"wos back...";
    this->hide();
    emit back_sig();
}
