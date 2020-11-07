#ifndef FILES_H
#define FILES_H

#include <QObject>
#include <filesystem>
#include <iostream>

class Files : public QObject
{
    Q_OBJECT
public:
    Files();
    void SetCopyList(std::string &path);

private:
    typedef struct {
        std::vector<std::string> name;
        std::vector<std::string> path;
    } metadata;

    metadata metadata_;
    QList<QString> GetFolderList();
    QList<QString> GetFileList();


};

#endif // FILES_H
