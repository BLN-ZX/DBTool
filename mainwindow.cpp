#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtConcurrent/QtConcurrent>

//#include "sql.h"
//#include "prod.h"
//#include "grammar.h"
//#include <memory>
//#include <typeinfo>
//#include <cassert>
//#include <sstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置进度条为0
    ui->progressBar->setValue(0);
    ui->progressBar_2->setValue(0);
    ui->progressBar_3->setValue(0);
    ui->progressBar_4->setValue(0);
    ui->progressBar_5->setValue(0);
    ui->progressBar_6->setValue(0);
    ui->progressBar_7->setValue(0);
    ui->progressBar_8->setValue(0);
    ui->progressBar_9->setValue(0);

    // 设置数据类型测试按钮不可用
    ui->pushButton_2->setEnabled(false);
    // 设置语法测试按钮不可用
    ui->pushButton_3->setEnabled(false);
    // 初始化工具条
    ui->tabWidget->setAttribute(Qt::WA_StyledBackground);

    // 初始化子窗口，展示连接进度，并连接槽函数
    pr = new Processing;
    connect(pr, &Processing::sendConn, this, &MainWindow::recConn);

    // 初始化子窗口，展示测试进度
    tpr = new TestProcess;

    // 初始化指针
    M = nullptr;
    S = nullptr;
    dataTh = nullptr;
    gramTh = nullptr;

    // 系统日志
    //获取系统当前的时间
    QDateTime dateTime= QDateTime::currentDateTime();
    //格式化时间
    QString str = dateTime .toString("yyyy-MM-dd hh:mm:ss");

    // 系统日志
    ui->textBrowser_2->append(str + "系统启动..." + "\n");

    // 设置无法选中
    ui->radioButton->setEnabled(false);
    ui->radioButton_2->setEnabled(false);

    // 连接状态
    ui->radioButton->setChecked(false);
    ui->radioButton_2->setChecked(true);
}

// 返回数据类型测试信息
void MainWindow::recDataRes(QMap<QString, float> res, QString mess)
{
    connect(pwatcher, &QFutureWatcher<void>::finished, this, [=]()
    {
        //主线程操作
        tpr->StopAnimation();//关闭动画
        tpr->hide();
    });
    pwatcher->setFuture(future);

    // 根据返回信息做出判断，数据类型测试是否成功进行
    if(mess != "Y")
    {
        ui->textBrowser->append(mess);
        return;
    }
    else
    {
        ui->textBrowser->append("The data type test has been completed.");
    }

    datares = res;

    // 显示前进行判断
    // 若大于100%则显示100%
    if(datares["tinyint"] > 1)
    {
        ui->progressBar->setValue(100);
    }
    else ui->progressBar->setValue(datares["tinyint"] * 100);

    if(datares["smallint"] > 1)
    {
        ui->progressBar_2->setValue(100);
    }
    else ui->progressBar_2->setValue(datares["smallint"] * 100);

    if(datares["int"] > 1)
    {
        ui->progressBar_3->setValue(100);
    }
    else ui->progressBar_3->setValue(datares["int"] * 100);

    if(datares["bool"] > 1)
    {
        ui->progressBar_4->setValue(100);
    }
    else ui->progressBar_4->setValue(datares["bool"] * 100);

    if(datares["float"] > 1)
    {
        ui->progressBar_5->setValue(100);
    }
    else ui->progressBar_5->setValue(datares["float"] * 100);

    if(datares["double"] > 1)
    {
        ui->progressBar_6->setValue(100);
    }
    else ui->progressBar_6->setValue(datares["double"] * 100);

    if(datares["char"] > 1)
    {
        ui->progressBar_7->setValue(100);
    }
    else ui->progressBar_7->setValue(datares["char"] * 100);

    if(datares["varchar"] > 1)
    {
        ui->progressBar_8->setValue(100);
    }
    else ui->progressBar_8->setValue(datares["varchar"] * 100);

    ui->textBrowser_7->setText("测试结果代表在某种数据类型中：SQLServer对MySQL的兼容百分比\n0%代表完全不兼容\n100%代表完全兼容");

    // 设置语法测试可以进行
    ui->pushButton_3->setEnabled(true);
}

