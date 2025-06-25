#ifndef TESTPROCESS_H
#define TESTPROCESS_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class TestProcess;
}

class TestProcess : public QWidget
{
    Q_OBJECT

public:
    explicit TestProcess(QWidget *parent = nullptr);
    ~TestProcess();

    //开始动画
    void StartAnimation();
    //停止动画
    void StopAnimation();

private:
    void colorLoop(QColor *);

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::TestProcess *ui;
    QColor m_color[8];
    QPoint m_point[8];
    int m_inRadius;
    int m_outRadius;
    int m_outRadiuslist[8];
    QTimer *m_timer;
    bool m_displayedWhenStopped = true;//停止显示动画的标识，默认为true
};

#endif // TESTPROCESS_H
