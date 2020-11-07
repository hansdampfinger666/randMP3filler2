#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) 
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QObject::connect(this->ui->pb_crt_copy_list, &QPushButton::clicked,
//                     )
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::ConnectToController(AppController *app_controller)
{
    ac_ = app_controller;

    QObject::connect(this, &MainWindow::BTCreateCopyList,
                     this->ac_, &AppController::SetCopyList);
}

void MainWindow::SetSourceLabel(const std::string &txt)
{
    this->ui->lbl_source->setText(QString::fromStdString(txt));
}


void MainWindow::SetTargetLabel(const std::string &txt)
{
    this->ui->lbl_target->setText(QString::fromStdString(txt));
}