// 返回语法测试结果
void MainWindow::recGramRes(double r, QStringList SQLstmt, QStringList m_res, QStringList s_res, QStringList graminfo, QString mess)
{
    connect(pwatcher, &QFutureWatcher<void>::finished, this, [=]()
    {
        //主线程操作
        tpr->StopAnimation();//关闭动画
        tpr->hide();
    });
    pwatcher->setFuture(future);

    // 根据返回信息做出判断，语法测试是否成功进行
    if(mess != "Y")
    {
        ui->textBrowser->append(mess);
        return;
    }
    else
    {
        ui->textBrowser->append("The grammar test has been completed.");
    }

    ui->progressBar_9->setValue(r * 100);

    // SQL语句和执行结果传回
    SQLSTMT = SQLstmt;
    mysqlRes = m_res;
    sqlserRes = s_res;
    \
    // 处理结果并放入表格
    int row_index = 0;
    // 存储是否已经检测过
    QMap<QString, bool> is_check;

    int num_row = ui->tableWidget->rowCount();
    int num_column = ui->tableWidget->columnCount();
    for(int row=0; row<num_row ; row++){
        for(int col=0; col<num_column ; col++){
            ui->tableWidget->removeCellWidget(row, col);
        }
    }


    for(int i = 0; i < graminfo.length(); i++)
    {
        if(graminfo[i].contains("非布尔类型"))
        {
            if(is_check.contains("true") == false)
            {
                QTableWidgetItem* t = new QTableWidgetItem("true");
                ui->tableWidget->setItem(row_index, 0, t);

                if(SQLSTMT[i].contains("insert") || SQLSTMT[i].contains("delete") || SQLSTMT[i].contains("update") || SQLSTMT[i].contains("select"))
                {
                    QTableWidgetItem* t1 = new QTableWidgetItem("DML");
                    ui->tableWidget->setItem(row_index, 1, t1);
                }
                else
                {
                    QTableWidgetItem* t1 = new QTableWidgetItem("DDL");
                    ui->tableWidget->setItem(row_index, 1, t1);
                }

                // 兼容情况
                t = new QTableWidgetItem("Yes");
                ui->tableWidget->setItem(row_index, 2, t);

                t = new QTableWidgetItem("No");
                ui->tableWidget->setItem(row_index, 3, t);

                row_index++;

                is_check.insert("true", true);
            }

            if(is_check.contains("false") == false)
            {
                QTableWidgetItem* t = new QTableWidgetItem("false");
                ui->tableWidget->setItem(row_index, 0, t);

                if(SQLSTMT[i].contains("insert") || SQLSTMT[i].contains("delete") || SQLSTMT[i].contains("update") || SQLSTMT[i].contains("select"))
                {
                    QTableWidgetItem* t1 = new QTableWidgetItem("DML");
                    ui->tableWidget->setItem(row_index, 1, t1);
                }
                else
                {
                    QTableWidgetItem* t1 = new QTableWidgetItem("DDL");
                    ui->tableWidget->setItem(row_index, 1, t1);
                }

                // 兼容情况
                t = new QTableWidgetItem("Yes");
                ui->tableWidget->setItem(row_index, 2, t);

                t = new QTableWidgetItem("No");
                ui->tableWidget->setItem(row_index, 3, t);


                row_index++;

                is_check.insert("false", true);
            }
        }
        else if(graminfo[i].contains("limit") &&  is_check.contains("limit") == false)
        {
            QTableWidgetItem* t = new QTableWidgetItem("limit");
            ui->tableWidget->setItem(row_index, 0, t);

            if(SQLSTMT[i].contains("insert") || SQLSTMT[i].contains("delete") || SQLSTMT[i].contains("update") || SQLSTMT[i].contains("select"))
            {
                QTableWidgetItem* t1 = new QTableWidgetItem("DML");
                ui->tableWidget->setItem(row_index, 1, t1);
            }
            else
            {
                QTableWidgetItem* t1 = new QTableWidgetItem("DDL");
                ui->tableWidget->setItem(row_index, 1, t1);
            }

            // 兼容情况
            t = new QTableWidgetItem("Yes");
            ui->tableWidget->setItem(row_index, 2, t);

            t = new QTableWidgetItem("No");
            ui->tableWidget->setItem(row_index, 3, t);

            row_index++;

            is_check.insert("limit", true);
        }
        else if(graminfo[i].contains("show") && is_check.contains("show") == false)
        {
            QTableWidgetItem* t = new QTableWidgetItem("show");
            ui->tableWidget->setItem(row_index, 0, t);

            if(SQLSTMT[i].contains("insert") || SQLSTMT[i].contains("delete") || SQLSTMT[i].contains("update") || SQLSTMT[i].contains("select"))
            {
                QTableWidgetItem* t1 = new QTableWidgetItem("DML");
                ui->tableWidget->setItem(row_index, 1, t1);
            }
            else
            {
                QTableWidgetItem* t1 = new QTableWidgetItem("DDL");
                ui->tableWidget->setItem(row_index, 1, t1);
            }

            // 兼容情况
            t = new QTableWidgetItem("Yes");
            ui->tableWidget->setItem(row_index, 2, t);

            t = new QTableWidgetItem("No");
            ui->tableWidget->setItem(row_index, 3, t);

            row_index++;

            is_check.insert("show", true);
        }
        else if(graminfo[i].contains("rename") && is_check.contains("rename") == false)
        {
            QTableWidgetItem* t = new QTableWidgetItem("rename");
            ui->tableWidget->setItem(row_index, 0, t);

            if(SQLSTMT[i].contains("insert") || SQLSTMT[i].contains("delete") || SQLSTMT[i].contains("update") || SQLSTMT[i].contains("select"))
            {
                QTableWidgetItem* t1 = new QTableWidgetItem("DML");
                ui->tableWidget->setItem(row_index, 1, t1);
            }
            else
            {
                QTableWidgetItem* t1 = new QTableWidgetItem("DDL");
                ui->tableWidget->setItem(row_index, 1, t1);
            }

            // 兼容情况
            t = new QTableWidgetItem("Yes");
            ui->tableWidget->setItem(row_index, 2, t);

            t = new QTableWidgetItem("No");
            ui->tableWidget->setItem(row_index, 3, t);

            row_index++;

            is_check.insert("rename", true);
        }
        else if(graminfo[i].contains("1") && !is_check.contains("1"))
        {
            QTableWidgetItem* t = new QTableWidgetItem("1");
            ui->tableWidget->setItem(row_index, 0, t);

            if(SQLSTMT[i].contains("insert") || SQLSTMT[i].contains("delete")
                    || SQLSTMT[i].contains("update") || SQLSTMT[i].contains("select"))
            {
                QTableWidgetItem* t1 = new QTableWidgetItem("DML");
                ui->tableWidget->setItem(row_index, 1, t1);
            }
            else
            {
                QTableWidgetItem* t1 = new QTableWidgetItem("DDL");
                ui->tableWidget->setItem(row_index, 1, t1);
            }

            // 兼容情况
            t = new QTableWidgetItem("Yes");
            ui->tableWidget->setItem(row_index, 2, t);

            t = new QTableWidgetItem("No");
            ui->tableWidget->setItem(row_index, 3, t);


            t = new QTableWidgetItem("不支持常量作为布尔值");
            ui->tableWidget->setItem(row_index, 4, t);

            row_index++;

            is_check.insert("1", true);
        }
        else if(graminfo[i].contains("returning") && !is_check.contains("returning"))
        {
            QTableWidgetItem* t = new QTableWidgetItem("returning");
            ui->tableWidget->setItem(row_index, 0, t);

            if(SQLSTMT[i].contains("insert") || SQLSTMT[i].contains("delete")
                    || SQLSTMT[i].contains("update") || SQLSTMT[i].contains("select"))
            {
                QTableWidgetItem* t1 = new QTableWidgetItem("DML");
                ui->tableWidget->setItem(row_index, 1, t1);
            }
            else
            {
                QTableWidgetItem* t1 = new QTableWidgetItem("DDL");
                ui->tableWidget->setItem(row_index, 1, t1);
            }

            // 兼容情况
            t = new QTableWidgetItem("Yes");
            ui->tableWidget->setItem(row_index, 2, t);

            t = new QTableWidgetItem("No");
            ui->tableWidget->setItem(row_index, 3, t);

            row_index++;

            is_check.insert("returning", true);
        }
        else if(graminfo[i].contains("数据库上下文") && !is_check.contains("数据库上下文"))
        {
            QTableWidgetItem* t = new QTableWidgetItem("数据库上下文");
            ui->tableWidget->setItem(row_index, 0, t);

            if(SQLSTMT[i].contains("insert") || SQLSTMT[i].contains("delete")
                    || SQLSTMT[i].contains("update") || SQLSTMT[i].contains("select"))
            {
                QTableWidgetItem* t1 = new QTableWidgetItem("DML");
                ui->tableWidget->setItem(row_index, 1, t1);
            }
            else
            {
                QTableWidgetItem* t1 = new QTableWidgetItem("DDL");
                ui->tableWidget->setItem(row_index, 1, t1);
            }

            // 兼容情况
            t = new QTableWidgetItem("Yes");
            ui->tableWidget->setItem(row_index, 2, t);

            t = new QTableWidgetItem("No");
            ui->tableWidget->setItem(row_index, 3, t);


            t = new QTableWidgetItem("schema_name.dbo.table_name");
            ui->tableWidget->setItem(row_index, 4, t);

            row_index++;

            is_check.insert("数据库上下文", true);
        }
        else if(graminfo[i].contains("不是已定义的系统类型"))
        {
            if(graminfo[i].contains("blob") && !is_check.contains("blob"))
            {
                // 语法点
                QTableWidgetItem* t = new QTableWidgetItem("blob");
                ui->tableWidget->setItem(row_index, 0, t);

                // 类型
                if(SQLSTMT[i].contains("insert") || SQLSTMT[i].contains("delete")
                        || SQLSTMT[i].contains("update") || SQLSTMT[i].contains("select"))
                {
                    QTableWidgetItem* t1 = new QTableWidgetItem("DML");
                    ui->tableWidget->setItem(row_index, 1, t1);
                }
                else
                {
                    QTableWidgetItem* t1 = new QTableWidgetItem("DDL");
                    ui->tableWidget->setItem(row_index, 1, t1);
                }

                // 兼容情况
                t = new QTableWidgetItem("Yes");
                ui->tableWidget->setItem(row_index, 2, t);

                t = new QTableWidgetItem("No");
                ui->tableWidget->setItem(row_index, 3, t);

                row_index++;

                is_check.insert("blob", true);
            }
            if(graminfo[i].contains("mediumint") && !is_check.contains("mediumint"))
            {
                QTableWidgetItem* t = new QTableWidgetItem("mediumint");
                ui->tableWidget->setItem(row_index, 0, t);

                if(SQLSTMT[i].contains("insert") || SQLSTMT[i].contains("delete")
                        || SQLSTMT[i].contains("update") || SQLSTMT[i].contains("select"))
                {
                    QTableWidgetItem* t1 = new QTableWidgetItem("DML");
                    ui->tableWidget->setItem(row_index, 1, t1);
                }
                else
                {
                    QTableWidgetItem* t1 = new QTableWidgetItem("DDL");
                    ui->tableWidget->setItem(row_index, 1, t1);
                }

                // 兼容情况
                t = new QTableWidgetItem("Yes");
                ui->tableWidget->setItem(row_index, 2, t);

                t = new QTableWidgetItem("No");
                ui->tableWidget->setItem(row_index, 3, t);

                row_index++;

                is_check.insert("mediumint", true);
            }
            if(graminfo[i].contains("year") && !is_check.contains("year"))
            {
                QTableWidgetItem* t = new QTableWidgetItem("year");
                ui->tableWidget->setItem(row_index, 0, t);

                if(SQLSTMT[i].contains("insert") || SQLSTMT[i].contains("delete")
                        || SQLSTMT[i].contains("update") || SQLSTMT[i].contains("select"))
                {
                    QTableWidgetItem* t1 = new QTableWidgetItem("DML");
                    ui->tableWidget->setItem(row_index, 1, t1);
                }
                else
                {
                    QTableWidgetItem* t1 = new QTableWidgetItem("DDL");
                    ui->tableWidget->setItem(row_index, 1, t1);
                }

                // 兼容情况
                t = new QTableWidgetItem("Yes");
                ui->tableWidget->setItem(row_index, 2, t);

                t = new QTableWidgetItem("No");
                ui->tableWidget->setItem(row_index, 3, t);

                row_index++;

                is_check.insert("year", true);
            }
        }
        else if(graminfo[i].contains("操作数类型冲突"))
        {

             if(SQLSTMT[i].contains("cast(0 as year)") && !is_check.contains("cast(0 as year)"))
             {
                 QTableWidgetItem* t = new QTableWidgetItem("cast(* as year)");
                 ui->tableWidget->setItem(row_index, 0, t);

                 if(SQLSTMT[i].contains("insert") || SQLSTMT[i].contains("delete")
                         || SQLSTMT[i].contains("update") || SQLSTMT[i].contains("select"))
                 {
                     QTableWidgetItem* t1 = new QTableWidgetItem("DML");
                     ui->tableWidget->setItem(row_index, 1, t1);
                 }
                 else
                 {
                     QTableWidgetItem* t1 = new QTableWidgetItem("DDL");
                     ui->tableWidget->setItem(row_index, 1, t1);
                 }


                 t = new QTableWidgetItem("NO");
                 ui->tableWidget->setItem(row_index, 2, t);

                 t = new QTableWidgetItem("No");
                 ui->tableWidget->setItem(row_index, 3, t);

                 t = new QTableWidgetItem("MySQL不支持year作为cast函数的type类型\nSQLServer没有year类型");
                 ui->tableWidget->setItem(row_index, 4, t);

                 row_index++;
                 is_check.insert("cast(0 as year)", true);

             }
             else if(SQLSTMT[i].contains("cast(0 as text)") && !is_check.contains("cast(0 as text)"))
             {
                 QTableWidgetItem* t = new QTableWidgetItem("cast(* as text)");
                 ui->tableWidget->setItem(row_index, 0, t);

                 if(SQLSTMT[i].contains("insert") || SQLSTMT[i].contains("delete")
                         || SQLSTMT[i].contains("update") || SQLSTMT[i].contains("select"))
                 {
                     QTableWidgetItem* t1 = new QTableWidgetItem("DML");
                     ui->tableWidget->setItem(row_index, 1, t1);
                 }
                 else
                 {
                     QTableWidgetItem* t1 = new QTableWidgetItem("DDL");
                     ui->tableWidget->setItem(row_index, 1, t1);
                 }


                 t = new QTableWidgetItem("NO");
                 ui->tableWidget->setItem(row_index, 2, t);

                 t = new QTableWidgetItem("Yes");
                 ui->tableWidget->setItem(row_index, 3, t);

                 t = new QTableWidgetItem("MySQL不支持text作为cast函数的type类型");
                 ui->tableWidget->setItem(row_index, 4, t);

                 row_index++;
                 is_check.insert("cast(0 as text)", true);
             }
             else if(SQLSTMT[i].contains("cast(0 as varchar)") && !is_check.contains("cast(0 as varchar)"))
             {
                 QTableWidgetItem* t = new QTableWidgetItem("cast(* as text)");
                 ui->tableWidget->setItem(row_index, 0, t);

                 if(SQLSTMT[i].contains("insert") || SQLSTMT[i].contains("delete")
                         || SQLSTMT[i].contains("update") || SQLSTMT[i].contains("select"))
                 {
                     QTableWidgetItem* t1 = new QTableWidgetItem("DML");
                     ui->tableWidget->setItem(row_index, 1, t1);
                 }
                 else
                 {
                     QTableWidgetItem* t1 = new QTableWidgetItem("DDL");
                     ui->tableWidget->setItem(row_index, 1, t1);
                 }

                 t = new QTableWidgetItem("NO");
                 ui->tableWidget->setItem(row_index, 2, t);

                 t = new QTableWidgetItem("Yes");
                 ui->tableWidget->setItem(row_index, 3, t);

                 t = new QTableWidgetItem("MySQL不支持text作为cast函数的type类型");
                 ui->tableWidget->setItem(row_index, 4, t);

                 row_index++;
                 is_check.insert("cast(0 as varchar)", true);
             }
             else if(SQLSTMT[i].contains("cast(0 as timestamp)") && !is_check.contains("cast(0 as timestamp)"))
             {
                 QTableWidgetItem* t = new QTableWidgetItem("cast(* as text)");
                 ui->tableWidget->setItem(row_index, 0, t);

                 if(SQLSTMT[i].contains("insert") || SQLSTMT[i].contains("delete")
                         || SQLSTMT[i].contains("update") || SQLSTMT[i].contains("select"))
                 {
                     QTableWidgetItem* t1 = new QTableWidgetItem("DML");
                     ui->tableWidget->setItem(row_index, 1, t1);
                 }
                 else
                 {
                     QTableWidgetItem* t1 = new QTableWidgetItem("DDL");
                     ui->tableWidget->setItem(row_index, 1, t1);
                 }

                 t = new QTableWidgetItem("NO");
                 ui->tableWidget->setItem(row_index, 2, t);

                 t = new QTableWidgetItem("Yes");
                 ui->tableWidget->setItem(row_index, 3, t);

                 t = new QTableWidgetItem("MySQL不支持text作为cast函数的type类型");
                 ui->tableWidget->setItem(row_index, 4, t);

                 row_index++;
                 is_check.insert("cast(0 as timestamp)", true);
             }
             else if(SQLSTMT[i].contains("cast(0 as blob)") && !is_check.contains("cast(0 as blob)"))
             {
                 QTableWidgetItem* t = new QTableWidgetItem("cast(* as blob)");
                 ui->tableWidget->setItem(row_index, 0, t);

                 if(SQLSTMT[i].contains("insert") || SQLSTMT[i].contains("delete")
                         || SQLSTMT[i].contains("update") || SQLSTMT[i].contains("select"))
                 {
                     QTableWidgetItem* t1 = new QTableWidgetItem("DML");
                     ui->tableWidget->setItem(row_index, 1, t1);
                 }
                 else
                 {
                     QTableWidgetItem* t1 = new QTableWidgetItem("DDL");
                     ui->tableWidget->setItem(row_index, 1, t1);
                 }

                 t = new QTableWidgetItem("NO");
                 ui->tableWidget->setItem(row_index, 2, t);

                 t = new QTableWidgetItem("Yes");
                 ui->tableWidget->setItem(row_index, 3, t);

                 t = new QTableWidgetItem("MySQL不支持blob作为cast函数的type类型");
                 ui->tableWidget->setItem(row_index, 4, t);

                 row_index++;
                 is_check.insert("cast(0 as blob)", true);
             }
             else if(SQLSTMT[i].contains("cast(0 as smallint)") && !is_check.contains("cast(0 as smallint)"))
             {
                 QTableWidgetItem* t = new QTableWidgetItem("cast(* as smallint)");
                 ui->tableWidget->setItem(row_index, 0, t);

                 if(SQLSTMT[i].contains("insert") || SQLSTMT[i].contains("delete")
                         || SQLSTMT[i].contains("update") || SQLSTMT[i].contains("select"))
                 {
                     QTableWidgetItem* t1 = new QTableWidgetItem("DML");
                     ui->tableWidget->setItem(row_index, 1, t1);
                 }
                 else
                 {
                     QTableWidgetItem* t1 = new QTableWidgetItem("DDL");
                     ui->tableWidget->setItem(row_index, 1, t1);
                 }

                 t = new QTableWidgetItem("NO");
                 ui->tableWidget->setItem(row_index, 2, t);

                 t = new QTableWidgetItem("Yes");
                 ui->tableWidget->setItem(row_index, 3, t);

                 t = new QTableWidgetItem("MySQL不支持smallint作为cast函数的type类型");
                 ui->tableWidget->setItem(row_index, 4, t);

                 row_index++;
                 is_check.insert("cast(0 as smallint)", true);
             }
             else if(SQLSTMT[i].contains("cast(0 as tinyint)") && !is_check.contains("cast(0 as tinyint)"))
             {
                 QTableWidgetItem* t = new QTableWidgetItem("cast(* as tinyint)");
                 ui->tableWidget->setItem(row_index, 0, t);

                 if(SQLSTMT[i].contains("insert") || SQLSTMT[i].contains("delete")
                         || SQLSTMT[i].contains("update") || SQLSTMT[i].contains("select"))
                 {
                     QTableWidgetItem* t1 = new QTableWidgetItem("DML");
                     ui->tableWidget->setItem(row_index, 1, t1);
                 }
                 else
                 {
                     QTableWidgetItem* t1 = new QTableWidgetItem("DDL");
                     ui->tableWidget->setItem(row_index, 1, t1);
                 }

                 t = new QTableWidgetItem("NO");
                 ui->tableWidget->setItem(row_index, 2, t);

                 t = new QTableWidgetItem("Yes");
                 ui->tableWidget->setItem(row_index, 3, t);

                 t = new QTableWidgetItem("MySQL不支持tinyint作为cast函数的type类型");
                 ui->tableWidget->setItem(row_index, 4, t);

                 row_index++;
                 is_check.insert("cast(0 as tinyint)", true);
             }
        }
        else if(graminfo[i].contains("显式转换"))
        {

        }
        else if(graminfo[i].contains("desc") && !is_check.contains("desc"))
        {
            QTableWidgetItem* t = new QTableWidgetItem("desc");
            ui->tableWidget->setItem(row_index, 0, t);

            if(SQLSTMT[i].contains("insert") || SQLSTMT[i].contains("delete")
                    || SQLSTMT[i].contains("update") || SQLSTMT[i].contains("select"))
            {
                QTableWidgetItem* t1 = new QTableWidgetItem("DML");
                ui->tableWidget->setItem(row_index, 1, t1);
            }
            else
            {
                QTableWidgetItem* t1 = new QTableWidgetItem("DDL");
                ui->tableWidget->setItem(row_index, 1, t1);
            }

            // 兼容情况
            t = new QTableWidgetItem("Yes");
            ui->tableWidget->setItem(row_index, 2, t);

            t = new QTableWidgetItem("No");
            ui->tableWidget->setItem(row_index, 3, t);

            row_index++;

            is_check.insert("desc", true);
        }
    }
}

