#ifndef WOSHI_DIALOG_H
#define WOSHI_DIALOG_H

#include <QDialog>
#include <QDebug>
#include <QIcon>

namespace Ui {
class Woshi_Dialog;
}

class Woshi_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Woshi_Dialog(QWidget *parent = 0);
    ~Woshi_Dialog();
signals:
    void back_sig();
private slots:
    void on_deng_clicked();

    void on_wos_back_clicked();

private:
    Ui::Woshi_Dialog *ui;
};

#endif // WOSHI_DIALOG_H
