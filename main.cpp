#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <vector>
#include "parser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    std::vector<PacketInfo> packets;
    QString fname = "/home/dev/public/ts-files/football.ts";

    Parser p;
    packets = p.getPacketsInfo(fname);



    return a.exec();
}
