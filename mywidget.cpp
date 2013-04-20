#include "mywidget.h"
#include "ui_mywidget.h"

#include "parser.h"



MyWidget::MyWidget()
{



    qp.color().black();
    qb.color().light();
    m_button = new QPushButton("My Button", this);
    m_button->setGeometry(QRect(QPoint(500, 500),
    QSize(200, 50)));
    connect(m_button, SIGNAL(released()), this, SLOT(handleButton()));
    m_button->setText("Open File");

    map_bg = new QGraphicsView(this);
    map_bg->setGeometry(50,50,200,200);
    scene = new QGraphicsScene(map_bg);
    scene->setSceneRect(50,50,200,200);
    map_bg->setBackgroundBrush(QBrush(Qt::white, Qt::SolidPattern));
    map_bg->setScene(scene);
    map_bg->scale(1,-1);


}

void MyWidget::paint()
{
    for (int u = 0; u<1500; u++)
        fff[u] = rand() % 8;

    int r = 0;
    int k = 0;
    for(int j = 0; j < 30; j++)
    {
        for(int i = 0; i < 50; i++)
        {
            k = fff[r];
        switch(k)
        {
        case 0: c = Qt::magenta; break;
        case 1: c = Qt::green; break;
        case 2: c = Qt::red; break;
        case 3: c = Qt::blue; break;
        case 4: c = Qt::black; break;
        case 5: c = Qt::yellow; break;
        case 6: c = Qt::darkCyan; break;
        case 7: c = Qt::lightGray; break;
        default: c = Qt::black; break;
        }
        r++;

        QGraphicsRectItem *r = scene->addRect(50+i*6,50+j*6,5,5,QPen(Qt::black),QBrush(c, Qt::SolidPattern));
        //scene->mousePressEvent();
        rec.push_back(r);
        }
    }

}

void MyWidget::handleButton()
{

    path = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));
    Parser p;
    packets = p.getPacketsInfo(path);
    paint();
}

MyWidget::~MyWidget()
{
    delete ui;
}


