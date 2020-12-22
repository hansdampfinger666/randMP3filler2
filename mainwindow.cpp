#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) 
    , ui(new Ui::MainWindow){

    ui->setupUi(this);
    this->ui->prog_bar_list->setVisible(false);
    this->ui->prog_bar_copy->setVisible(false);

    QObject::connect(this->ui->pb_crt_copy_list, &QPushButton::clicked,
                     this, [&]{ emit GUICreateCopyList(); });
    QObject::connect(this->ui->pb_copy_list, &QPushButton::clicked,
                     this, [&]{ emit GUICopyList(); });
    QObject::connect(
                this->ui->pb_chg_source_dir, &QPushButton::clicked,
                this, [&]{
        emit GUISourceDirChanged(DirectoryDialog(source_dir_)); });
    QObject::connect(this->ui->pb_chg_target_dir, &QPushButton::clicked,
                     this, [&]{
        emit GUITargetDirChanged(DirectoryDialog(target_dir_)); });
    QObject::connect(this->ui->edit_prc_fill_up, &QLineEdit::editingFinished,
                     this, [&]{
        emit GUIFillPercentOfFree(this->ui->edit_prc_fill_up->text()); });
    QObject::connect(this->ui->act_app_options, &QAction::triggered,
                     this, [&]{
        emit GUIOpenAppOptionsMenu(); });
    QObject::connect(this->app_options_.ui->cb_respect_last_transfer,
                     &QCheckBox::stateChanged,
                     this, [&]{
        bool respect_last_transfer =
                this->app_options_.ui->cb_respect_last_transfer->isChecked();

        emit GUIRespectLastTransferListChanged(respect_last_transfer); });

    QIntValidator val_int;
    this->ui->edit_prc_fill_up->setValidator(&val_int);
}

void MainWindow::SetSourceLabel(const std::string &txt){
    source_dir_ = QString::fromStdString(txt);
    this->ui->lbl_source->setText(source_dir_);
}

void MainWindow::SetTargetLabel(const std::string &txt){
    target_dir_ = QString::fromStdString(txt);
    this->ui->lbl_target->setText(target_dir_);
}

std::string MainWindow::DirectoryDialog(const QString &initial_dir){
    QFileDialog dialog;
    dialog.setOption(QFileDialog::ShowDirsOnly);
    if(!initial_dir.isEmpty())
        dialog.setDirectory(initial_dir);
    dialog.exec();
    QObject::connect(&dialog, &QFileDialog::fileSelected,
                     &dialog, &QFileDialog::close);
    return dialog.selectedFiles().at(0).toStdString();
}

void MainWindow::SetTargetUsedSpace(const float bytes){
    std::string txt = Format::GetReadableBytes(bytes);
    this->ui->used->setText(QString::fromStdString(txt));
}

void MainWindow::SetTargetFreeSpace(const float bytes){
    std::string txt = Format::GetReadableBytes(bytes);
    this->ui->free->setText(QString::fromStdString(txt));
}

void MainWindow::SetTargetFillSpace(const float bytes){
    std::string txt = Format::GetReadableBytes(bytes);
    this->ui->fill_space->setText(QString::fromStdString(txt));
}

void MainWindow::SetMainThreadBusy(const bool flag){
    if(flag)
        QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    else
        QGuiApplication::restoreOverrideCursor();
}

void MainWindow::SetListStatusBarVisible(const bool flag){
    this->ui->prog_bar_list->setVisible(flag);
}

void MainWindow::SetListStatusBarValue(const int val){
    this->ui->prog_bar_list->setValue(val);
}

void MainWindow::SetCopyStatusBarVisible(const bool flag){
    this->ui->prog_bar_copy->setVisible(flag);
}

void MainWindow::SetCopyStatusBarValue(const int val){
    this->ui->prog_bar_copy->setValue(val);
}

void MainWindow::OpenAppOptionsMenu(const Config::Data *config_data){

    this->app_options_.ui->cb_respect_last_transfer->setChecked(
                config_data->avoid_last_file_list);
    app_options_.exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}
