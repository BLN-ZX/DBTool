#include "sql.h"

mysql_conn::mysql_conn(QString hn, QString port, QString un, QString pwd)
{
    // 连接mysql,如果mysql已经存在了相应数据库的名字，则可以setDatabaseName再open打开连接数据库
    // 如果没有，想要系统去创建数据库，则setDatabaseName不要设置，open打开时这样连接不上,因为没有相应数据库
    // 而是先连接上mysql，通过命令创建数据库，再通过命令使用相应数据库，切换到该数据库上面
    MySQLdb = QSqlDatabase::addDatabase("QMYSQL");
    MySQLdb.setHostName(hn); // 输入连接的数据库IP
    MySQLdb.setPort(port.toInt()); // 输入连接的数据库端口
    MySQLdb.setUserName(un); //输入连接的用户名
    MySQLdb.setPassword(pwd); //输入连接的密码
    ms_is_open = MySQLdb.open();
    if(ms_is_open)
    {
//        QString databaseName = "infosystem";
//        MySQLdb.setDatabaseName(databaseName); // 数据库名称
//        qDebug()<<"打开数据库成功";
//        QSqlQuery query;
//        if(query.exec("create database "+databaseName + " DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci")) {
//            qDebug()<<"创建数据库"+databaseName+"成功";
//        } else
//        {
//            qDebug()<<"创建数据库"+databaseName+"失败";
//        }

//        if(query.exec("use "+databaseName)) {
//            qDebug()<<"使用数据库"+databaseName+"成功";
//        } else
//        {
//            qDebug()<<"使用数据库"+databaseName+"失败";
//        }
        qDebug() << "MYSQL OPEN SUCCESS!";

    } else
    {
        qDebug()<<"MYSQL OPEN ERROR!";
    }
}

mysql_conn::~mysql_conn()
{
    ms_is_open = false;

    // 关闭连接
    if(MySQLdb.open())
    {
        MySQLdb.close();
    }
}

sqlServer_conn::sqlServer_conn(QString hn, QString dbn, QString un, QString pwd)
{

    // 数据库配置
    // windows身份认证
    SqlServerdb = QSqlDatabase::addDatabase("QODBC", "SQLServer");
    SqlServerdb.setHostName(hn);         // 服务器名称
    SqlServerdb.setDatabaseName(dbn);       // 数据源名称
    SqlServerdb.setUserName(un);               // 用户名
    SqlServerdb.setPassword(pwd);         // 密码

    //数据库连接
    ss_is_open= SqlServerdb.open();
    if(ss_is_open)
    {
        qDebug()<<"SQLServer open sucess!";
    }
    else
    {
        qDebug()<<"SQLServer open ERROR!";
        qDebug() << SqlServerdb.lastError();
    }

    //数据库查询
//    QSqlQuery query("SELECT * FROM class;");
//    //qDebug()<<"ID     LoginName        LoginTime                LogoutTime        OPeration";
//    while (query.next())
//    {
////        int ID = query.value(0).toInt();
////        QString LoginName = query.value(1).toString();
////        QString LoginTime = query.value(2).toString();
////        QString LogoutTime = query.value(3).toString();
////        int OPeration = query.value(4).toInt();

////        qDebug()<<QString("%1       %2       %3         %4      %5")
////                      .arg(ID).arg(LoginName).arg(LoginTime).arg(LogoutTime).arg(OPeration);
//        QString clsNo = query.value(0).toString();
//        qDebug() << clsNo;
//    }
}

sqlServer_conn::~sqlServer_conn()
{
    ss_is_open = false;

    // 关闭连接
    if(SqlServerdb.open())
    {
        SqlServerdb.close();
    }
}


schema_mysql::schema_mysql(mysql_conn *m_schema_arg, sqlServer_conn* s_schema_arg, QString schemaNa)
    : schema()  //, m_schema(m_schema_arg), s_schema(s_schema_arg)
{
    init_info(m_schema_arg, s_schema_arg, schemaNa);
}

