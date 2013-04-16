#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QtGui>
#include <vector>
#include "types.h"
#include <stdlib.h>
#include <QPushButton>
#include <QFileDialog>

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget();
    ~MyWidget();
    int fff[1500];
    QString path;

private slots:
    void handleButton();

protected:
    void paintEvent(QPaintEvent *);
    QColor c;

private:
    Ui::MyWidget *ui;
    QPushButton *m_button;
    std::vector<PacketInfo> packets;
};

#endif // MYWIDGET_H
