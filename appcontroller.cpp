#include "appcontroller.h"


AppController::AppController(MainWindow *mainwindow)
{
    mainwindow_ = mainwindow;
    QObject::connect(mainwindow_, &MainWindow::CreateCopyList,
                     this, &AppController::CreateCopylist);
    QObject::connect(mainwindow_, &MainWindow::GUISourceDirChanged,
                     this, &AppController::GUISourceDirChanged);
    QObject::connect(mainwindow_, &MainWindow::GUITargetDirChanged,
                     this, &AppController::GUITargetDirChanged);

    source_dir_id_ = config_.AddParam(config_tokens_.at(0));
    target_dir_id_ = config_.AddParam(config_tokens_.at(1));

    if (not config_.ReadConfig())
        std::cout << "Config could not be read" << std::endl;

    QObject::connect(&dirs_, &Directories::DirectoryChanged,
                     this, &AppController::DirectoryChanged);

    source_dir_id_ = dirs_.AddDirectory();
    target_dir_id_ = dirs_.AddDirectory();

    if(not dirs_.SetDirectoryPath(source_dir_id_, config_.GetParam(source_dir_id_)))
        dirs_.SetDirectoryPath(source_dir_id_, std::filesystem::current_path());

    if(not dirs_.SetDirectoryPath(target_dir_id_, config_.GetParam(target_dir_id_)))
        dirs_.SetDirectoryPath(target_dir_id_, std::filesystem::current_path());
}


void AppController::DirectoryChanged(const int &id)
{
    switch (id)
    {
        case 0: mainwindow_->SetSourceLabel(dirs_.GetDirectoryPath(id)); break;
        case 1: mainwindow_->SetTargetLabel(dirs_.GetDirectoryPath(id)); break;
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


void AppController::CreateCopylist()
{
    filetransfer_.SetCopyList(dirs_.GetDirectoryPath(source_dir_id_), 5000000000, 3);
}
