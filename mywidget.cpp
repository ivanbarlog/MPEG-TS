#include "mywidget.h"
#include "ui_mywidget.h"

#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <QPointF>


MyWidget::MyWidget()
{



    qp.color().black();
    qb.color().light();
    m_button = new QPushButton("My Button", this);
    m_button->setGeometry(QRect(QPoint(500, 600),
    QSize(200, 50)));
    connect(m_button, SIGNAL(released()), this, SLOT(handleButton()));
    m_button->setText("Open File");

    map_bg = new QGraphicsView(this);
    map_bg->setGeometry(20,20,1100,500);
    scene = new Scene();

    map_bg->setBackgroundBrush(QBrush(Qt::white, Qt::SolidPattern));
    map_bg->setScene(scene);
    map_bg->scale(1,1);
    cb = new QCheckBox("1fff",this);
    cb->setGeometry(1150,200,100,20);
    cb->setChecked(true);
    cb->setText("1fff");
    cboxes.push_back(cb);
    cb = new QCheckBox("0000",this);
    cb->setGeometry(1150,250,100,20);
    cb->setChecked(true);
    cb->setText("0000");
    cboxes.push_back(cb);
    cb = new QCheckBox("other",this);
    cb->setGeometry(1150,300,100,20);
    cb->setChecked(true);
    cb->setText("other");
    cboxes.push_back(cb);

    r_button = new QPushButton("Refresh", this);
    r_button->setGeometry(QRect(QPoint(1150, 400),
    QSize(100, 50)));
    connect(r_button, SIGNAL(released()), this, SLOT(handleButton2()));
    r_button->setText("Refresh");





}

void MyWidget::paint()
{

    currentlist.clear();
    int r = 0;
    int k = 100;
    int q = 0, nr = 0;
    int off = 26, voff = 40;
    QString st;

    for(int ff = 0; ff < 10; ff++)
    {

        io = new QGraphicsTextItem;
        io->setPos(60+103*ff ,20);
        st = QString("%1 - %2").arg(ff*10).arg(ff*10+9);
        io->setPlainText(st);
        scene->addItem(io);
    }



   // scene->addText("trololo",)
    for(int j = 0; j <= (s / 100); j++)
    {
        io = new QGraphicsTextItem;
        io->setPos(0,70+j*110);
        st = QString("%1\n -\n%2k\n").arg(j).arg(j+1);
        io->setPlainText(st);
        scene->addItem(io);
        if(j % 10 == 0) voff+=10;



        off =25;
        k = 100;
        if(j == (s/100))
            k = s % 100;
        for(int i = 0; i < k; i++)
        {


            bool present = false;
            for (int w = 0; w < 20; w++)
                if(pid[w] == packets[q].pid)
                {
                    present = true;
                    r = w;
                    break;
                }
            if(!present && nr!=19)
            {
                pid[nr] = packets[q].pid;
                r = nr++;


            }
            present = false;
            q++;

            if((cboxes[0]->isChecked() == false)&& pid[r] == 8191)
            continue;
            if(!cboxes[1]->isChecked() && pid[r] == 0)
            continue;
            if(!cboxes[2]->isChecked() && (pid[nr] != 8191 && pid[nr] != 0))
            continue;


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


                default: c = Qt::black; break;
            }
            if(i % 10 == 0) off+=4;

            currentlist.push_back(q-1);
            scene->addRect(off + i*10,voff + j*10,9,9,QPen(Qt::white),QBrush(c, Qt::SolidPattern));
        //scene->mousePressEvent();

        }
    }

}


void MyWidget::handleButton()
{

    path = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));
    Parser p;
    packets = p.getPacketList(path);
    s = packets.size();
    scene->setSceneRect(0,0,1050,(s/100)*11+100);
    scene->s = s;
    scene->packets = packets;
    paint();
    scene->currentlist = currentlist;

    /* usage */
    QHash<uint16_t, Program> programInfo;
    programInfo = p.getProgramInfo();

    Program prgrm;
    prgrm = programInfo[0x0020];

    std::vector<PMTProgram> v = prgrm.pmt.programs;
    PMTProgram asdf = v.at(0);

    //globalna premenna
    StreamType *streamTypes = new StreamType();
    QString typ = streamTypes->getStreamType(asdf.streamType);

    std::string text = typ.toUtf8().constData();
    std::cout << "Typ: " << text << std::endl;

}

void MyWidget::handleButton2()
{
    scene->clear();
    paint();
    scene->currentlist = currentlist;
}

MyWidget::~MyWidget()
{
    delete ui;
}


