#ifndef KETING_DIALOG_H
#define KETING_DIALOG_H

#include <QDialog>
#include "woshi_dialog.h"
#include "chufang_dialog.h"
#include "caidan_dialog.h"
namespace Ui {
class Keting_Dialog;
}

class Keting_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Keting_Dialog(QWidget *parent = 0);
    ~Keting_Dialog();
signals:
    void back_sig();
private slots:
    void on_wos_clicked();
    void ke_show();
    void on_dianshi_clicked();

    void on_ket_back_clicked();

    void on_chufang_clicked();

private:
    Ui::Keting_Dialog *ui;
    Woshi_Dialog *w;
    Chufang_Dialog *chu;
    Caidan_Dialog *cai;
};

#endif // KETING_DIALOG_H
