#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <format.h>
#include <QMainWindow>
#include <QFileDialog>
#include <QIntValidator>
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
    void SetTargetUsedSpace(const float &bytes);
    void SetTargetFreeSpace(const float &bytes);
    void SetTargetFillSpace(const float &bytes);

signals:
    void CreateCopyList();
    void CopyList();
    void GUISourceDirChanged(std::string dir);
    void GUITargetDirChanged(std::string dir);
    void GUIFillPercentOfFree(QString percent);

private:
    QIntValidator *val_int_;

    Ui::MainWindow *ui;
    QString source_dir_;
    QString target_dir_;

    std::string DirectoryDialog(const QString &initial_dir);
};
#endif // MAINWINDOW_H
