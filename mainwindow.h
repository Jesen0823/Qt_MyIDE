#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr); // 改造函数
    ~MainWindow(); // 析构函数

    // 自定义函数
    void testUI();
    void testCodec();

private slots:
    void on_actionutf_8_triggered();

    void on_actiongbk_triggered();

    void on_actionopen_triggered();

    void on_actionsaveas_triggered();

    void on_actionundo_triggered();

    void on_actioncopy_triggered();

    void on_actionposte_triggered();

    void on_actioncut_triggered();

    void on_actioncomplier_triggered();

    void on_actionexit_triggered();

    void on_actionsave_triggered();

    void on_actionnew_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
