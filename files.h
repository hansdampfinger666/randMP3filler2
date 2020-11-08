#ifndef FILES_H
#define FILES_H

#include <QObject>
#include <filesystem>
#include <iostream>
#include <random.h>
#include <vector>

class Files : public QObject
{
    Q_OBJECT

public:

    typedef struct {
        std::vector<unsigned int> id;
        std::vector<unsigned int> parent_id;
        std::vector<unsigned int> size;
        std::vector<unsigned int> detph;
        std::vector<std::string> names;
        std::vector<std::string> absolute_paths;
    } files;

//    typedef struct{
//        std::vector<std::string> names;
//        std::vector<std::string> absolute_paths;
//        std::vector<int> sizes;
//        std::vector<unsigned int> folder_id;
//    } folders;

    Files();
    void SetCopyList(const std::string &root_path, const int &size, const unsigned int &file_depth);

private:

    files files_;
//    folders folders_;
    Random *randomizer_;
    int copy_size_;

    unsigned int CountSubfolders(const std::string &path);
    std::string GetSubPathNameByIndex(const std::string &path, const unsigned int &id);
    int GetFileSizesInFolder(const std::string &path);
    std::vector<std::string> GetAbsoluteFilePathsInFolder(const std::string &path);


    void PrintTransferList();

};

#endif // FILES_H