//QString schema_mysql::mess_conn = NULL;
//bool schema_mysql::is_conn = false;


//schema_mysql::schema_mysql(QString hn_ms, QString port_ms,
//                           QString un_ms, QString pwd_ms,
//                           QString hn_ss, QString dbn_ss,
//                           QString un_ss, QString pwd_ss,
//                           QString schemaName)
//    :schema(), mysql_conn(hn_ms, port_ms, un_ms, pwd_ms), sqlServer_conn(hn_ss, dbn_ss, un_ss, pwd_ss)
//{
//    //设置数据库模式

//    schema_mysql::is_conn = false;

//    this->schemaName = schemaName;

//    if(ms_is_open)
//    {
//        schema_mysql::mess_conn = "MYSQL IS OPEN SUCCESS";
//        if(ss_is_open)
//        {
//            schema_mysql::mess_conn.append("SQLServer IS OPEN SUCCESS");
//            schema_mysql::is_conn = true;
//            // 若能同时连接MySQL和SQLServer, 则初始化数据库
//            init_info();
//        }
//        else
//        {
//            schema_mysql::mess_conn.append("SQLServer IS OPEN DEFEAT");
//        }
//    }
//    else
//    {
//        schema_mysql::mess_conn = "MYSQL IS OPEN DEFEAT";
//    }
//}

