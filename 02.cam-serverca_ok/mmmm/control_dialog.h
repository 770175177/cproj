#ifndef CONTROL_DIALOG_H
#define CONTROL_DIALOG_H

#include <QDialog>
#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QCloseEvent>

namespace Ui {
class Control_Dialog;
}

class Control_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Control_Dialog(QWidget *parent = 0);
    ~Control_Dialog();
signals:
    void contrl_close();
    void contrl_back(char *bmsg);
public:
    void set_contrl(char *word);
protected:
     virtual void closeEvent(QCloseEvent *event);
protected slots:
    void read_control();
private slots:
    void on_deng_clicked();
    void on_deng1_clicked();
    void on_deng2_clicked();
    void on_deng3_clicked();
    void on_fen_clicked();
private:
    Ui::Control_Dialog *ui;
    QTcpSocket *contrl;
    char msg_contrl[100];
public:
    static int den_flage;
    static int den_flage1;
    static int den_flage2;
    static int den_flage3;
    static int feng_flage;
};

#endif // CONTROL_DIALOG_H
