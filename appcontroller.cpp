#include "appcontroller.h"
#include "ui_mainwindow.h"


AppController::AppController(QObject *parent) : QObject(parent)
{
    mainwindow_->Ui->setupUi(this);
    mainwindow_ = parent;

    Config config_;
    source_dir_param_id_ = config_.add_param(config_tokens_.at(0));
    target_dir_param_id_ = config_.add_param(config_tokens_.at(1));

    if (!config_.read_config())
        std::cout << "Config could not be read" << std::endl;

    Directories dirs_;
    source_dir_id_ = dirs_.add_directory(config_.get_param(source_dir_id_));
    target_dir_id_ = dirs_.add_directory(config_.get_param(target_dir_id_));

    if(source_dir_id_ == -1u)
        source_dir_id_ = dirs_.add_directory(std::filesystem::current_path());
    if(target_dir_id_ == -1u)
        target_dir_id_ = dirs_.add_directory(std::filesystem::current_path());


    mainwindow_->ui->lbl_target->setText("PLS");

    // now I think we've set up everything from a data lvl, so we can call the GUI to present itself like the cunt that she is

//    QObject::connect(mainwindow_->ui->lbl_target
//    QObject::connect(this->ui->BrowseInboundButton, &QPushButton::clicked,
//                     this, &MainWindow::OnBrowseFromDirButtonClicked);

    dirs_.cout_all_drive_sizes();


}
