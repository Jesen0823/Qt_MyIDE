#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <stdlib.h>
#include <QTextCodec>
#include <QFileDialog> // file select dialog
#include <string.h>

#define cout qDebug()

// 定义枚举
enum MyCode{
  utf_8, gbk
};

enum MyCode flag;

/*
字符编码：
1.如果给控件设置中文，必须utf-8编码
2.如果从Qt得到的字符串有中文，也必须utf-8
3.如果使用标准c函数，如果有中文必须是gbk编码
*/

QTextCodec *codec;

QString path;

// 如果封装函数，函数需要ui控件，类中声明，类外定义，定义时加上作用域
void MainWindow::testUI(){
    // set QLabel  count
    ui->label->setText("I setText to label");

    // get QLabel count
    QString str;
    str = ui->label->text();
    cout << "str = " << str;

    // use QTextEdit
    ui->textEdit->setText("int main(int argc, char* argv[]){\n\treturn 0;\n}");
    str = ui->textEdit->toPlainText();
    cout << "edit content is:" << str;
}

void MainWindow::testCodec(){
    // 要想显示中文必须为utf-8
    //ui->label->setText("编码");

    // gbk转utf-8
    codec = QTextCodec::codecForName("GBK");
    ui->label->setText(codec->toUnicode("中文显示正常"));

    /* // 编码转换
    // gbk-> utf-8
    codec->toUnicode();
    // utf-8->gbk
    codec->fromUnicode();
    */
}

int *p;

// 构造函数，初始化工作;窗口显示前自动调用
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //qDebug() << "MainWindow...";
    cout << "cout MainWindow...";

    p = NULL;
    p = (int *)malloc(sizeof(int));
    *p = 10;

    //testUI();
    // testCodec();

    // 以上均为测试代码
    // 初始化，默认以utf-8显示
    flag = utf_8;
    codec = QTextCodec::codecForName("GBK");

}

// 析构函数，在对象结束前关闭时，做自动调用析构函数，做清理工作
MainWindow::~MainWindow()
{
    delete ui;

    if(p != NULL){
        free(p);
        p = NULL;
        cout << "free malloc p.";
    }
}

// 槽函数，按钮处理函数
void MainWindow::on_actionutf_8_triggered()
{
    flag = utf_8;
    ui->label->setText(codec->toUnicode("当前编码 utf-8"));
}

void MainWindow::on_actiongbk_triggered()
{
    flag = gbk;
    ui->label->setText("gbk");
}

void MainWindow::on_actionopen_triggered()
{
    // open file select dialog
    path = QFileDialog::getOpenFileName();
    cout << "file path =" << path;
    if(path.isEmpty()){
        return;
    }
    // utf-8 code, if the file contain Chinese, concert to GBK
    // convert QString to char*
    char *fileName = codec->fromUnicode(path).data();
    cout << "fileName=" << fileName;
    FILE *fp = fopen(fileName, "rb");
    if(fp == NULL){
        cout << "on_actionopen_triggered filed.";
        return;
    }

    // read file content
    char buf[1024];
    QString str = "";
    while(1)
    {
        memset(buf, 0, sizeof (buf));
        fgets(buf, sizeof (buf), fp);
        if(flag == utf_8){
            str = str + buf;
        }else if(flag == gbk){
            str = str + codec->toUnicode(buf);
        }

        if(feof(fp))
        {
            break;
        }
    }

    ui->textEdit->setText(str);

    // close file
    fclose(fp);
}

void MainWindow::on_actionsaveas_triggered()
{
    // 1. choice a path to save file
    path = QFileDialog::getSaveFileName();
    if(path.isEmpty())
    {
        return;
    }

    // 2. utf-8 to gbk,and QString to char*
    char *fileName = codec->fromUnicode(path).data();
    cout << "save as, path:" << fileName;

    // 3. open file, get path is char*, To gbk if contain Chinese
    FILE *fp = fopen(fileName, "wb");
    if(fp == NULL)
    {
        cout << "on_actionsaveas_triggered fopen err";
        return;
    }

    // 4. get editing content QString and convert it to char*
    QString str = ui->textEdit->toPlainText();
    const char *buf = str.toStdString().data();

    // 5. put contain to file
    fputs(buf, fp);

    // 6. close file
    fclose(fp);
}

void MainWindow::on_actionundo_triggered()
{
    ui->textEdit->undo();
}


void MainWindow::on_actioncopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionposte_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_actioncut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actioncomplier_triggered()
{
    if(path.isEmpty())
    {
        return;
    }
    cout << "compile path = " << path;
    QString demo = path;
    demo.replace(".c", ""); // output path

    QString cmd = QString("gcc %1 -o %2").arg(path).arg(demo);
    cout << "cmd = " << cmd;

    // to gbk and QString to char* if contain Chinese
    int ret = system(codec->fromUnicode(cmd).data());
    cout << "compile ret:" << ret;

    if(ret != 0){
        cout << "compile err. result:" << ret;
        return;
    }

    // running the program
    QString target = demo;
    system(codec->fromUnicode(demo).data());

}

void MainWindow::on_actionexit_triggered()
{
    exit(0);
}

void MainWindow::on_actionsave_triggered()
{
    // if path is empty,need choice a path
    cout << "save, path :" << path;
    if(path.isEmpty()){
        path = QFileDialog::getSaveFileName();
        cout << "save,2, path :" << path;
        if(path.isEmpty()){
            cout << "path is empty.";
            return;
        }
    }
    // if path is valid, then save content to the path
    char *fileName = codec->fromUnicode(path).data();
    cout << "save, fileName :" << fileName;
    FILE *fp = fopen(fileName, "wb");
    if(fp == NULL)
    {
        cout << "save, fopen err.";
        return;
    }
    QString str = ui->textEdit->toPlainText();
    const char *buf = codec->fromUnicode(str).data();
    fputs(buf, fp);

    fclose(fp);
}

void MainWindow::on_actionnew_triggered()
{
    ui->textEdit->clear();
    path = "";
}
