#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <appcontroller.h>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void SetSourceLabel(const std::string &txt);
    void SetTargetLabel(const std::string &txt);

signals:
    void CreateCopyList();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
