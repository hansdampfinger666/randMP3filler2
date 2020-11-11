#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <mainwindow.h>
#include <config.h>
#include <directories.h>
#include <filetransfer.h>
#include <format.h>
#include <QObject>

#include <iostream>


class AppController : public QObject
{
    Q_OBJECT

public:
    explicit AppController(MainWindow *mainwindow);

private slots:

    void DirectoryChanged(const int &dir_id);
    void DriveChanged(const int &dir_id);
    void GUISourceDirChanged(const std::string dir);
    void GUITargetDirChanged(const std::string dir);
    void GUIFillPercentOfFree(const QString percent);
    void CreateCopylist();
    void CopyList();

private:

    MainWindow *mainwindow_;
    Config config_;
    Directories dirs_;
    FileTransfer filetransfer_;

    int source_dir_param_id_;
    int target_dir_param_id_;

    int source_dir_id_;
    int target_dir_id_;
    int source_drive_id_;
    int target_drive_id_;
    const std::vector<std::string> config_tokens_ { "default_source_dir = ", "default_target_dir = " };

    void SetupMainwindow(MainWindow *mainwindow);
    void SetupConfig();
    void SetupDirectories();
};

#endif // APPCONTROLLER_H
