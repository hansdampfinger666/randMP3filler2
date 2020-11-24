#include "appcontroller.h"


AppController::AppController(MainWindow *mainwindow)
{
    int ec;
    SetupMainwindow(mainwindow);
    SetupConfig(ec);
    if(ec not_eq 0)
        SetupDirectories(false);
    else
        SetupDirectories(true);
}


void AppController::SetupMainwindow(MainWindow *mainwindow)
{
    mainwindow_ = mainwindow;
    QObject::connect(mainwindow_, &MainWindow::GUICreateCopyList,
                     this, &AppController::CreateCopylist);
    QObject::connect(mainwindow_, &MainWindow::GUICopyList,
                     this, &AppController::CopyList);
    QObject::connect(mainwindow_, &MainWindow::GUISourceDirChanged,
                     this, &AppController::GUISourceDirChanged);
    QObject::connect(mainwindow_, &MainWindow::GUITargetDirChanged,
                     this, &AppController::GUITargetDirChanged);
    QObject::connect(mainwindow_, &MainWindow::GUIFillPercentOfFree,
                     this, &AppController::GUIFillPercentOfFree);
    QObject::connect(&filetransfer_, &FileTransfer::ReportListStatus,
                     this, &AppController::ReceiveListStatus);
    QObject::connect(&filetransfer_, &FileTransfer::ReportCopyStatus,
                     this, &AppController::ReceiveCopyStatus);
}


void AppController::SetupConfig(int &ec)
{
    source_dir_param_id_ = config_.AddParam(config_tokens_.at(0));
    target_dir_param_id_ = config_.AddParam(config_tokens_.at(1));
    config_.ReadConfig(ec);
}


void AppController::SetupDirectories(bool use_config)
{
    QObject::connect(&dirs_, &Directories::DirectoryChanged,
                     this, &AppController::DirectoryChanged);
    QObject::connect(&dirs_, &Directories::DriveForDirChanged,
                     this, &AppController::DriveChanged);

    source_dir_id_ = dirs_.AddDirectory();
    target_dir_id_ = dirs_.AddDirectory();
    std::string source_path, target_path;

    if(use_config)
    {
        source_path = config_.GetParam(source_dir_param_id_);
        target_path = config_.GetParam(target_dir_param_id_);
    }
    else
    {
        source_path = std::filesystem::current_path();
        target_path = std::filesystem::current_path();
    }
    dirs_.SetDirectoryPath(source_dir_id_, source_path);
    dirs_.SetDirectoryPath(target_dir_id_, target_path);
}


void AppController::DirectoryChanged(const int dir_id)
{
    if(dir_id == source_dir_id_)
        mainwindow_->SetSourceLabel(dirs_.GetDirPath(dir_id));
    else if(dir_id == target_dir_id_)
        mainwindow_->SetTargetLabel(dirs_.GetDirPath(dir_id));
}


void AppController::DriveChanged(const int dir_id)
{
    if(dir_id == target_dir_id_)
    {
        float used = dirs_.GetDriveUsedSpace(dir_id);
        float free = dirs_.GetDriveFreeSpace(dir_id);
        mainwindow_->SetTargetUsedSpace(used);
        mainwindow_->SetTargetFreeSpace(free);
    }
}


void AppController::GUISourceDirChanged(const std::string &dir)
{
    dirs_.SetDirectoryPath(0, dir);
}


void AppController::GUITargetDirChanged(const std::string &dir)
{
    dirs_.SetDirectoryPath(1, dir);
}


void AppController::GUIFillPercentOfFree(const QString &percent)
{
    float factor = percent.toFloat() / 100;
    float size = dirs_.GetDriveFreeSpace(target_dir_id_) * factor;
    mainwindow_->SetTargetFillSpace(size);
    filetransfer_.SetTransferSize(size);
}


void AppController::CreateCopylist()
{
    mainwindow_->SetMainThreadBusy(true);
    mainwindow_->SetListStatusBarVisible(true);
    filetransfer_.SetCopyList(dirs_.GetDirPath(source_dir_id_), dirs_.GetDirPath(target_dir_id_), 3);
    mainwindow_->SetMainThreadBusy(false);
    mainwindow_->SetListStatusBarValue(100);
}


void AppController::CopyList()
{
    mainwindow_->SetMainThreadBusy(true);
    mainwindow_->SetCopyStatusBarVisible(true);
    filetransfer_.TransferFiles(dirs_.GetDirPath(target_dir_id_));
    mainwindow_->SetMainThreadBusy(false);
    mainwindow_->SetCopyStatusBarValue(100);
}


void AppController::ReceiveListStatus(const float &size)
{
    int percent = (int)(size / filetransfer_.GetTransferSize() * 100);
    mainwindow_->SetListStatusBarValue(percent);
}


void AppController::ReceiveCopyStatus(const float &size)
{
    int percent = (int)(size / filetransfer_.GetTransferSize() * 100);
    mainwindow_->SetCopyStatusBarValue(percent);
}
