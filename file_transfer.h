#ifndef FILES_H
#define FILES_H

#include <QObject>
#include <filesystem>
#include <iostream>
#include <random.h>
#include <vector>

class FileTransfer : public QObject
{
    Q_OBJECT

public:

    typedef struct {
        long total_size = 0;
        std::vector<std::string> paths;
        std::vector<long> sizes;
    } folders;

    void SetCopyList(const std::string &source_path, const long &size, const int &file_depth);
    void TransferFiles(const std::string &target_path);

private:

    folders folders_;
    Random *randomizer_ {nullptr};
    int copy_size_;

    int CountSubfolders(const std::string &path);
    std::string GetSubPathNameByIndex(const std::string &root_path, const int &id);
    long GetFileSizesInFolder(const std::string &path);
    bool FolderContainsFiles(const std::string &path);


    void PrintTransferList();

};

#endif // FILES_H
