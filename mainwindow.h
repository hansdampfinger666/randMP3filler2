#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once
#include <format.h>
#include <appoptions.h>
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
    void SetTargetUsedSpace(const float bytes);
    void SetTargetFreeSpace(const float bytes);
    void SetTargetFillSpace(const float bytes);
    void SetMainThreadBusy(const bool flag);
    void SetListStatusBarVisible(const bool flag);
    void SetListStatusBarValue(const int val);
    void SetCopyStatusBarVisible(const bool flag);
    void SetCopyStatusBarValue(const int val);

signals:
    void GUICreateCopyList();
    void GUICopyList();
    void GUISourceDirChanged(const std::string dir);
    void GUITargetDirChanged(const std::string dir);
    void GUIFillPercentOfFree(const QString percent);

private:
    Ui::MainWindow *ui;
    AppOptions app_options_;
    QString source_dir_;
    QString target_dir_;

    std::string DirectoryDialog(const QString &initial_dir);
    void OpenAppOptionsMenu();
};
#endif // MAINWINDOW_H
