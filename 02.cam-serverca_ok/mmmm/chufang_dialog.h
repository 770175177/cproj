#ifndef CHUFANG_DIALOG_H
#define CHUFANG_DIALOG_H

#include <QDialog>
#include <QDebug>
namespace Ui {
class Chufang_Dialog;
}

class Chufang_Dialog : public QDialog
{
    Q_OBJECT
signals:
    void back_sig();
public:
    explicit Chufang_Dialog(QWidget *parent = 0);
    ~Chufang_Dialog();

private slots:
    void on_chu_back_clicked();

private:
    Ui::Chufang_Dialog *ui;
};

#endif // CHUFANG_DIALOG_H
