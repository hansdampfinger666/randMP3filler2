#ifndef APPOPTIONS_H
#define APPOPTIONS_H

#pragma once
#include <x_pch.h>
#include "ui_appoptions.h"


namespace Ui {
class AppOptions;
}

class AppOptions : public QDialog
{
    Q_OBJECT

public:   
    explicit AppOptions(QWidget *parent = nullptr);
    ~AppOptions();

private:
    friend class MainWindow;
    Ui::AppOptions *ui;
};

#endif // APPOPTIONS_H
