#ifndef FILES_H
#define FILES_H

#pragma once
#include <QObject>
#include <format.h>
#include <filesystem>
#include <iostream>
#include <random.h>
#include <vector>
#include <chrono>
#include <cereal/access.hpp>

#include <benchmark.h>


class FileTransfer : public QObject
{
    Q_OBJECT

public:
    typedef struct {
        int qty;
        int duplicate_hits;
        int last_list_duplicate_hits;
        int exists_in_target_hits;
        int iterations;
        int iterations_without_size_chg;
        float total_size;
        float runtime;
        std::vector<std::string> paths;
        std::vector<float> sizes;
    } Folders;

    void SetTransferSize(const float size){ copy_size_ = size; };
    void SetCopyList(const std::string &source_path, const std::string &target_path,
                     const int file_depth, const bool avoid_last_list);
    void TransferFiles(const std::string &target_path);
    float GetTransferSize(){ return copy_size_; };
    void ResetTransferList();

signals:
    void ReportListStatus(const float size);
    void ReportCopyStatus(const float size);

private:
    Folders folders_ { 0, 0, 0, 0, 0, 0, 0, 0, {}, {} };
    Random *randomizer_ = nullptr;
    float copy_size_ = 0;
    int file_depth_ = 1;    //built for minimum file depth of 2: root=0/folder_depth=1/file_depth=2
    std::vector<std::string> existing_target_folders_ {};
    std::vector<std::string> last_transfer_list_ {};

    std::vector<std::string> GetExistingFolders(const std::string &path);
    bool FolderExistsInTarget(const std::string &path);
    std::vector<std::string> DrillUpPath(const std::string &path, const int drill_up_depth);
    int CountSubfolders(const std::string &path);
    std::string GetSubPathNameByIndex(const std::string &root_path, const int id);
    std::vector<std::string> GetAllSubPathNames(const std::vector<std::string> &paths);
    float GetFileSizesInFolder(const std::string &path);
    bool FolderContainsFiles(const std::string &path);
    bool IsDuplicateFolder(const std::string &path, const std::vector<std::string> &comparison_list);
    void PrintTransferList();

    friend class cereal::access;
    template<class Archive>
    void save(Archive &ar) const { ar(folders_.paths); };
    template<class Archive>
    void load(Archive &ar){ ar(last_transfer_list_); }
};

#endif // FILES_H
