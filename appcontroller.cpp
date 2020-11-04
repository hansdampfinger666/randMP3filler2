#include "appcontroller.h"

AppController::AppController(QObject *parent) : QObject(parent)
{
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




}
