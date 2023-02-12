#ifndef CAMERA_DIALOG_H
#define CAMERA_DIALOG_H

#include <QDialog>
#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>
#include <QDebug>
#include <QCloseEvent>
#include <QPixmap>
#include <QIcon>

namespace Ui {
class Camera_Dialog;
}

class Camera_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Camera_Dialog(QWidget *parent = 0);
    ~Camera_Dialog();
signals:
    void camera_close();
protected:
     virtual void closeEvent(QCloseEvent *event);
protected slots:
    void show_camera();
    void new_connect();
    void read_camera();
    void close_camera();
private:
    Ui::Camera_Dialog *ui;
    QTcpSocket *camera;
    QTcpServer *mobile_listen;
    QTcpSocket *moblie_camera;
    static unsigned int num_connect;
};

#endif // CAMERA_DIALOG_H
