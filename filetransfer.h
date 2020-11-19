#ifndef FILES_H
#define FILES_H

#include <QObject>
#include <format.h>
#include <filesystem>
#include <iostream>
#include <random.h>
#include <vector>
#include <chrono>

class FileTransfer : public QObject
{
    Q_OBJECT

public:

    typedef struct {
        int qty;
        int duplicate_hits;
        float total_size;
        float runtime;
        std::vector<std::string> paths;
        std::vector<float> sizes;
    } folders;

    void SetTransferSize(const float &size){ copy_size_ = size; };
    void SetCopyList(const std::string &source_path, const int &file_depth);
    void TransferFiles(const std::string &target_path);
    float GetTransferSize(){ return copy_size_; };
    void ResetTransferList();

signals:
    void ReportListStatus(const float &size);
    void ReportCopyStatus(const float &size);

private:

    folders folders_ { 0, 0, 0, 0, {}, {} };
    Random *randomizer_ = nullptr;
    float copy_size_ = 0;
    int file_depth_ = 1;    //built for minimum file depth of 1: root/folder_depth=1/filedepth=2

    int CountSubfolders(const std::string &path);
    std::string GetSubPathNameByIndex(const std::string &root_path, const int &id);
    float GetFileSizesInFolder(const std::string &path);
    bool FolderContainsFiles(const std::string &path);
    bool IsDuplicateFolder(const std::string &path);


    void PrintTransferList();

};

#endif // FILES_H
