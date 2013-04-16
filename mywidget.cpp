#include "mywidget.h"
#include "ui_mywidget.h"



MyWidget::MyWidget()
{


    m_button = new QPushButton("My Button", this);
        // set size and location of the button
        m_button->setGeometry(QRect(QPoint(500, 500),
                                     QSize(200, 50)));
        connect(m_button, SIGNAL(released()), this, SLOT(handleButton()));
        m_button->setText("Open FIle");
    QPalette palette(MyWidget::palette());
    palette.setColor(backgroundRole(), Qt::white);
    setPalette(palette);

}

void MyWidget::paintEvent(QPaintEvent *)
{
    for (int u = 0; u<1500; u++)
        fff[u] = rand() % 8;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::darkGreen);
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

         painter.fillRect(50+i*6,50+j*6,5,5,c);
        }
    }

}

void MyWidget::handleButton()
{

    path = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.*)"));

}

MyWidget::~MyWidget()
{
    delete ui;
}


