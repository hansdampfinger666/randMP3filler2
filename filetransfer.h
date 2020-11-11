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
        int qty = 0;
        float total_size = 0;
        float runtime = 0;
        std::vector<std::string> paths;
        std::vector<float> sizes;
    } folders;

    void SetTransferSize(const float &size){ copy_size_ = size; };
    void SetCopyList(const std::string &source_path, const int &file_depth);
    void TransferFiles(const std::string &target_path);
    float GetTransferSize(){ return copy_size_; };

private:

    folders folders_;
    Random *randomizer_ {nullptr};
    float copy_size_ = 0;
    int file_depth_ = 1;    //built for minimum file depth of 1: root/folder_depth1/file

    int CountSubfolders(const std::string &path);
    std::string GetSubPathNameByIndex(const std::string &root_path, const int &id);
    float GetFileSizesInFolder(const std::string &path);
    bool FolderContainsFiles(const std::string &path);


    void PrintTransferList();

};

#endif // FILES_H
