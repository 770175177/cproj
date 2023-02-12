#ifndef CAIDAN_DIALOG_H
#define CAIDAN_DIALOG_H

#include <QDialog>
#include <QDebug>
#include <QTcpServer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
//#include <QtWidgets>
#include "control_dialog.h"
#include "camera_dialog.h"

namespace Ui {
class Caidan_Dialog;
}

class Caidan_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Caidan_Dialog(QWidget *parent = 0);
    ~Caidan_Dialog();
signals:
    void back_sig();
protected slots:
    void new_connect();
    void read_user();
    void close_user();
    void control_back(char *back_msg);
protected:
    void zhuci_func();
    void login_func();
    void camera_func();
    void control_func();
private slots:
    void on_dev_clicked();
    void on_caemra_clicked();
    void on_caidan_back_clicked();
    void main_show();
private:
    Ui::Caidan_Dialog *ui;
    QTcpServer *tcplisten;
    QTcpSocket *usersock;
    char buf[100];
    Camera_Dialog *p;
    Control_Dialog *z;
    QSqlDatabase db;
    int num_connect;
};

#endif // CAIDAN_DIALOG_H
