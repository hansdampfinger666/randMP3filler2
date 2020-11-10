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
        std::vector<float> sizes_used;
        std::vector<float> sizes_free;
        std::vector<float> sizes_total;
    } drives;

    int AddDirectory();
    bool SetDirectoryPath(const int &id, const std::string &path);
    std::string GetDirPath(const int &dir_id);
    int GetDriveToDir(const int &dir_id);
    float GetDriveUsedSpace(const int &dir_id);
    float GetDriveFreeSpace(const int &dir_id);
    float GetDriveTotalSpace(const int &dir_id);

    void PrintAllDrives();

signals:
    void DirectoryChanged(const int &dir_id);
    void DriveForDirChanged(const int &dir_id);

private:

    dirs dirs_;
    drives drives_;

    int SetDrive(const std::string &path);

};

#endif // DIRECTORY_H
