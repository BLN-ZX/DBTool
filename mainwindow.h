#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "sql.h"

#include "prod.h"
#include "grammar.h"
#include <memory>
#include <typeinfo>
#include <cassert>
#include <sstream>
#include <QFutureWatcher>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

#include "processing.h"
#include "datatypetest.h"
#include "gramtest.h"
#include "testprocess.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void recConn(mysql_conn *m_arg, sqlServer_conn *s_arg, QString mess);

    void recDataRes(QMap<QString, float> res, QString mess);

    void recGramRes(double r, QStringList SQLstmt, QStringList m_res, QStringList s_res, QStringList graminfo, QString mess);

    void closeEvent(QCloseEvent *event);

//signals:
//    void sendPrcess();


private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::MainWindow *ui;

    // 定义数据库的范围
    scope scope;
    // 定义数据库的模式
    shared_ptr<schema> schema;
    // SQL语句生成器
    shared_ptr<prod> gen;
    // SQL语句输出流
    std::ostringstream s;

    // 子窗口，展示连接进度
    Processing *pr;
    // 子窗口，测试进度
    TestProcess *tpr;

    // 监视异步操作的结果
    QFutureWatcher<void> *pwatcher;
    // 异步操作
    QFuture<void> future;

    // mysql连接
    mysql_conn *M;
    // SQLServer连接
    sqlServer_conn *S;

    // 数据类型测试子线程
    DataTypeTest *dataTh;
    gramtest *gramTh;

    //数据类型测试结果
    QMap<QString, float> datares;

    // 预测测试过程中执行的SQL语句
    QStringList SQLSTMT;

    // 每一条SQL语句执行结果
    QStringList mysqlRes;
    QStringList sqlserRes;

};
#endif // MAINWINDOW_H
