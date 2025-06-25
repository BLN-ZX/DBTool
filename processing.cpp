#include "processing.h"
#include "ui_processing.h"

Processing::Processing(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Processing)
{
    ui->setupUi(this);

    // 初始化进度条
    ui->progressBar->setValue(0);

    // 设置模态
    this->setWindowModality(Qt::ApplicationModal);

    // 设置标题
    setWindowTitle("CONNECT");

    // 设置输入回显
    ui->lineEdit_4->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    ui->lineEdit_8->setEchoMode(QLineEdit::PasswordEchoOnEdit);
}

Processing::~Processing()
{
    delete ui;
}

void Processing::on_pushButton_clicked()
{
    // 初始化进度条
    ui->progressBar->setValue(0);

    // 禁止再次点击
    ui->pushButton->setEnabled(false);

    try {
        QString mess;
        mysqlHostName = ui->lineEdit->text();
        mysqlPort = ui->lineEdit_2->text();
        mysqlUserName = ui->lineEdit_3->text();
        mysqlPassWord = ui->lineEdit_4->text();

        sqlSerHostName = ui->lineEdit_5->text();
        sqlSerDatabaseName = ui->lineEdit_6->text();
        sqlSerUserName = ui->lineEdit_7->text();
        sqlSerPassword = ui->lineEdit_8->text();

        m = new mysql_conn(mysqlHostName, mysqlPort, mysqlUserName, mysqlPassWord);
        s = new sqlServer_conn(sqlSerHostName, sqlSerDatabaseName, sqlSerUserName, sqlSerPassword);

        if(m->ms_is_open && s->ss_is_open)
        {
            int p = 0;
            while(p < 100)
            {
                p = p + d42();
                if(p > 100)
                {
                    p = 100;
                    ui->progressBar->setValue(p);
                    break;
                }
                ui->progressBar->setValue(p);
                // 创建定时器
                ui->progressBar->setFormat(QString("连接进度:%1%").arg(QString::number(ui->progressBar->value())));
                QTime time;
                time.start();
                //等待时间流逝2秒钟
                while(time.elapsed() < 2000)
                    //处理事件
                    QCoreApplication::processEvents();
            }
            ui->progressBar->setFormat("连接成功:100%");

            emit sendConn(m, s, "Y");
        }
        else
        {
            ui->progressBar->setFormat(QString("连接失败!!!"));

            // 返回错误信息
            QString err;
            QSqlError error;

            // MYSQL错误信息
            error=m->MySQLdb.lastError();
            if(error.isValid())//发生错误时isValid()返回true
            {
                 switch (error.type())
                 {
                    case QSqlError::NoError:
                        //qDebug()<<"Mysql无错误";
                        err = "Mysql无错误";
                        break;
                    case QSqlError::ConnectionError:  //连接错语
                        //qDebug()<<error.text();
                        err = error.text();
                        break;
                    case QSqlError::StatementError:   //语句错语
                        //qDebug()<<error.text();
                        err = error.text();
                        break;
                    case QSqlError::TransactionError: //事务错误
                        //qDebug()<<error.text();
                        err = error.text();
                        break;
                    default:                          //未知错误
                        //qDebug()<<error.text();
                        err = error.text();
                        break;
                 }
            }

            // SQLServer错误信息
            error=s->SqlServerdb.lastError();
            if(error.isValid())//发生错误时isValid()返回true
            {
                 switch (error.type())
                 {
                    case QSqlError::NoError:
                        //qDebug()<<"SQLServer无错误";
                        err.append("SQLServer无错误");
                        break;
                    case QSqlError::ConnectionError:  //连接错语
                        //qDebug()<<error.text();
                        err.append(error.text());
                        break;
                    case QSqlError::StatementError:   //语句错语
                        //qDebug()<<error.text();
                        err.append(error.text());
                        break;
                    case QSqlError::TransactionError: //事务错误
                        //qDebug()<<error.text();
                        err.append(error.text());
                        break;
                    default:                          //未知错误
                        //qDebug()<<error.text();
                        err.append(error.text());
                        break;
                 }
            }

           emit sendConn(m, s, err);

        }
    } catch (const QSqlError &amp) {
        //emit sendConn(nullptr, nullptr, amp.text());
    }

    // 连接完毕，释放按钮
    ui->pushButton->setEnabled(true);

}

// Mysql密码显示
void Processing::on_pushButton_2_pressed()
{
    ui->lineEdit_4->setEchoMode(QLineEdit::Normal);
}

void Processing::on_pushButton_2_released()
{
    ui->lineEdit_4->setEchoMode(QLineEdit::PasswordEchoOnEdit);
}


// SQLServer密码显示
void Processing::on_pushButton_3_pressed()
{
    ui->lineEdit_8->setEchoMode(QLineEdit::Normal);
}

void Processing::on_pushButton_3_released()
{
    ui->lineEdit_8->setEchoMode(QLineEdit::PasswordEchoOnEdit);
}
