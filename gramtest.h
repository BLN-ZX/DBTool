#ifndef GRAMTEST_H
#define GRAMTEST_H

#include <QObject>
#include <QThread>
#include <QSqlDatabase>
#include <QDebug>
#include <sstream>
#include <memory>
#include <typeinfo>
#include <cassert>
#include <QThread>
#include <QTime>

#include "sql.h"
#include "prod.h"
#include "grammar.h"
#include "schema.h"

class gramtest : public QThread
{
    Q_OBJECT
public:
    explicit gramtest(scope scope_arg, shared_ptr<schema> schema_arg,
                      mysql_conn *m_arg, sqlServer_conn *s_arg,
                      int num,
                      QObject *parent = nullptr);

    mysql_conn *gramM;
    sqlServer_conn *gramS;

    void run();

    // 定义数据库的范围
    scope scope;
    // 定义数据库的模式
    shared_ptr<schema> schema;
    // SQL语句生成器
    shared_ptr<prod> gen;
    // SQL语句输出流
    std::ostringstream s;
    // 测试语句数量
    int number;

signals:

    void gramtestDone(double r, QStringList stmt, QStringList m, QStringList s, QStringList info, QString mess);
};

#endif // GRAMTEST_H
