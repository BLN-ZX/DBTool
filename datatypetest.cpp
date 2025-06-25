#include "datatypetest.h"

DataTypeTest::DataTypeTest(mysql_conn *m_arg, sqlServer_conn *s_arg, QObject *parent)
    : QThread(parent), dataM(m_arg), dataS(s_arg)
{

}

void DataTypeTest::run()
{
    // map存储每一种数据类型测试结果
    QMap<QString, float> res;
    try
    {
        // 争对数据类型进行测试
        qDebug() << "线程正在运行";
        QStringList data_q;
        data_q.push_back("drop table if exists tinyint_test_table; create table tinyint_test_table(tinyint_test tinyint);");
        data_q.push_back("drop table if exists smallint_test_table; create table smallint_test_table(smallint_test smallint);");
        data_q.push_back("drop table if exists int_test_table; create table int_test_table(int_test int);");
        //data_q.push_back("drop table if exists bigint_test_table; create table bigint_test_table(bigint_test bigint);");
        data_q.push_back("drop table if exists bool_test_table; create table bool_test_table(bool_test bool);");
        data_q.push_back("drop table if exists float_test_table; create table float_test_table(float_test float);");
        data_q.push_back("drop table if exists double_test_table; create table double_test_table(double_test double);");
        data_q.push_back("drop table if exists char_test_table; create table char_test_table(char_test char(:length));");
        data_q.push_back("drop table if exists varchar_test_table; create table varchar_test_table(varchar_test varchar(:length));");
        data_q.push_back("drop table if exists decimal_test_table; create table decimal_test_table(decimal_test decimal);");

        //连接数据库

        QSqlQuery test_my(dataM->MySQLdb);

        QSqlQuery test_sql(dataS->SqlServerdb);

        //定义数据，成功次数
        int n = 0;
        float mysql_count = 0, sqlserver_count = 0;

        // 测试tinyint--------------------------------------------------------
        QString tiny_insert = "insert into tinyint_test_table values(:up);"
                              "insert into tinyint_test_table values(:down);";
        test_my.prepare("use sakila;" + data_q[0] + tiny_insert);
        test_my.bindValue(":up", 0);
        test_my.bindValue(":down", 0);

        test_sql.prepare("use sakila;" + data_q[0] + tiny_insert);
        test_sql.bindValue(":up", 0);
        test_sql.bindValue(":down", 0);

        //测试mysql
        while(test_my.exec())
        {
            mysql_count++;
            n++;

            qint64 up = ~(1 << n)+1, down = (1 << n)-1;
            //qDebug() << up << down;


            test_my.prepare(tiny_insert);
            test_my.bindValue(":up", up);
            test_my.bindValue(":down", down);
        }
        //qDebug() << test_my.lastError();

        //qDebug() << mysql_count;

        //n置零
        n = 0;

        //测试sqlserver
        while(test_sql.exec())
        {
            sqlserver_count++;
            n++;

            qint64 up = ~(1 << n)+1, down = (1 << n)-1;
            //qDebug() << up << down;

            test_sql.prepare(tiny_insert);
            test_sql.bindValue(":up", up);
            test_sql.bindValue(":down", down);
        }
        //记录结果
        //qDebug() << sqlserver_count;
        res.insert("tinyint", sqlserver_count/mysql_count);
        qDebug() << "tinyint:" <<sqlserver_count/mysql_count;


        // 次数置零
        mysql_count = 0, sqlserver_count = 0, n = 0;

        // 测试smallint--------------------------------------------------
        QString small_insert = "insert into smallint_test_table values(:up);"
                              "insert into smallint_test_table values(:down);";
        test_my.prepare("use sakila;" + data_q[1] + small_insert);
        test_my.bindValue(":up", 0);
        test_my.bindValue(":down", 0);

        test_sql.prepare("use sakila;" + data_q[1] + small_insert);
        test_sql.bindValue(":up", 0);
        test_sql.bindValue(":down", 0);

        //测试mysql
        while(test_my.exec())
        {
            mysql_count++;
            n++;

            qint64 up = ~(1 << n)+1, down = (1 << n)-1;
            //qDebug() << up << down;


            test_my.prepare(small_insert);
            test_my.bindValue(":up", up);
            test_my.bindValue(":down", down);
        }
        qDebug() << test_my.lastError();

        //qDebug() << mysql_count;

        //n置零
        n = 0;

        //测试sqlserver
        while(test_sql.exec())
        {
            sqlserver_count++;
            n++;

            qint64 up = ~(1 << n)+1, down = (1 << n)-1;
            //qDebug() << up << down;

            test_sql.prepare(small_insert);
            test_sql.bindValue(":up", up);
            test_sql.bindValue(":down", down);
        }
        //记录结果
        //qDebug() << sqlserver_count;
        res.insert("smallint", sqlserver_count/mysql_count);
        qDebug() << "smallint:" <<sqlserver_count/mysql_count;


        // 次数置零
        mysql_count = 0, sqlserver_count = 0, n = 0;

        // 测试int--------------------------------------------------
        QString int_insert = "insert into int_test_table values(:up);"
                              "insert into int_test_table values(:down);";
        test_my.prepare("use sakila;" + data_q[2] + int_insert);
        test_my.bindValue(":up", 0);
        test_my.bindValue(":down", 0);

        test_sql.prepare("use sakila;" + data_q[2] + int_insert);
        test_sql.bindValue(":up", 0);
        test_sql.bindValue(":down", 0);

        //测试mysql
        while(test_my.exec())
        {
            mysql_count++;
            n++;

            quint64 up = ~(1 << n)+1;
            //qDebug() << n << up;


            test_my.prepare(int_insert);
            test_my.bindValue(":up", up);
            test_my.bindValue(":down", 0);

        }
        qDebug() << test_my.lastError();

        //qDebug() << mysql_count;

        //n置零
        n = 0;

        //测试sqlserver
        while(test_sql.exec())
        {
            sqlserver_count++;
            n++;

            quint64 up = ~(1 << n)+1;
            //qDebug() << up;

            test_sql.prepare(int_insert);
            test_sql.bindValue(":up", up);
            test_sql.bindValue(":down", 0);
        }
        //记录结果
        //qDebug() << sqlserver_count;
        res.insert("int", sqlserver_count/mysql_count);
        qDebug() << "int:" <<sqlserver_count/mysql_count;

        // 次数置零
        mysql_count = 0, sqlserver_count = 0, n = 0;

        // 测试bool--------------------------------------------------
        QString bool_insert = "insert into bool_test_table values(:up);"
                              "insert into bool_test_table values(:down);";
        test_my.prepare("use sakila;" + data_q[3] + bool_insert);
        test_my.bindValue(":up", "true");
        test_my.bindValue(":down", "false");

        test_sql.prepare("use sakila;" + data_q[3] + bool_insert);
        test_sql.bindValue(":up", "true");
        test_sql.bindValue(":down", "false");

        //测试mysql
        if(test_my.exec()) mysql_count++;
        qDebug() << test_my.lastError();
        //qDebug() << mysql_count;

        //n置零
        n = 0;

        //测试sqlserver
        if(test_sql.exec()) sqlserver_count++;
        //记录结果
        //qDebug() << sqlserver_count;
        res.insert("bool", sqlserver_count/mysql_count);
        qDebug() << "bool:" <<sqlserver_count/mysql_count;


        // 次数置零
        mysql_count = 0, sqlserver_count = 0, n = 0;

        // 测试float--------------------------------------------------
        QString float_insert = "insert into float_test_table values(:up);"
                              "insert into float_test_table values(:down);";
        test_my.prepare("use sakila;" + data_q[4] + float_insert);
        test_my.bindValue(":up", -3.402823466E+38);
        test_my.bindValue(":down", -1.175494351E-38);

        if(test_my.exec())
        {
            mysql_count++;
        }

        qDebug() << mysql_count;
        qDebug() << test_my.lastError();

        //n置零
        n = 0;

        test_sql.prepare("use sakila;" + data_q[4] + float_insert);
        test_sql.bindValue(":up", -3.402823466E+38);
        test_sql.bindValue(":down", -1.175494351E-38);

        if(test_sql.exec())
        {
            sqlserver_count++;
        }

        //qDebug() << sqlserver_count;
        res.insert("float", sqlserver_count/mysql_count);
        qDebug() << "float:" <<sqlserver_count/mysql_count;
        // 通知主线程测试已经完成，并返回测试结果

        // 次数置零
        mysql_count = 0, sqlserver_count = 0, n = 0;

        // 测试double--------------------------------------------------
        QString double_insert = "insert into double_test_table values(:up);"
                              "insert into double_test_table values(:down);";
        test_my.prepare("use sakila;" + data_q[5] + double_insert);
        test_my.bindValue(":up", -1.7976931348623157E+308);
        test_my.bindValue(":down", -2.2250738585072014E-308);

        if(test_my.exec())
        {
            mysql_count++;
        }

        qDebug() << mysql_count;
        qDebug() << test_my.lastError();

        //n置零
        n = 0;

        test_sql.prepare("use sakila;" + data_q[5] + double_insert);
        test_sql.bindValue(":up", -1.7976931348623157E+308);
        test_sql.bindValue(":down", -2.2250738585072014E-308);

        if(test_sql.exec())
        {
            sqlserver_count++;
        }

        //qDebug() << sqlserver_count;
        res.insert("double", sqlserver_count/mysql_count);
        qDebug() << "double:" <<sqlserver_count/mysql_count;
        // 通知主线程测试已经完成，并返回测试结果


        // 次数置零
        mysql_count = 0, sqlserver_count = 0, n = 0;

        // 测试char--------------------------------------------------
    //    QString char_insert = "insert into char_test_table values(:up);"
    //                          "insert into char_test_table values(:down);";
        test_my.prepare("use sakila;" + data_q[6]);
        test_my.bindValue(":length", 0);

    //    test_my.prepare("drop table if exists char_test_table;");
    //    test_my.exec();

    //    test_my.prepare("create table char_test_table(char_test char(256));");
    //    test_my.exec();

        int l = 0;

        while(test_my.exec())
        {
            mysql_count++;
            l += 10;
            //qDebug() << l;
            test_my.prepare("use sakila; drop table if exists char_test_table;");
            test_my.exec();

            test_my.prepare("create table char_test_table(char_test char(:length));");
            test_my.bindValue(":length", l);
        }


        //qDebug() << mysql_count;
        qDebug() << test_my.lastError();

        //n置零
        l = 0;

    //    test_sql.prepare("use sakila;" + data_q[6]);
    //    test_sql.bindValue(":length", 0);

        test_sql.prepare("use sakila");
        while(test_sql.exec())
        {
            sqlserver_count++;
            l += 10;
            //qDebug() << l;

            test_sql.prepare("use sakila; drop table if exists char_test_table;");
            test_sql.exec();
            test_sql.prepare("create table char_test_table(char_test char(" + QString::number(l) + "));");
        }

        //qDebug() << sqlserver_count;
        res.insert("char", sqlserver_count/mysql_count);
        qDebug() << "char:" << sqlserver_count/mysql_count;

        // 次数置零
        mysql_count = 0, sqlserver_count = 0, n = 0;

        // 测试varchar--------------------------------------------------
        test_my.prepare("use sakila;" + data_q[7]);
        test_my.bindValue(":length", 0);

        l = 0;

        while(test_my.exec())
        {
            mysql_count++;
            l += 100;
            //qDebug() << l;
            test_my.prepare("use sakila; drop table if exists varchar_test_table;");
            test_my.exec();

            test_my.prepare("create table varchar_test_table(varchar_test varchar(:length));");
            test_my.bindValue(":length", l);
        }


        //qDebug() << mysql_count;
        qDebug() << test_my.lastError();

        //n置零
        l = 0;

        test_sql.prepare("use sakila");
        while(test_sql.exec())
        {
            sqlserver_count++;
            l += 100;
            //qDebug() << l;

            test_sql.prepare("use sakila; drop table if exists varchar_test_table;");
            test_sql.exec();
            test_sql.prepare("create table varchar_test_table(varchar_test varchar(" + QString::number(l) + "));");
        }
        //qDebug() << sqlserver_count;
        res.insert("varchar", sqlserver_count/mysql_count);
        qDebug() << "varchar:" << sqlserver_count/mysql_count;
        // 通知主线程测试已经完成，并返回测试结果

        emit dataTestDone(res, "Y");
    } catch (const QSqlError &amp)
    {
        qDebug() << amp.text();
        emit dataTestDone(res, amp.text());
    }

}
