#include "mainwindow.h"
#include <QApplication>
#include "parser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    Parser p;
    p.run();



    return a.exec();
}
