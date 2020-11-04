#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <config.h>
#include <directories.h>
#include <QObject>

#include <iostream>


class AppController : public QObject
{
    Q_OBJECT
public:
    explicit AppController(QObject *parent = nullptr);

signals:

private:

    QObject *mainwindow_;
    Config *config_;
    Directories *dirs_;

    unsigned int source_dir_param_id_;
    unsigned int target_dir_param_id_;
    unsigned int source_dir_id_;
    unsigned int target_dir_id_;
    const std::vector<std::string> config_tokens_ { "default_source_dir = ", "default_target_dir = " };
};

#endif // APPCONTROLLER_H
