#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <mainwindow.h>
#include <config.h>
#include <directories.h>
#include <files.h>
#include <QObject>

#include <iostream>


class AppController : public QObject
{
    Q_OBJECT
public:
    explicit AppController(MainWindow *mainwindow);
    void CreateCopyList();
    void SetCopyList();

signals:

private slots:

    void DirectoryChanged(const unsigned int &id);

private:

    MainWindow *mainwindow_;
    Config *config_;
    Directories *dirs_;
    Files *files_;

    unsigned int source_dir_id_;
    unsigned int target_dir_id_;
    const std::vector<std::string> config_tokens_ { "default_source_dir = ", "default_target_dir = " };
};

#endif // APPCONTROLLER_H
