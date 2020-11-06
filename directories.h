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
    } metadata;

    typedef struct{
        std::vector<QString> names;
        std::vector<unsigned long> sizes_free;
        std::vector<unsigned long> sizes_total;
    } drives;

    Directories();
    unsigned int AddDirectory(const std::string &path);
    bool SetDirectoryPath(const unsigned int &id, std::string &path);

    std::string GetDirectoryPath(const unsigned int &id);

    void PrintAllDrives();

signals:
    void DirectoryChanged(const unsigned int &id);
    void DriveChanged(const unsigned int &id);

private:

    metadata metadata_;
    drives drives_;

    const float byte_tib_ = 0.0000000000009094947f;
    const float byte_gib_ = 0.0000000009313225728f;

    unsigned int SetDrive(const std::string &path);

};

#endif // DIRECTORY_H
