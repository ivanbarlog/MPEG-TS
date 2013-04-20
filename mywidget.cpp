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
    map_bg->setGeometry(50,50,600,300);
    scene = new QGraphicsScene(map_bg);

    map_bg->setBackgroundBrush(QBrush(Qt::white, Qt::SolidPattern));
    map_bg->setScene(scene);
    map_bg->scale(1,-1);


}

void MyWidget::paint()
{
    u_int16_t pid[13];

    int r = 0;
    int k = 80;
    int q = 0, nr = 0;
    int s = packets.size();
    scene->setSceneRect(0,0,600,(s/80)*6+100);
    for(int j = 0; j < (s / 80); j++)
    {
        k = 80;
        if(j+1 == (s/80))
            k = s % 80;
        for(int i = 0; i < k; i++)
        {


            bool present = false;
            for (int w = 0; w < 13; w++)
                if(pid[w] == packets[q].pid)
                {
                    present = true;
                    r = w;
                    break;
                }
            if(!present && nr!=12)
            {
                pid[nr] = packets[q].pid;
                r = nr++;
            }

            switch(r)
            {
                case 0: c = Qt::magenta; break;
                case 1: c = Qt::green; break;
                case 2: c = Qt::red; break;
                case 3: c = Qt::blue; break;
                case 4: c = Qt::darkBlue; break;
                case 5: c = Qt::yellow; break;
                case 6: c = Qt::darkCyan; break;
                case 7: c = Qt::lightGray; break;
                case 8: c = Qt::darkGray; break;
                case 9: c = Qt::darkGreen; break;
                case 10: c = Qt::darkMagenta; break;
                case 11: c = Qt::darkRed; break;
                case 12: c = Qt::darkYellow; break;


                default: c = Qt::white; break;
            }
            present = false;
            q++;

            QGraphicsRectItem *r = scene->addRect(50+i*6,((s/80)*6+100)-(50+j*6),5,5,QPen(Qt::black),QBrush(c, Qt::SolidPattern));
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


