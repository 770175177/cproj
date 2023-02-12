#include "control_dialog.h"
#include "ui_control_dialog.h"

int Control_Dialog::den_flage=0;
int Control_Dialog::den_flage1=0;
int Control_Dialog::den_flage2=0;
int Control_Dialog::den_flage3=0;
int Control_Dialog::feng_flage=0;

Control_Dialog::Control_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Control_Dialog)
{
    ui->setupUi(this);
    ui->frame->setObjectName("wframe");
    ui->frame->setStyleSheet("QFrame#wframe{border-image:url(:/timg);}");
    contrl = new QTcpSocket(this);
    contrl->connectToHost(QHostAddress("10.0.0.100"),2222);
    if(contrl->waitForConnected(3000))
    {
        qDebug()<<"control connect success...";
        connect(contrl,SIGNAL(readyRead()),this,SLOT(read_control()));
    }
    else
    {
        qDebug()<<"control connect fail...";
    }
}

Control_Dialog::~Control_Dialog()
{
    delete ui;
}
void Control_Dialog::read_control()
{
    //读板子反馈，解析消息
    char back_msg[50];
    int ret;
    memset(back_msg,0,sizeof(back_msg));
    ret = contrl->read(back_msg,sizeof(back_msg));
    if(0 > ret)
    {
        qDebug()<<"PC contrl read fail...";
        return;
    }
    qDebug()<<back_msg;
    emit contrl_back(back_msg);
    //qDebug()<<"............";
}
void Control_Dialog::set_contrl(char *word)
{
    memset(msg_contrl,0,sizeof(msg_contrl));
    memcpy(msg_contrl,word,sizeof(msg_contrl));
    qDebug()<<"msg_control is ...."<<msg_contrl;
    /*******************灯****************/
    if(strcmp(msg_contrl,"4 1 on")==0)
    {
        contrl->write("1 1 on",sizeof("1 1 on"));
        den_flage=1;
        qDebug()<<"deng1 on...";
    }
    else if(strcmp(msg_contrl,"4 1 off")==0)
    {
        contrl->write("1 1 off",sizeof("1 1 off"));
        den_flage=0;
        qDebug()<<"deng1 off...";
    }
    if(strcmp(msg_contrl,"4 2 on")==0)
    {
        contrl->write("1 2 on",sizeof("1 2 on"));
        den_flage1=1;
        qDebug()<<"deng2 on...";
    }
    else if(strcmp(msg_contrl,"4 2 off")==0)
    {
        contrl->write("1 2 off",sizeof("1 2 off"));
        den_flage1=0;
        qDebug()<<"deng2 off...";
    }
    if(strcmp(msg_contrl,"4 3 on")==0)
    {
        contrl->write("1 3 on",sizeof("1 3 on"));
        den_flage2=1;
        qDebug()<<"deng3 on...";
    }
    else if(strcmp(msg_contrl,"4 3 off")==0)
    {
        contrl->write("1 3 off",sizeof("1 3 off"));
        den_flage2=0;
        qDebug()<<"deng3 off...";
    }
    if(strcmp(msg_contrl,"4 4 on")==0)
    {
        contrl->write("1 4 on",sizeof("1 4 on"));
        den_flage3=1;
        qDebug()<<"deng4 on...";
    }
    else if(strcmp(msg_contrl,"4 4 off")==0)
    {
        contrl->write("1 4 off",sizeof("1 4 off"));
        den_flage3=0;
        qDebug()<<"deng4 off...";
    }
    /*******************蜂鸣器****************/
    if(strcmp(msg_contrl,"4 5 on")==0)
    {
        contrl->write("2 on",sizeof("2 on"));
        feng_flage=1;
        qDebug()<<"feng on...";
    }
    else if(strcmp(msg_contrl,"4 5 off")==0)
    {
        contrl->write("2 off",sizeof("2 off"));
        feng_flage=0;
        qDebug()<<"feng off...";
    }
    else qDebug()<<"operate error..";
}
void Control_Dialog::closeEvent(QCloseEvent *event)
{
    qDebug()<<"zhuci close.....";
    emit contrl_close();
}
//主机界面按键触发槽函数


void Control_Dialog::on_deng_clicked()
{
    if(0 == den_flage)
    {
        contrl->write("1 1 on",sizeof("1 1 on"));
        den_flage=1;
        qDebug()<<"deng1 on";
    }
    else if(1 == den_flage)
    {
        contrl->write("1 1 off",sizeof("1 1 off"));
        den_flage=0;
        qDebug()<<"deng1 off";
    }
}

void Control_Dialog::on_deng1_clicked()
{
    if(0 == den_flage1)
    {
        contrl->write("1 2 on",sizeof("1 2 on"));
        den_flage1=1;
        qDebug()<<"deng2 on";
    }
    else if(1 == den_flage1)
    {
        contrl->write("1 2 off",sizeof("1 2 off"));
        den_flage1=0;
        qDebug()<<"deng2 off";
    }
}

void Control_Dialog::on_deng2_clicked()
{
    if(0 == den_flage2)
    {
        contrl->write("1 3 on",sizeof("1 3 on"));
        den_flage2=1;
        qDebug()<<"deng3 on";
    }
    else if(1 == den_flage2)
    {
        contrl->write("1 3 off",sizeof("1 3 off"));
        den_flage2=0;
        qDebug()<<"deng3 off";
    }
}

void Control_Dialog::on_deng3_clicked()
{
    if(0 == den_flage3)
    {
        contrl->write("1 4 on",sizeof("1 4 on"));
        den_flage3=1;
        qDebug()<<"deng4 on";
    }
    else if(1 == den_flage3)
    {
        contrl->write("1 4 off",sizeof("1 4 off"));
        den_flage3=0;
        qDebug()<<"deng4 off";
    }
}

void Control_Dialog::on_fen_clicked()
{
    if(0 == feng_flage)
    {
        contrl->write("2 on",sizeof("2 on"));
        feng_flage=1;
        qDebug()<<"feng on";
    }
    else if(1 == feng_flage)
    {
        contrl->write("2 off",sizeof("2 off"));
        feng_flage=0;
        qDebug()<<"feng off";
    }
}
