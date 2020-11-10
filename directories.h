#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <QObject>
#include <QStorageInfo>
#include <filesystem>

class Directories : public QObject
{
    Q_OBJECT

public:

    typedef struct{
        std::vector<std::string> paths;
        std::vector<int> drive_ids;
    } dirs;

    typedef struct{
        std::vector<QString> names;
        std::vector<long> sizes_free;
        std::vector<long> sizes_total;
    } drives;

    int AddDirectory();
    bool SetDirectoryPath(const int &id, const std::string &path);
    std::string GetDirectoryPath(const int &id);

    void PrintAllDrives();

signals:
    void DirectoryChanged(const int &id);

private:

    dirs dirs_;
    drives drives_;

    const float byte_tib_ = 0.0000000000009094947f;
    const float byte_gib_ = 0.0000000009313225728f;

    int SetDrive(const std::string &path);

};

#endif // DIRECTORY_H
