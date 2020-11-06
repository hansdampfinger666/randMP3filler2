#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) 
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::SetSourceLabel(const std::string &txt)
{
    this->ui->lbl_source->setText(QString::fromStdString(txt));
}


void MainWindow::SetTargetLabel(const std::string &txt)
{
    this->ui->lbl_target->setText(QString::fromStdString(txt));
}


