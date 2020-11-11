#include "appcontroller.h"


AppController::AppController(MainWindow *mainwindow)
{
    SetupMainwindow(mainwindow);
    SetupConfig();
    SetupDirectories();
}


void AppController::SetupMainwindow(MainWindow *mainwindow)
{
    mainwindow_ = mainwindow;
    QObject::connect(mainwindow_, &MainWindow::CreateCopyList,
                     this, &AppController::CreateCopylist);
    QObject::connect(mainwindow_, &MainWindow::GUISourceDirChanged,
                     this, &AppController::GUISourceDirChanged);
    QObject::connect(mainwindow_, &MainWindow::GUITargetDirChanged,
                     this, &AppController::GUITargetDirChanged);
    QObject::connect(mainwindow_, &MainWindow::GUIFillPercentOfFree,
                     this, &AppController::GUIFillPercentOfFree);
}


void AppController::SetupConfig()
{
    source_dir_param_id_ = config_.AddParam(config_tokens_.at(0));
    target_dir_param_id_ = config_.AddParam(config_tokens_.at(1));

    if (not config_.ReadConfig())
        std::cout << "Config could not be read" << std::endl;
}


void AppController::SetupDirectories()
{
    QObject::connect(&dirs_, &Directories::DirectoryChanged,
                     this, &AppController::DirectoryChanged);
    QObject::connect(&dirs_, &Directories::DriveForDirChanged,
                     this, &AppController::DriveChanged);

    source_dir_id_ = dirs_.AddDirectory();
    target_dir_id_ = dirs_.AddDirectory();

    std::string source_path = config_.GetParam(source_dir_param_id_);
    if(not dirs_.SetDirectoryPath(source_dir_id_, source_path))
        dirs_.SetDirectoryPath(source_dir_id_, std::filesystem::current_path());

    std::string target_path = config_.GetParam(target_dir_param_id_);
    if(not dirs_.SetDirectoryPath(target_dir_id_, target_path))
        dirs_.SetDirectoryPath(target_dir_id_, std::filesystem::current_path());
}


void AppController::DirectoryChanged(const int &dir_id)
{
    if(dir_id == source_dir_id_)
        mainwindow_->SetSourceLabel(dirs_.GetDirPath(dir_id));
    else if(dir_id == target_dir_id_)
        mainwindow_->SetTargetLabel(dirs_.GetDirPath(dir_id));
}


void AppController::DriveChanged(const int &dir_id)
{
    float used = dirs_.GetDriveUsedSpace(dir_id);
    float free = dirs_.GetDriveFreeSpace(dir_id);

    if(dir_id == source_dir_id_)
    {
    }
    else if (dir_id == target_dir_id_)
    {
        mainwindow_->SetTargetUsedSpace(used);
        mainwindow_->SetTargetFreeSpace(free);
    }
}


void AppController::GUISourceDirChanged(const std::string dir)
{
    dirs_.SetDirectoryPath(0, dir);
}


void AppController::GUITargetDirChanged(const std::string dir)
{
    dirs_.SetDirectoryPath(1, dir);
}


void AppController::GUIFillPercentOfFree(const QString percent)
{
    float factor = percent.toFloat() / 100;
    float size = dirs_.GetDriveFreeSpace(target_dir_id_) * factor;
    mainwindow_->SetTargetFillSpace(size);
    filetransfer_.SetTransferSize(size);
}


void AppController::CreateCopylist()
{
    filetransfer_.SetCopyList(dirs_.GetDirPath(source_dir_id_), 3);
}


void AppController::CopyList()
{
    filetransfer_.TransferFiles("path");
}
