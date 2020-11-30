#include "appoptions.h"
#include "ui_appoptions.h"

AppOptions::AppOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppOptions)
{
    ui->setupUi(this);
}


bool AppOptions::OptionsChanged()
{
    if(options_changed_ == true)
    {
        options_changed_ = false;
        return true;
    }
    return false;
}


AppOptions::~AppOptions()
{
    delete ui;
}
