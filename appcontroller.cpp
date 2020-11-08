#include "appcontroller.h"


AppController::AppController(MainWindow *mainwindow)
{
    mainwindow_ = mainwindow;

    QObject::connect(mainwindow_, &MainWindow::CreateCopyList,
                     this, &AppController::CreateCopylist);

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

    QObject::connect(&dirs_, &Directories::DirectoryChanged,
                     this, &AppController::DirectoryChanged);
}


void AppController::DirectoryChanged(const unsigned int &id)
{
    switch (id)
    {
        case 0:
        mainwindow_->SetSourceLabel(dirs_.GetDirectoryPath(id)); break;
        case 1:
        mainwindow_->SetTargetLabel(dirs_.GetDirectoryPath(id)); break;
    }

}


void AppController::CreateCopylist()
{
    files_.SetCopyList(dirs_.GetDirectoryPath(source_dir_id_), 1000);
}