QString schema_mysql::init_info(mysql_conn *mysql_arg, sqlServer_conn *sql_arg, QString schemaNa)
{
    schemaName = schemaNa;

    //MySQLdb.setDatabaseName("course");
    //QString schemaName = "sakila";

    mysql_arg->MySQLdb.setDatabaseName(schemaName);

    QString q;

    // q = "select * from information_schema.TABLES where table_schema = 'course';";
    q = "select * from information_schema.TABLES where table_schema = '" + schemaName + "';";
    //构建查询，初始化table
    QSqlQuery query1(q, mysql_arg->MySQLdb);
    //query.prepare(q);
//    if(query.exec())
//    {
//        while(query.next())
//        {
//            qDebug() << QString("字段数:%1     字段名:%2").arg(query.value(0).toString()).arg(query.value(1).toString());
//        }
//    }

    // 构建查询，初始化表
    while(query1.next())
    {
        bool view = (QString("VIEW") == query1.value(3));
        qDebug() << view;
        table tap(query1.value(2).toString().toStdString(), schemaName.toStdString(), !view, !view);
        tables.push_back(tap);
        qDebug() << QString("表模式:%1     表名:%2      表类型:%3").
                    arg(query1.value(1).toString()).arg(query1.value(2).toString()).arg(query1.value(3).toString());
    }

    // 构建查询，初始化列数据
    for(auto t = tables.begin(); t != tables.end(); ++t)
    {
        QString q = "select distinct(COLUMN_NAME), DATA_TYPE from information_schema.COLUMNS where table_name = '";
        q.append(QString::fromStdString(t->name)); q.append("\'");
        QSqlQuery query2(q, mysql_arg->MySQLdb);
        while(query2.next())
        {
            column c(query2.value(0).toString().toStdString(), sqltype::get(query2.value(1).toString().toStdString()));
            t->columns().push_back(c);
            qDebug() << QString("属性名:%1     属性类型:%2").
                                arg(query2.value(0).toString()).arg(query2.value(1).toString());
        }
    }

    // #t、#n 转换成字符串
    #define BINOP(n,t) do {op o(#n,sqltype::get(#t),sqltype::get(#t),sqltype::get(#t)); register_operator(o); } while(0)

    BINOP(+, TINYINT); BINOP(-, TINYINT); BINOP(*, TINYINT); BINOP(/, TINYINT);
    BINOP(+, SMALLINT); BINOP(-, SMALLINT); BINOP(*, SMALLINT); BINOP(/, SMALLINT);
    BINOP(+, MEDIUMINT); BINOP(-, MEDIUMINT); BINOP(*, MEDIUMINT); BINOP(/, MEDIUMINT);
    BINOP(+, INT); BINOP(-, INT); BINOP(*, INT); BINOP(/, INT);
    BINOP(+, BIGINT); BINOP(-, BIGINT); BINOP(*, BIGINT); BINOP(/, BIGINT);

    BINOP(+, FLOAT); BINOP(-, FLOAT); BINOP(*, FLOAT); BINOP(/, FLOAT);
    BINOP(+, DOUBLE); BINOP(-, DOUBLE); BINOP(*, DOUBLE); BINOP(/, DOUBLE);
    BINOP(+, DECIMAL); BINOP(-, DECIMAL); BINOP(*, DECIMAL); BINOP(/, DECIMAL);

    //BINOP(||, TEXT);

    // 位运算符
    BINOP(&, INT); BINOP(&, SMALLINT);
    BINOP(|, INT); BINOP(|, SMALLINT);
    BINOP(~, INT); BINOP(~, SMALLINT);
    BINOP(^, INT); BINOP(^, SMALLINT);
    BINOP(<<, INT); BINOP(<<, SMALLINT);
    BINOP(>>, INT); BINOP(>>, SMALLINT);

    BINOP(<, INT); BINOP(<, SMALLINT);
    BINOP(<=, INT); BINOP(<=, SMALLINT);
    BINOP(>, INT); BINOP(>, SMALLINT);
    BINOP(>=, INT); BINOP(>=, SMALLINT);

    BINOP(=, INT); BINOP(=, SMALLINT);
    BINOP(<>, INT); BINOP(<>, SMALLINT);
    BINOP(=, CHAR);
    BINOP(<>, CHAR);
    BINOP(=, VARCHAR);
    BINOP(<>, VARCHAR);
    BINOP(=, TEXT);
    BINOP(<>, TEXT);
    // BINOP(IS, INT);
    // BINOP(IS NOT, INT);

    BINOP(AND, INT); BINOP(AND, SMALLINT);
    BINOP(OR, INT); BINOP(OR, SMALLINT);

    // 返回类型r 无参数
    #define FUNC(n,r) do {							\
      routine proc("", "", sqltype::get(#r), #n);				\
      register_routine(proc);						\
    } while(0)

    // 返回类型r 参数类型a
    #define FUNC1(n,r,a) do {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      						\
      routine proc("", "", sqltype::get(#r), #n);				\
      proc.argtypes.push_back(sqltype::get(#a));				\
      register_routine(proc);						\
    } while(0)

    // 返回类型r 参数类型a b
    #define FUNC2(n,r,a,b) do {						\
      routine proc("", "", sqltype::get(#r), #n);				\
      proc.argtypes.push_back(sqltype::get(#a));				\
      proc.argtypes.push_back(sqltype::get(#b));				\
      register_routine(proc);						\
    } while(0)

    // 返回类型r 参数类型a b c
    #define FUNC3(n,r,a,b,c) do {						\
      routine proc("", "", sqltype::get(#r), #n);				\
      proc.argtypes.push_back(sqltype::get(#a));				\
      proc.argtypes.push_back(sqltype::get(#b));				\
      proc.argtypes.push_back(sqltype::get(#c));				\
      register_routine(proc);						\
    } while(0)


//    FUNC(last_insert_rowid, INTEGER);
//    FUNC(random, INTEGER);
//    FUNC(sqlite_source_id, TEXT);
//    FUNC(sqlite_version, TEXT);
//    FUNC(total_changes, INTEGER);

    FUNC(RAND, FLOAT);

    FUNC1(ABS, FLOAT, FLOAT);
    FUNC1(ABS, INT, INT); FUNC1(ABS, INT, SAMLLINT);
    FUNC1(LENGTH, INT, CHAR); FUNC1(LENGTH, INT, VARCHAR);
    FUNC1(LOWER, CHAR, CHAR); FUNC1(LOWER, VARCHAR, VARCHAR);

    //FUNC1(abs, INTEGER, FLOAT);
    //FUNC1(hex, CHAR, CHAR);
    FUNC1(length, INT, VARCHAR);
    FUNC1(lower, VARCHAR, VARCHAR);
    FUNC1(ltrim, TEXT, TEXT);
    FUNC1(quote, varchar, varchar);
    //FUNC1(randomblob, TEXT, INTEGER);
    FUNC1(round, INT, FLOAT);
    FUNC1(rtrim, VARCHAR, VARCHAR);
    //FUNC1(soundex, TEXT, TEXT);
    //FUNC1(sqlite_compileoption_get, TEXT, INTEGER);
    //FUNC1(sqlite_compileoption_used, INTEGER, TEXT);
    FUNC1(trim, VARCHAR, VARCHAR);
    //FUNC1(typeof, TEXT, INTEGER);
    //FUNC1(typeof, TEXT, NUMERIC);
    //FUNC1(typeof, TEXT, REAL);
    //FUNC1(typeof, TEXT, TEXT);
    //FUNC1(unicode, INTEGER, TEXT);
    FUNC1(upper, VARCHAR, VARCHAR);  FUNC1(upper, CHAR, CHAR);
    //FUNC1(zeroblob, TEXT, INTEGER);

    //FUNC2(glob, INTEGER, TEXT, TEXT);
    FUNC2(instr, INT, VARCHAR, VARCHAR); FUNC2(instr, INT, CHAR, CHAR);
    FUNC2(like, INT, VARCHAR, VARCHAR); FUNC2(like, INT, CHAR, CHAR);
//    FUNC2(ltrim, TEXT, TEXT, TEXT);
//    FUNC2(rtrim, TEXT, TEXT, TEXT);
//    FUNC2(trim, TEXT, TEXT, TEXT);
    FUNC2(round, INT, FLOAT, INT);
    FUNC2(substr, VARCHAR, VARCHAR, INT);
    FUNC3(substr, VARCHAR, VARCHAR, INT, INT);
    FUNC3(replace, VARCHAR, VARCHAR, VARCHAR, VARCHAR);

    // 聚集函数
    #define AGG(n,r, a) do {						\
       routine proc("", "", sqltype::get(#r), #n);				\
       proc.argtypes.push_back(sqltype::get(#a));				\
       register_aggregate(proc);						\
     } while(0)

     AGG(avg, INT, INT);
     AGG(avg, FLOAT, FLOAT); AGG(avg, DECIMAL, DECIMAL);
     AGG(count, INT, FLOAT);
     AGG(count, INT, VARCHAR);
     AGG(count, INT, INT);
     AGG(count, INT, SAMLLINT);
     AGG(count, INT, DECIMAL);
     //AGG(group_concat, TEXT, TEXT);
     AGG(max, FLOAT, FLOAT); AGG(max, DECIMAL, DECIMAL);
     AGG(max, INT, INT); AGG(max, SMALLINT, SMALLINT);
     AGG(sum, FLOAT, FLOAT); AGG(sum, DECIMAL, DECIMAL);
     AGG(sum, INT, INT); AGG(sum, SMALLINT, SMALLINT);
//     AGG(total, REAL, INTEGER);
//     AGG(total, REAL, REAL);


     // 初始化类成员
     // mysql没有内置的BOOL/BOOLEAN的类型，使用TINYINT作为同义词
     booltype = sqltype::get("TINYINT");
     inttype = sqltype::get("INT");



     //不可作为参数的数据类型
     internaltype = sqltype::get("internal");
     //ARRAY数据类型
     arraytype = sqltype::get("ARRAY");

     // 真假值
     true_literal = "true";
     false_literal = "false";

     generate_indexes();

     return "SUCCESS!";
}

