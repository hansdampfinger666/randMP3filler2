#include "appoptions.h"

AppOptions::AppOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppOptions){

    ui->setupUi(this);
}

AppOptions::~AppOptions(){
    delete ui;
}
