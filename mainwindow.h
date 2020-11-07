#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <appcontroller.h>
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
    void ConnectToController(AppController *app_controller);
    void SetSourceLabel(const std::string &txt);
    void SetTargetLabel(const std::string &txt);

signals:
    void BTCreateCopyList();

private:
    Ui::MainWindow *ui;
    AppController *ac_;
};
#endif // MAINWINDOW_H
