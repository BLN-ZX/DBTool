#ifndef PROCESSING_H
#define PROCESSING_H

#include <QWidget>
#include <QThread>
#include <QTime>

#include "random.h"
#include "sql.h"

namespace Ui
{
    class Processing;
}

class Processing : public QWidget
{
    Q_OBJECT

public:
    explicit Processing(QWidget *parent = nullptr);
    ~Processing();

    QString mysqlHostName;
    QString mysqlPort;
    QString mysqlUserName;
    QString mysqlPassWord;

    QString sqlSerHostName;
    QString sqlSerDatabaseName;
    QString sqlSerUserName;
    QString sqlSerPassword;

    mysql_conn *m;
    sqlServer_conn *s;

signals:
    // 返回连接信息到主线程，无论是否成功
    void sendConn(mysql_conn *m_arg, sqlServer_conn *s_arg, QString mess);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_pressed();

    void on_pushButton_2_released();

    void on_pushButton_3_pressed();

    void on_pushButton_3_released();

private:
    Ui::Processing *ui;
};

#endif // PROCESSING_H
