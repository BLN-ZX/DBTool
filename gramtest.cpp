#include "gramtest.h"

gramtest::gramtest(struct scope scope_arg, shared_ptr<struct schema> schema_arg, mysql_conn *m_arg, sqlServer_conn *s_arg, int num, QObject *parent)
    : QThread(parent), gramM(m_arg), gramS(s_arg), scope(scope_arg), schema(schema_arg), number(num)
{

}

void gramtest::run()
{
    // 存储运行返回信息
    QStringList gramm_info;

    // 存储运行的SQL语句
    QStringList SQL_stmt;
    QStringList m_res;
    QStringList s_res;

    // 开始测试
    try
    {
        QString clear_query = "use sakila;"
                        "drop table if exists tinyint_test_table;"
                        "drop table if exists smallint_test_table;"
                        "drop table if exists int_test_table;"
                        "drop table if exists bigint_test_table;"
                        "drop table if exists bool_test_table;"
                        "drop table if exists float_test_table;"
                        "drop table if exists double_test_table;"
                        "drop table if exists char_test_table;"
                        "drop table if exists varchar_test_table;";


        QSqlQuery clear_my(clear_query, gramM->MySQLdb);
        QSqlQuery clear_ss(clear_query, gramS->SqlServerdb);
        clear_ss.prepare(clear_query);

        clear_ss.exec();
        clear_ss.exec();


//        if(!(clear_my.exec() && clear_ss.exec()))
//        {
//            qDebug() << clear_my.lastError() << clear_ss.lastError();
//            emit(nullptr, nullptr, mess);
//            return;
//        }


        float t = 0, f = 0;

        QTime time;
        // 进行语法测试
        for(int i = 0; i < number; i++)
        {
            time.start();
            //等待时间流逝1秒钟
            while(time.elapsed() < 1000)
            {
                //处理事件
            }

            //构造生成器
            gen = statement_factory(&scope);
            gen->out(s);

            std::cerr << s.str() << "\n-----------------------------\n";

            // 转为QString
            QString stmt = QString::fromStdString(s.str());

            SQL_stmt.append(stmt);


            // SQL语句输入数据库进行测试
            QString mysql_stmt = stmt;
            QString sql_stmt = "use sakila;" + stmt.replace("sakila.", "");

            // 构建查询
            QSqlQuery gram_my(mysql_stmt, gramM->MySQLdb);
            QSqlQuery gram_ss(sql_stmt, gramS->SqlServerdb);

            // SQLServer必备，否则函数序列错误
            gram_ss.prepare(sql_stmt);

            bool ms = gram_my.exec();
            bool ss = gram_ss.exec();

            if(ms && ss)
            {

                qDebug() << "T";
                t++;
                m_res.push_back("T");
                s_res.push_back("T");
            }
            else if(ms)
            {
                qDebug() << "F";
                f++;
                // qDebug() << gram_ss.lastError();
                gramm_info.push_back(gram_ss.lastError().text());

                m_res.push_back("T");
                s_res.push_back(gram_ss.lastError().text());
            }
            else
            {
                //
                gramm_info.push_back(gram_ss.lastError().text());
                m_res.push_back(gram_my.lastError().text());
                s_res.push_back(gram_ss.lastError().text());
            }

            //清空缓冲区
            s.str("");
         }
         qDebug() << float(t/(t+f));

         emit gramtestDone(float(t/(t+f)), SQL_stmt, m_res, s_res, gramm_info, "Y");
    } catch (const QSqlError &amp)
    {
          emit gramtestDone(0, SQL_stmt, m_res, s_res, gramm_info, amp.text());
    }

}
