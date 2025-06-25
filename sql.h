#ifndef MYSQL_H
#define MYSQL_H

#include "schema.h"
#include "relmodel.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>

struct mysql_conn
{
    bool ms_is_open;
    QString mysqlHostName;
    QString mysqlPort;
    QString mysqlUserName;
    QString mysqlPassWord;

    QSqlDatabase MySQLdb;
    mysql_conn(QString hn = "localhost", QString port = "3306", QString un = "root", QString pwd = "2023@hfut");
    ~mysql_conn();
};

struct sqlServer_conn
{
    bool ss_is_open;
    QString sqlSerHostName;
    QString sqlSerDatabaseName;
    QString sqlSerUserName;
    QString sqlSerPassword;

    QSqlDatabase SqlServerdb;
    sqlServer_conn(QString hn = "DESKTOP-6HLMKAP", QString dbn = "ZXTEST", QString un = "ZX", QString pwd = "ZX123456");
    ~sqlServer_conn();
};

//struct schema_mysql : schema, mysql_conn, sqlServer_conn
//{

//    schema_mysql(QString hn_ms = "localhost", QString port_ms = "3306",
//                 QString un_ms = "root", QString pwd_ms = "2023@hfut",
//            QString hn_ss = "DESKTOP-6HLMKAP", QString dbn_ss = "ZXTEST",
//                 QString un_ss = "ZX", QString pwd_ss = "ZX123456",
//                QString schemaName = "sakila");
//    QString init_info();
//    QString schemaName;
//    static QString mess_conn;
//    static bool is_conn;
//    virtual std::string quote_name(const std::string &id)
//    {
//       return id;
//    }
//};

struct schema_mysql : schema
{
//    mysql_conn *m_schema;
//    sqlServer_conn *s_schema;
    schema_mysql(mysql_conn *m_schema_arg, sqlServer_conn *s_schema_arg, QString schemaNa);
    virtual std::string quote_name(const std::string &id)
    {
        return id;
    }
    QString schemaName;
    QString init_info(mysql_conn *mysql_arg, sqlServer_conn *sql_arg, QString schemaNa);
};


#endif // MYSQL_H
