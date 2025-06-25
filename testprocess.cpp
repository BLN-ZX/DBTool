#include "testprocess.h"
#include "ui_testprocess.h"

#include <QPainter>
#include <QLabel>
#include <QDebug>

TestProcess::TestProcess(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestProcess)
{
    ui->setupUi(this);

    // 设置模态
    this->setWindowModality(Qt::ApplicationModal);

    // 设置标题
    setWindowTitle("Please wait while testing . . .");

    m_inRadius = 30;
    m_outRadius = 5;
    //效果1
//    m_color[0] = QColor(255,140,0);
//    m_color[1] = QColor(255,140,0);
//    m_color[2] = QColor(255,140,0);
//    m_color[3] = QColor(255,140,0);
//    m_color[4] = QColor(34,139,34);
//    m_color[5] = QColor(34,139,34);
//    m_color[6] = QColor(34,139,34);
//    m_color[7] = QColor(34,139,34);

//    m_outRadiuslist[0] = 8;
//    m_outRadiuslist[1] = 8;
//    m_outRadiuslist[2] = 8;
//    m_outRadiuslist[3] = 8;
//    m_outRadiuslist[4] = 8;
//    m_outRadiuslist[5] = 8;
//    m_outRadiuslist[6] = 8;
//    m_outRadiuslist[7] = 10;

    //效果2
    m_color[0] = QColor(9,126,186);
    m_color[1] = QColor(9,126,186);
    m_color[2] = QColor(9,126,186);
    m_color[3] = QColor(9,126,186);
    m_color[4] = QColor(9,126,186);
    m_color[5] = QColor(9,126,186);
    m_color[6] = QColor(9,126,186);
    m_color[7] = QColor(9,126,186);

    m_outRadiuslist[0] = 1;
    m_outRadiuslist[1] = 2;
    m_outRadiuslist[2] = 3;
    m_outRadiuslist[3] = 4;
    m_outRadiuslist[4] = 5;
    m_outRadiuslist[5] = 6;
    m_outRadiuslist[6] = 7;
    m_outRadiuslist[7] = 8;


    m_point[0] = QPoint(this->width()/2-m_outRadius-m_inRadius,
                        this->height()/2-m_outRadius);//左
    m_point[1] = QPoint(this->width()/2-m_outRadius-m_inRadius*2/3,
                        this->height()/2-m_outRadius-m_inRadius*2/3);//左上
    m_point[2] = QPoint(this->width()/2-m_outRadius,
                        this->height()/2-m_outRadius-m_inRadius);//上
    m_point[3] = QPoint(this->width()/2-m_outRadius+m_inRadius*2/3,
                        this->height()/2-m_outRadius-m_inRadius*2/3);//右上
    m_point[4] = QPoint(this->width()/2-m_outRadius+m_inRadius,
                        this->height()/2-m_outRadius);//右
    m_point[5] = QPoint(this->width()/2-m_outRadius+m_inRadius*2/3,
                        this->height()/2-m_outRadius+m_inRadius*2/3);//右下
    m_point[6] = QPoint(this->width()/2-m_outRadius,
                        this->height()/2-m_outRadius+m_inRadius);//下
    m_point[7] = QPoint(this->width()/2-m_outRadius-m_inRadius*2/3,
                        this->height()/2-m_outRadius+m_inRadius*2/3);//左下

    m_timer = new QTimer(this);
    connect(m_timer,&QTimer::timeout,this,[=]()
    {
        colorLoop(m_color);
        update();
    });
}

TestProcess::~TestProcess()
{
    delete ui;
}


void TestProcess::StartAnimation()
{
    qDebug()<<"开始动画";
    m_displayedWhenStopped= false;
    m_timer->start(100);
}

void TestProcess::StopAnimation()
{
    qDebug()<<"停止动画";
    m_timer->stop();
    m_displayedWhenStopped= true;
    update();
}

void TestProcess::colorLoop(QColor *color)
{
    QColor tmp = m_color[7];
    int tmpInt = m_outRadiuslist[7];
    for (int var = 7; var > 0; --var) {
       m_color[var] = m_color[var-1];
       m_outRadiuslist[var] = m_outRadiuslist[var-1];
    }
    m_color[0] = tmp;
    m_outRadiuslist[0] = tmpInt;
}

void TestProcess::paintEvent(QPaintEvent *event)
{
    //创建画笔
    QPainter painter(this);
    //抗锯齿和使用平滑转换算法
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    for (int var = 0; var < 8; ++var)
    {
        QPen pen(m_color[var]);//定义画笔
        pen.setWidth(1);//
        pen.setStyle(Qt::CustomDashLine);
        painter.setPen(pen);
        painter.setBrush(QBrush(m_color[var], Qt::SolidPattern));
        painter.drawEllipse(m_point[var],m_outRadiuslist[var],m_outRadiuslist[var]);
    }
}
