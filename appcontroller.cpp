#include "appcontroller.h"
#include "ui_mainwindow.h"


AppController::AppController(MainWindow *mainwindow)
{
    mainwindow_ = mainwindow;

    config_ = new Config();
    source_dir_id_ = config_->AddParam(config_tokens_.at(0));
    target_dir_id_ = config_->AddParam(config_tokens_.at(1));

    if (!config_->ReadConfig())
        std::cout << "Config could not be read" << std::endl;

    dirs_ = new Directories();

    QObject::connect(dirs_, &Directories::DirectoryChanged,
                     this, &AppController::DirectoryChanged);

    source_dir_id_ = dirs_->AddDirectory(config_->GetParam(source_dir_id_));
    target_dir_id_ = dirs_->AddDirectory(config_->GetParam(target_dir_id_));

    if(source_dir_id_ == -1u)
        source_dir_id_ = dirs_->AddDirectory(std::filesystem::current_path());
    if(target_dir_id_ == -1u)
        target_dir_id_ = dirs_->AddDirectory(std::filesystem::current_path());

    files_ = new Files();
}


void AppController::DirectoryChanged(const unsigned int &id)
{
    switch (id)
    {
        case 0:
        mainwindow_->SetSourceLabel(dirs_->GetDirectoryPath(id)); break;
        case 1:
        mainwindow_->SetTargetLabel(dirs_->GetDirectoryPath(id)); break;
    }

}
