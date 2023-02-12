#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QRect>
#include <QDebug>
#include <QSize>
#include "keting_dialog.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_men_clicked();
    void quan_show();
    void on_exit_clicked();

    void on_frame_destroyed();

private:
    Ui::Widget *ui;
    Keting_Dialog *k;
};

#endif // WIDGET_H
