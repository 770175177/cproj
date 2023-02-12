#include "caidan_dialog.h"
#include "ui_caidan_dialog.h"

Caidan_Dialog::Caidan_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Caidan_Dialog)
{
    ui->setupUi(this);
    ui->frame->setObjectName("wframe");
    ui->frame->setStyleSheet("QFrame#wframe{border-image:url(:/cai);}");

    num_connect=0;

    z = new Control_Dialog(this);
    connect(z,SIGNAL(contrl_close()),this,SLOT(main_show()));
    connect(z,SIGNAL(contrl_back(char*)),this,SLOT(control_back(char*)));

    p = new Camera_Dialog(this);
    connect(p,SIGNAL(camera_close()),this,SLOT(main_show()));

    tcplisten = new QTcpServer(this);
    if(tcplisten->listen(QHostAddress::AnyIPv4,20000))
    {
        qDebug()<<"listen success...";
        connect(tcplisten,SIGNAL(newConnection()),this,SLOT(new_connect()));
    }
    else
    {
        qDebug()<<"listen fail...";
    }

    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
        db.close();
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");//添加数据库驱动
        db.setDatabaseName("my.db");//设置数据库名称
    }
    if(db.open())//打开数据库
    {
        qDebug() << "open database success";
        QSqlQuery sql_query;
        QString create_sql = "create table if not exists user(username text primary key, password varchar(30));";
        if(!sql_query.exec(create_sql)) //创建表，然后查看创建表是否成功
        {
            qDebug()<<"Table Create failed";
        }
        else
        {
            qDebug()<<"create table success....";
        }
        db.close();
    }
    else
    {
        qDebug() << "open database error";
        db.close();
    }
}

