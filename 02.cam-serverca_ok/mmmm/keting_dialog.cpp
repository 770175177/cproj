#include "keting_dialog.h"
#include "ui_keting_dialog.h"

Keting_Dialog::Keting_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Keting_Dialog)
{
    ui->setupUi(this);
    ui->dianshi->setIcon(QIcon(":/dians"));
    ui->chufang->setIcon(QIcon(":/chumm"));
    ui->wos->setIcon(QIcon(":/df"));
    ui->frame->setObjectName("wframe");
    ui->frame->setStyleSheet("QFrame#wframe{border-image:url(:/keting);}");//设置背景图片
    ui->dianshi->setFlat(true);
    ui->dianshi->setStyleSheet("QPushButton{color:white; background-color:transparent;}");
    w=new Woshi_Dialog(this);
    connect(w,SIGNAL(back_sig()),this,SLOT(ke_show()));

    chu=new Chufang_Dialog(this);
    connect(chu,SIGNAL(back_sig()),this,SLOT(ke_show()));

    cai=new Caidan_Dialog(this);
    connect(cai,SIGNAL(back_sig()),this,SLOT(ke_show()));
}

Keting_Dialog::~Keting_Dialog()
{
    //this->setWindowFlags(Qt::Dialog);
    //this->showNormal();
    delete w;
    delete chu;
    delete cai;
    delete ui;
}

void Keting_Dialog::on_wos_clicked()
{
    w->show();
    w->setWindowFlags(Qt::Window);
    w->showFullScreen();
    this->hide();
}
void Keting_Dialog::ke_show()
{
    this->show();
}
void Keting_Dialog::on_dianshi_clicked()
{
    static int flage=0;
    if(0 == flage)
    {
        cai->show();
        flage=1;
    }
    else
    {
        cai->hide();
        flage=0;
    }
    //this->hide();
}

void Keting_Dialog::on_ket_back_clicked()
{
    qDebug()<<"keting back...";
    this->hide();
    emit back_sig();
}

void Keting_Dialog::on_chufang_clicked()
{
    chu->show();
    chu->setWindowFlags(Qt::Window);
    chu->showFullScreen();
    this->hide();
}
