#include "titlescroll.h"
#include <QPainter>
#include <QTimer>

TitleScroll::TitleScroll(QWidget *parent)
    : QLabel(parent)
{
//    setMinimumWidth(200);
//    setMinimumHeight(40);

    // 当前文字下标值
    m_curIndex = 0;

    // 显示的文字
    m_showText = tr("Database compatibility testing tool by Xu and ui design help by youyou.");

    // 每个字符的宽度
    m_charWidth = fontMetrics().width("a");

    // 定时器，定时滚动
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TitleScroll::updateIndex);
    timer->start(100);
}


TitleScroll::~TitleScroll()
{

}


void TitleScroll::paintEvent(QPaintEvent *)
{
    QPen pen;
    pen.setColor(QColor(0,0,0));

    pen.setStyle(Qt::DashDotDotLine);

    QFont font("楷体",14,QFont::Bold);

    QPainter painter(this);
    painter.setPen(pen);

    painter.setFont(font);
    painter.drawText(0, 15, m_showText.mid(m_curIndex));
    painter.drawText(width() - m_charWidth*m_curIndex, 15, m_showText.left(m_curIndex));
}


void TitleScroll::updateIndex()
{
    update();
    m_curIndex++;
    if (m_curIndex*m_charWidth > width())
        m_curIndex = 0;
}
