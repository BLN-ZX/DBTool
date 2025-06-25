#ifndef DATATYPETEST_H
#define DATATYPETEST_H

#include <QObject>
#include <QThread>
#include <QSqlDatabase>
#include <QDebug>
#include "sql.h"

class DataTypeTest : public QThread
{
    Q_OBJECT
public:
    explicit DataTypeTest(mysql_conn *m_arg, sqlServer_conn *s_arg, QObject *parent = nullptr);

    // 线程入口函数
    void run();


    mysql_conn *dataM;
    sqlServer_conn *dataS;

signals:
    void dataTestDone(QMap<QString, float> data_res, QString mess);

};

#endif // DATATYPETEST_H
