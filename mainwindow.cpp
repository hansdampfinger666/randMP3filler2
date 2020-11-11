#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) 
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(this->ui->pb_crt_copy_list, &QPushButton::clicked,
                     this, [&]{ emit CreateCopyList(); });
    QObject::connect(this->ui->pb_copy_list, &QPushButton::clicked,
                     this, [&]{ emit CopyList(); });
    QObject::connect(this->ui->pb_chg_source_dir, &QPushButton::clicked,
                     this, [&]{ emit GUISourceDirChanged(DirectoryDialog(source_dir_)); });
    QObject::connect(this->ui->pb_chg_target_dir, &QPushButton::clicked,
                     this, [&]{ emit GUITargetDirChanged(DirectoryDialog(target_dir_)); });
    QObject::connect(this->ui->edit_prc_fill_up, &QLineEdit::editingFinished,
                     this, [&]{ emit GUIFillPercentOfFree(this->ui->edit_prc_fill_up->text()); });

    val_int_ = new QIntValidator(this);
    this->ui->edit_prc_fill_up->setValidator(val_int_);
}

MainWindow::~MainWindow(){ delete ui; }


void MainWindow::SetSourceLabel(const std::string &txt)
{
    source_dir_ = QString::fromStdString(txt);
    this->ui->lbl_source->setText(source_dir_);
}


void MainWindow::SetTargetLabel(const std::string &txt)
{
    target_dir_ = QString::fromStdString(txt);
    this->ui->lbl_target->setText(target_dir_);
}


std::string MainWindow::DirectoryDialog(const QString &initial_dir)
{
    QFileDialog dialog;
    dialog.setOption(QFileDialog::ShowDirsOnly);
    if(!initial_dir.isEmpty())
        dialog.setDirectory(initial_dir);
    dialog.exec();
    QObject::connect(&dialog, &QFileDialog::fileSelected,
                     &dialog, &QFileDialog::close);
    return dialog.selectedFiles().at(0).toStdString();
}


void MainWindow::SetTargetUsedSpace(const float &bytes)
{
    std::string txt = Format::GetReadableBytes(bytes);
    this->ui->used->setText(QString::fromStdString(txt));
}


void MainWindow::SetTargetFreeSpace(const float &bytes)
{
    std::string txt = Format::GetReadableBytes(bytes);
    this->ui->free->setText(QString::fromStdString(txt));
}


void MainWindow::SetTargetFillSpace(const float &bytes)
{
    std::string txt = Format::GetReadableBytes(bytes);
    this->ui->fill_space->setText(QString::fromStdString(txt));
}

