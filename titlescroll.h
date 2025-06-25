#ifndef TITLESCROLL_H
#define TITLESCROLL_H

#include <QWidget>
#include <QLabel>

class TitleScroll : public QLabel
{
    Q_OBJECT
public:
    TitleScroll(QWidget *parent = nullptr);
    ~TitleScroll();

protected:
    void paintEvent(QPaintEvent *);
    void updateIndex();

private:
    //字符串宽度
    int m_charWidth;
    int m_curIndex;
    //需要显示的字符串
    QString m_showText;

public:
    QString getText()
    {
         return  m_showText;
    }
};

#endif // TITLESCROLL_H