// 返回连接信息
void MainWindow::recConn(mysql_conn *m_arg, sqlServer_conn *s_arg, QString mess)
{
    // 根据返回信息做出判断，连接是否成功的进行
    if(mess != "Y")
    {
        ui->textBrowser->append(mess);
        return;
    }
    else
    {
        ui->textBrowser->append("DBMS Connect Successfully!!!");

        // 更改状态
        ui->radioButton->setChecked(true);
        ui->radioButton_2->setChecked(false);
    }

    M = m_arg;
    S = s_arg;

    if(M->ms_is_open && S->ss_is_open)
    {
        // 初始化数据类型测试子线程，并连接槽函数
        dataTh = new DataTypeTest(M, S);
        connect(dataTh, &DataTypeTest::dataTestDone, this, &MainWindow::recDataRes);

        //初始化数据库模式
        schema = std::make_shared<schema_mysql>(M, S, "sakila");
        // 已同时连接MYSQL和SQLServer
        // 设置测试按钮可用
        ui->pushButton_2->setEnabled(true);

        // 初始化数据库
        schema->fill_scope(scope);

        // 初始化语法测试子线程，并连接槽函数
//        gramTh = new gramtest(scope, schema, M, S);
//        connect(gramTh, &gramtest::gramtestDone, this, &MainWindow::recGramRes);
        //构造生成器
        //gen = statement_factory(&scope);

//        gen->out(s);

        //std::cerr << s.str();
    }
    else
    {
        qDebug() << "ERROR!";
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{

    // 系统日志
    //获取系统当前的时间
    QDateTime dateTime= QDateTime::currentDateTime();
    //格式化时间
    QString str = dateTime .toString("yyyy-MM-dd hh:mm:ss");

    ui->textBrowser_2->append(str + "退出系统..." + "\n");

    QString fileName = "./log.txt";

    qDebug() << "aaaa";

    qDebug() << fileName;

    QFile f(fileName);

    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << ("打开文件失败");
    }

    QTextStream out(&f);

    out << ui->textBrowser_2->toPlainText();

    f.close();


    // 退出提示
    QMessageBox mess(QMessageBox::Question,tr("EXIT"),tr("<font color='black'>Are you sure?</font>"));
    QPushButton *okbutton = (mess.addButton(tr("Yes"), QMessageBox::AcceptRole));
    QPushButton *nobutton = (mess.addButton(tr("No"), QMessageBox::RejectRole));

    mess.exec();

    if(mess.clickedButton() == okbutton)
    {
        event->accept();
    }
    else if(mess.clickedButton() == nobutton)
    {
        event->ignore();
    }
}


MainWindow::~MainWindow()
{
    // 释放线程
    dataTh->quit();
    gramTh->quit();

    // 释放窗口
    delete ui;
}

// 连接数据库
void MainWindow::on_pushButton_clicked()
{
    // 系统日志
    //获取系统当前的时间
    QDateTime dateTime= QDateTime::currentDateTime();
    //格式化时间
    QString str = dateTime.toString("yyyy-MM-dd hh:mm:ss");

    ui->textBrowser_2->append(str + "连接数据库..." + "\n");

    // 显示
    pr->show();

    // 设置按钮不可用，避免重复连接
    ui->pushButton->setEnabled(false);
}

// 数据类型测试
void MainWindow::on_pushButton_2_clicked()
{
    if(M == nullptr || S == nullptr)
    {
        QMessageBox::warning(this, "结果", "<font color='black'>未初始化数据库!!!</font>");
        return;
    }

    // 系统日志
    //获取系统当前的时间
    QDateTime dateTime= QDateTime::currentDateTime();
    //格式化时间
    QString str = dateTime .toString("yyyy-MM-dd hh:mm:ss");

    ui->textBrowser_2->append(str + "进行了数据类型测试..." + "\n");

    // 进度窗口展示
    tpr->show();

    //监视异步操作的结果
    pwatcher = new QFutureWatcher<void>;
    //启动动画
    tpr->StartAnimation();

    //异步操作，匿名函数
    future = QtConcurrent::run([=]()
    {
        //QThread::msleep(5000); //模拟耗时操作
        // 注册
        // 启动数据类型测试线程
        qRegisterMetaType<QMap<QString,float> >("QMap<QString,float>");
        qDebug() << "线程已经启动！";
        dataTh->start();
    });
}

// 语法测试
void MainWindow::on_pushButton_3_clicked()
{
    if(M == nullptr || S == nullptr)
    {
        QMessageBox::warning(this, "结果", "<font color='black'>未初始化数据库!!!</font>");
        return;
    }

    // 系统日志
    //获取系统当前的时间
    QDateTime dateTime= QDateTime::currentDateTime();
    //格式化时间
    QString str = dateTime .toString("yyyy-MM-dd hh:mm:ss");

    ui->textBrowser_2->append(str + "进行了语法测试..." + "\n");

    int testNumber = ui->comboBox_2->currentText().toInt();

    if(testNumber > 100)
    {
        QMessageBox::warning(this, "警告", "<font color='black'>测试语句数量过大，可能会导致系统崩溃!!!</font>");
    }

    // 启动进度窗口
    tpr->show();

    // 监视异步操作的结果
    pwatcher = new QFutureWatcher<void>;
    tpr->StartAnimation();

    // 异步操作
    future = QtConcurrent::run([=]()
    {
        // 启动语法测试线程
        qDebug() << "语法测试线程启动！";
        gramTh = new gramtest(scope, schema, M, S, testNumber);
        connect(gramTh, &gramtest::gramtestDone, this, &MainWindow::recGramRes);
        gramTh->start();
    });
}

// 清空控制台
void MainWindow::on_pushButton_4_clicked()
{    
    // 系统日志
    //获取系统当前的时间
    QDateTime dateTime= QDateTime::currentDateTime();
    //格式化时间
    QString str = dateTime .toString("yyyy-MM-dd hh:mm:ss");

    ui->textBrowser_2->append(str + "清空控制台..." + "\n");


    // 清空控制台内容
    ui->textBrowser->clear();
}

// 断开与数据库的连接
void MainWindow::on_pushButton_5_clicked()
{
    // 系统日志
    //获取系统当前的时间
    QDateTime dateTime= QDateTime::currentDateTime();
    //格式化时间
    QString str = dateTime .toString("yyyy-MM-dd hh:mm:ss");

    ui->textBrowser_2->append(str + "断开与数据库的连接..." + "\n");

    // 防止连续的点击

    ui->pushButton_5->setEnabled(false);

    if(M != nullptr &&  M->MySQLdb.open())
    {
        M->MySQLdb.close();
        ui->textBrowser->append("已经与MySQL断开连接!");
        M = nullptr;
    }

    if(S != nullptr && S->SqlServerdb.open())
    {
        S->SqlServerdb.close();
        ui->textBrowser->append("已经与SQLServer断开连接!");
        S = nullptr;
    }

    // 更改状态
    ui->radioButton->setChecked(false);
    ui->radioButton_2->setChecked(true);

    ui->pushButton_5->setEnabled(true);

    // 可以重新进行连接
    ui->pushButton->setEnabled(true);
}


// 将执行的SQL放入文件中，方便后续查看
void MainWindow::on_pushButton_6_clicked()
{

    // 系统日志
    //获取系统当前的时间
    QDateTime dateTime= QDateTime::currentDateTime();
    //格式化时间
    QString str = dateTime .toString("yyyy-MM-dd hh:mm:ss");

    ui->textBrowser_2->append(str + "导出SQL语句..." + "\n");

    if(SQLSTMT.length() == 0)
    {
        int ret = QMessageBox::warning(this, tr("Warning"),
                                        tr("<font color='red'>The grammer test has not been done!!!.\n"
                                           "Please do grammer test firstly!!!</font>"),
                                        QMessageBox::Cancel,
                                        QMessageBox::Cancel);

        return;
    }

    // 禁止连续的点击

    ui->pushButton_6->setEnabled(false);

    QString fileName = QFileDialog::getSaveFileName(NULL,"另存为","../",".sql");
    qDebug() << fileName;

    QFile f(fileName);

    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << ("打开文件失败");
    }

    QTextStream out(&f);

    for(int i = 0; i < SQLSTMT.length(); i++)
    {
        out << SQLSTMT[i] << "\n"
            << "MYSQL:" << mysqlRes[i] << "\n"
            << "SQLServer:" << sqlserRes[i] << "\n\n";
    }

    f.close();

    ui->pushButton_6->setEnabled(true);

}