Caidan_Dialog::~Caidan_Dialog()
{
    delete ui;
}
void Caidan_Dialog::new_connect()
{
    qDebug()<<"new connect...";
    usersock = new QTcpSocket(this);
    usersock = tcplisten->nextPendingConnection();
    connect(usersock,SIGNAL(readyRead()),this,SLOT(read_user()));
    connect(usersock,SIGNAL(disconnected()),this,SLOT(close_user()));
    num_connect++;
}
void Caidan_Dialog::read_user()
{
    int ret;
    //char buf[100];
    memset(buf,0,sizeof(buf));
    ret = usersock->read(buf,sizeof(buf));//接收用户发送的请求
    if(ret < 0)
    {
        qDebug()<<"main read error...";
        return;
    }
    qDebug()<<buf;
    //解析消息
    if(strncmp(buf,"1",1)==0)
    {
       login_func();
    }
    if(strncmp(buf,"2",1)==0)
    {
       zhuci_func();
    }
    if(strncmp(buf,"3",1)==0)
    {
       camera_func();
    }
    if(strncmp(buf,"4",1)==0)
    {
       control_func();
    }

}
void Caidan_Dialog::close_user()
{
    qDebug()<<"user sock close...";
    num_connect--;
    //usersock->close();
}
void Caidan_Dialog::zhuci_func()
{
    char *log_msg;
    char msg[100];
    memcpy(msg,buf,sizeof(msg));
    qDebug()<<msg;
    qDebug()<<strtok(msg," ");
    log_msg = strtok(NULL," ");
    qDebug()<<log_msg;
    if(db.open())//打开数据库
    {
        qDebug() << "open database success";
        QSqlQuery query;
        QString username = QString(log_msg);
        log_msg = strtok(NULL," ");
        qDebug()<<log_msg;
        QString password = QString(log_msg);
        QString str=QString("insert into user values ('%1','%2')").arg(username).arg(password);
        qDebug() << str;
        if(query.exec(str))
        {
            qDebug() << "zhuci sucess";
            usersock->write("2 ok",sizeof("2 ok"));
        }
        else
        {
            qDebug() << "zhuci failed";
            usersock->write("2 no",sizeof("2 no"));
        }
        db.close();
    }
    else
    {
        qDebug() << "open database error";
        db.close();
    }

}
void Caidan_Dialog::login_func()
{
    char *log_msg;
    char msg[100];
    memcpy(msg,buf,sizeof(msg));
    strtok(msg," ");
    log_msg = strtok(NULL," ");
    if(db.open())//打开数据库
    {
        qDebug() << "open database success";
        QSqlQuery query;
        QString username = QString(log_msg);
        log_msg = strtok(NULL," ");
        QString password = QString(log_msg);
        QString str=QString("select * from user where username = '%1' and password = '%2'").arg(username).arg(password);
        qDebug() << str;
        if(query.exec(str))
        {
            while(query.next())
            {
                qDebug() << "login sucess";
                username = query.value(0).toString();
                password = query.value(1).toString();
                qDebug()<<QString("name:%1  passwd:%2").arg(username).arg(password);
            }
            usersock->write("1 ok",sizeof("1 ok"));
        }
        else
        {
            qDebug() << "login failed";
            usersock->write("1 no",sizeof("1 no"));
        }
        db.close();
    }
    else
    {
        qDebug() << "open database error";
        db.close();
    }

}
void Caidan_Dialog::camera_func()
{

    qDebug()<< "mobile camera open...";
}
void Caidan_Dialog::control_func()
{
    z->set_contrl(buf);
    qDebug()<< " mobile control open...";
}
void Caidan_Dialog::control_back(char *back_msg)
{
    if(strncmp(back_msg,"1",1)==0)
    {
        if(strcmp(back_msg,"1 on")==0)
        {
            //qDebug()<<"0000000000000000000";
            if(num_connect > 0)
            {
                usersock->write("4 1 on",sizeof("4 1 on"));
            }
        }
        else
        {
            if(num_connect > 0)
            {
                usersock->write("4 1 off",sizeof("4 1 off"));
            }
        }
    }
    else if(strncmp(back_msg,"2",1)==0)
    {
        if(strcmp(back_msg,"2 on")==0)
        {
            if(num_connect > 0)
            {
                usersock->write("4 2 on",sizeof("4 2 on"));
            }
        }
        else
        {
            if(num_connect > 0)
            {
                usersock->write("4 2 off",sizeof("4 2 off"));
            }
        }
    }
    else if(strncmp(back_msg,"3",1)==0)
    {
        if(strcmp(back_msg,"3 on")==0)
        {
            if(num_connect > 0)
            {
                usersock->write("4 3 on",sizeof("4 3 on"));
            }
        }
        else
        {
            if(num_connect > 0)
            {
                usersock->write("4 3 off",sizeof("4 3 off"));
            }
        }
    }
    else if(strncmp(back_msg,"4",1)==0)
    {
        if(strcmp(back_msg,"4 on")==0)
        {
            if(num_connect > 0)
            {
                usersock->write("4 4 on",sizeof("4 4 on"));
            }
        }
        else
        {
            if(num_connect > 0)
            {
                usersock->write("4 4 off",sizeof("4 4 off"));
            }
        }
    }
    else if(strncmp(back_msg,"5",1)==0)
    {
        if(strcmp(back_msg,"5 on")==0)
        {
            if(num_connect > 0)
            {
                usersock->write("4 5 on",sizeof("4 5 on"));
            }
        }
        else
        {
            if(num_connect > 0)
            {
                usersock->write("4 5 off",sizeof("4 5 off"));
            }
        }
    }
}
void Caidan_Dialog::main_show()
{
    this->show();
}
//主机界面按键触发相应槽函数
void Caidan_Dialog::on_dev_clicked()
{
    z->show();
    this->hide();
    qDebug()<< "PC control open...";
}

void Caidan_Dialog::on_caemra_clicked()
{
    p->show();
    this->hide();
    qDebug()<< "PC camera open...";
}

void Caidan_Dialog::on_caidan_back_clicked()
{
    qDebug()<<"caidan back...";
    this->hide();
    //emit back_sig();
}
