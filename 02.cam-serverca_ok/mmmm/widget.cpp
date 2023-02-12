#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //ui->men->setVisible(false);
    ui->men->setIcon(QIcon(":/men"));
    ui->frame->setObjectName("wframe");
    ui->frame->setStyleSheet("QFrame#wframe{border-image:url(:/quanjing);}");//设置背景图片
    ui->exit->setFlat(true);
    ui->exit->setStyleSheet("QPushButton{color:white; background-color:transparent;}");
    this->showFullScreen();
    k=new Keting_Dialog(this);
    connect(k,SIGNAL(back_sig()),this,SLOT(quan_show()));
}

Widget::~Widget()
{
    delete k;
    delete ui;
}

void Widget::on_men_clicked()
{
    k->show();
    k->setWindowFlags(Qt::Window);
    k->showFullScreen();
    this->hide();
}
void Widget::quan_show()
{
    this->show();
}

void Widget::on_exit_clicked()
{
    this->close();
    disconnect(k,SIGNAL(back_sig()),this,SLOT(quan_show()));
}

void Widget::on_frame_destroyed()
{

}