void MainWindow::on_pushButton_8_clicked()
{
    // 系统日志
    //获取系统当前的时间
    QDateTime dateTime= QDateTime::currentDateTime();
    //格式化时间
    QString str = dateTime .toString("yyyy-MM-dd hh:mm:ss");

    ui->textBrowser_2->append(str + "查询了测试表结构..." + "\n");

    int index = ui->comboBox->currentIndex();

    switch(index)
    {
        case 0:
            ui->textBrowser_4->setText("Field");
            ui->textBrowser_5->setText("Type");
            ui->textBrowser_6->setText("Key");

            ui->textBrowser_4->append("\nactor_id\nfirstname\nlastname\nlast_update");
            ui->textBrowser_5->append("\nsmallint\nvarchar\nvarchar\ntimestamp");
            ui->textBrowser_6->append("\nPRI\n\nMUL\n\n");
            break;
        case 1:
            ui->textBrowser_4->setText("Field");
            ui->textBrowser_5->setText("Type");
            ui->textBrowser_6->setText("Key");


            ui->textBrowser_4->append("\naddress_id\naddress\naddress2\ndistrict\ncity_id\npostal_code\nphone\nlocation\nlast_update");
            ui->textBrowser_5->append("\nsmallint\nvarchar\nvarchar\nvarchar\nsmallint\nvarchar\nvarchar\ngeometry\ntimestamp");
            ui->textBrowser_6->append("\nPRI\n\n\n\nMUL\n\n\nMUL\n");
            break;
        case 2:
            ui->textBrowser_4->setText("Field");
            ui->textBrowser_5->setText("Type");
            ui->textBrowser_6->setText("Key");

            ui->textBrowser_4->append("\ncategory_id\nname\nlast_update");
            ui->textBrowser_5->append("\ntinyint\nvarchar\ntimestamp");
            ui->textBrowser_6->append("\nPRI\n");
            break;
        case 3:
            ui->textBrowser_4->setText("Field");
            ui->textBrowser_5->setText("Type");
            ui->textBrowser_6->setText("Key");

            ui->textBrowser_4->append("\ncity_id\ncity\ncountry_id\nlast_update");
            ui->textBrowser_5->append("\nsmallint\nvarchar\nsmallint\ntimestamp");
            ui->textBrowser_6->append("\nPRI\n\nMUL\n");
            break;

        case 4:
            ui->textBrowser_4->setText("Field");
            ui->textBrowser_5->setText("Type");
            ui->textBrowser_6->setText("Key");

            ui->textBrowser_4->append("\ncountry_id\ncountry\nlast_update");
            ui->textBrowser_5->append("\nsmallint\nvarchar\ntimestamp");
            ui->textBrowser_6->append("\nPRI\n");
            break;
        case 5:
            ui->textBrowser_4->setText("Field");
            ui->textBrowser_5->setText("Type");
            ui->textBrowser_6->setText("Key");

            ui->textBrowser_4->append("\ncustomer_id\nstore_id\nfirst_name\nlast_name\nemail\naddress_id\nactive\ncreate_data\nlast_update");
            ui->textBrowser_5->append("\nsmallint\ntinyint\nvarchar\nvarchar\nvarchar\nsmallint\ntinyint\ndatetime\ntimestamp");
            ui->textBrowser_6->append("\nPRI\nMUL\n\nMUL\n\nMUL\n");
            break;
        case 6:
            ui->textBrowser_4->setText("Field");
            ui->textBrowser_5->setText("Type");
            ui->textBrowser_6->setText("Key");

            ui->textBrowser_4->append("\nfilm_id\ntitle\ndescription\nrelease_year\nlanguage_id\noriginal_language_id\nrental_duration\nrental_rate\nlength\nreplacement_cost\nrating\nspecial_features\nlast_update");
            ui->textBrowser_5->append("\nsmallint\nvarchar\ntext\nyear\ntinyint\ntinyint\ntinyint\ndecimal\nsmallint\ndecimal\nenum\nset\ntimestamp");
            ui->textBrowser_6->append("\nPRI\nMUL\n\n\nMUL\nMUL\n");
            break;
        case 7:
            ui->textBrowser_4->setText("Field");
            ui->textBrowser_5->setText("Type");
            ui->textBrowser_6->setText("Key");

            ui->textBrowser_4->append("\nactor_id\nfilm_id\nlast_update");
            ui->textBrowser_5->append("\nsmallint\nsmallint\ntimestamp");
            ui->textBrowser_6->append("\nPRI\nPRI\n");
            break;
        case 8:
            ui->textBrowser_4->setText("Field");
            ui->textBrowser_5->setText("Type");
            ui->textBrowser_6->setText("Key");

            ui->textBrowser_4->append("\nfilm_id\ncategory_id\nlast_update");
            ui->textBrowser_5->append("\nsmallint\ntinyint\ntimestamp");
            ui->textBrowser_6->append("\nPRI\nPRI\n");
            break;
        case 9:
            ui->textBrowser_4->setText("Field");
            ui->textBrowser_5->setText("Type");
            ui->textBrowser_6->setText("Key");

            ui->textBrowser_4->append("\nfilm_id\ntitle\ndescription");
            ui->textBrowser_5->append("\nsmallint\nvarchar\ntext");
            ui->textBrowser_6->append("\nPRI\nMUL\n");
            break;
        case 10:
            ui->textBrowser_4->setText("Field");
            ui->textBrowser_5->setText("Type");
            ui->textBrowser_6->setText("Key");

            ui->textBrowser_4->append("\ninventory_id\nfilm_id\nstore_id\nlast_update");
            ui->textBrowser_5->append("\nmediumint\nsmallint\ntinyint\ntimestamp");
            ui->textBrowser_6->append("\nPRI\nMUL\nMUL\n");
            break;
        case 11:
            ui->textBrowser_4->setText("Field");
            ui->textBrowser_5->setText("Type");
            ui->textBrowser_6->setText("Key");

            ui->textBrowser_4->append("\nlanguage_id\nname\nlast_update");
            ui->textBrowser_5->append("\ntinyint\nchar\ntimestamp");
            ui->textBrowser_6->append("\nPRI\n");
            break;
        case 12:
            ui->textBrowser_4->setText("Field");
            ui->textBrowser_5->setText("Type");
            ui->textBrowser_6->setText("Key");

            ui->textBrowser_4->append("\npayment_id\ncustomer_id\nstaff_id\nrental_id\namount\npayment_date\nlast_update");
            ui->textBrowser_5->append("\nsmallint\nsmallint\ntinyint\nint\ndecimal\ndatetime\ntimestamp");
            ui->textBrowser_6->append("\nPRI\nMUL\nMUL\nMUL\n");
            break;
        case 13:
            ui->textBrowser_4->setText("Field");
            ui->textBrowser_5->setText("Type");
            ui->textBrowser_6->setText("Key");

            ui->textBrowser_4->append("\nrental_id\nrental_date\ninventory_id\ncustomer_id\nreturn_date\nstaff_id\nlast_update");
            ui->textBrowser_5->append("\nint\ndatetime\nmediumint\nsmallint\ndatetime\ntinyint\ntimestamp");
            ui->textBrowser_6->append("\nPRI\nMUL\nMUL\nMUL\n\nMUL\n");
            break;
        case 14:
            ui->textBrowser_4->setText("Field");
            ui->textBrowser_5->setText("Type");
            ui->textBrowser_6->setText("Key");

            ui->textBrowser_4->append("\nstaff_id\nfirst_name\nlast_name\naddress_id\npicture\nemail\nstore_id\nactive\nusername\npassword\nlast_update");
            ui->textBrowser_5->append("\ntinyint\nvarchar\nvarchar\nsmallint\nblob\nvarchar\ntinyint\ntinyint\nvarchar\nvarchar\ntimestamp");
            ui->textBrowser_6->append("\nPRI\n\n\nMUL\n\n\nMUL\n");
            break;
        case 15:
            ui->textBrowser_4->setText("Field");
            ui->textBrowser_5->setText("Type");
            ui->textBrowser_6->setText("Key");

            ui->textBrowser_4->append("\nstore_id\nmanager_staff_id\naddress_id\nlast_update");
            ui->textBrowser_5->append("\ntinyint\ntinyint\nsmallint\ntimestamp");
            ui->textBrowser_6->append("\nPRI\nUNI\nMUL\n");
            break;
    }
}

