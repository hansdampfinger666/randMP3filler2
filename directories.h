#ifndef DIRECTORY_H
#define DIRECTORY_H

#pragma once
#include <x_pch.h>
#include <format.h>


class Directories : public QObject
{
    Q_OBJECT

public:
    typedef struct{
        std::vector<std::string> paths;
        std::vector<int> drive_ids;
    } Dirs;
    typedef struct{
        std::vector<QString> names;
        std::vector<float> sizes_used;
        std::vector<float> sizes_free;
        std::vector<float> sizes_total;
    } Drives;

    int AddDirectory();
    bool SetDirectoryPath(const int id, const std::string &path);
    std::string GetDirPath(const int dir_id);
    int GetDriveToDir(const int dir_id);
    float GetDriveUsedSpace(const int dir_id);
    float GetDriveFreeSpace(const int dir_id);
    float GetDriveTotalSpace(const int dir_id);
    void PrintAllDrives();

signals:
    void DirectoryChanged(const int dir_id);
    void DriveForDirChanged(const int dir_id);

private:
    Dirs dirs_;
    Drives drives_;

    int SetDrive(const std::string &path);

};

#endif // DIRECTORY_H
