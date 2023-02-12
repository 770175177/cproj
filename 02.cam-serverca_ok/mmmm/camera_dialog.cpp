#include "camera_dialog.h"
#include "ui_camera_dialog.h"

unsigned int Camera_Dialog::num_connect=0;

Camera_Dialog::Camera_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Camera_Dialog)
{
    ui->setupUi(this);
    camera = new QTcpSocket(this);
    camera->connectToHost(QHostAddress("169.254.32.5"),8888);
    if(camera->waitForConnected(3000))
    {
        qDebug()<<"camera connect success...";
        connect(camera,SIGNAL(readyRead()),this,SLOT(show_camera()));
    }
    else
    {
        qDebug()<<"camera connect fail...";
    }
    mobile_listen=new QTcpServer(this);
    if(mobile_listen->listen(QHostAddress("192.168.191.1"),10000))
    {
        qDebug()<<"mobile camera listen....";
        connect(mobile_listen,SIGNAL(newConnection()),this,SLOT(new_connect()));
    }
    else
    {
        qDebug()<<"mobile camera listen fail....";
    }
}

Camera_Dialog::~Camera_Dialog()
{
    qDebug()<<"camera dialog close...";
    delete ui;
}
void Camera_Dialog::show_camera()
{
    static int camera_flag=0;
    if(num_connect == 0)
        camera_flag=0;
    static int flag = 0;
    static int pic_size = 0;
    char size[10] = {0};
    char buff[320*480*3];
    if( 0 == flag )
    {
        camera->read(size,10);
        pic_size = atoi(size);

        if(num_connect > 0)
        {
            if(camera_flag == 0)
            {
                int ret = moblie_camera->write(size,10);
                if(ret < 0)
                {
                    qDebug()<<"moblie write size fail...";
                }
                camera_flag=1;
                qDebug()<<ret;
            }
        }

        //qDebug()<<"size ok....";
        flag = 1;
        return;
    }
    else
    {
        if(camera->bytesAvailable() > pic_size)
        {
            camera->read(buff,pic_size);
            QPixmap picture;
            picture.loadFromData((const uchar *)buff,pic_size);
            ui->label->setAutoFillBackground(true);
            ui->label->setScaledContents(true);
            ui->label->setPixmap(picture);
            ui->label->show();

            if(num_connect > 0)
            {
                if(camera_flag == 1)
                {
                    int ret = moblie_camera->write(buff,pic_size);
                    if(ret < 0)
                    {
                        qDebug()<<"mobile write data fail....";
                    }
                    camera_flag=0;
                    qDebug()<<ret;
                }
            }

            //qDebug()<<"data ok ....";
            flag = 0;
        }
        else
        {
            return;
        }
    }
}
void Camera_Dialog::closeEvent(QCloseEvent *event)
{
    this->hide();
    emit camera_close();
    qDebug()<<"camera close.....";
}
void Camera_Dialog::new_connect()
{
    qDebug()<<"new camera connect...";
    moblie_camera=new QTcpSocket(this);
    moblie_camera=mobile_listen->nextPendingConnection();
    connect(moblie_camera,SIGNAL(readyRead()),this,SLOT(read_camera()));
    connect(moblie_camera,SIGNAL(disconnected()),this,SLOT(close_camera()));
    num_connect++;
}
void Camera_Dialog::read_camera()
{

}
void Camera_Dialog::close_camera()
{
    qDebug()<<"mobile camera close...";
    num_connect--;
}
