#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <appcontroller.h>
#include <QMainWindow>
#include <QFileDialog>
#include <iostream>

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
    void GUISourceDirChanged(std::string &dir);
    void GUITargetDirChanged(std::string &dir);

private:
    Ui::MainWindow *ui;
    QString source_dir_;
    QString target_dir_;

    std::string DirectoryDialog(const QString &initial_dir);
};
#endif // MAINWINDOW_H