// 自主测试
void MainWindow::on_pushButton_7_clicked()
{
    // 系统日志
    //获取系统当前的时间
    QDateTime dateTime= QDateTime::currentDateTime();
    //格式化时间
    QString str = dateTime .toString("yyyy-MM-dd hh:mm:ss");

    ui->textBrowser_2->append(str + "进行了自主测试..." + "\n");

    if(M == nullptr || S == nullptr)
    {
        QMessageBox::warning(this, "结果", "<font color='black'>未初始化数据库!!!</font>");
        return;
    }

    if(M->ms_is_open)
    {
        if(S->ss_is_open)
        {
            QString q = ui->textEdit->toPlainText();

            if(q == NULL)
            {
                QMessageBox::warning(this, "结果", "<font color='black'>内容不能为空!</font>");
            }
            else
            {
                //QSqlQuery myAuto(M->MySQLdb);
                QSqlQuery sAuto(S->SqlServerdb);

                sAuto.prepare(q);
                if(sAuto.exec())
                {
                    QMessageBox::information(this, "结果", "<font color='black'>成功执行!</font>");
                }
                else
                {
                    QMessageBox::information(this, "结果", "<font color='red'>" + sAuto.lastError().text() + "</font>");
                }
            }

        }
        else
        {
            QMessageBox::warning(this, "结果", "<font color='black'>已于SQLServer断开连接</font>");
        }
    }
    else
    {
        QMessageBox::warning(this, "结果", "<font color='black'>已于MySQL断开连接</font>");
    }
}
