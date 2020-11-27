#include "filetransfer.h"


void FileTransfer::SetCopyList(const std::string &source_path, const std::string &target_path,
                               const int file_depth, const bool avoid_last_list)
{
    auto start = std::chrono::steady_clock::now();
    file_depth_ = file_depth;
    existing_target_folders_ = GetExistingFolders(target_path);

    if(randomizer_ == nullptr)
        randomizer_ = new Random;

    int top_dir_qty = CountSubfolders(source_path);

    while(folders_.total_size < copy_size_ - 100000000) // 100 mb landing zone
    {
        folders_.iterations++;
        std::string dir = source_path;
        int dir_qty = 0;

        for(int i = 0; i < file_depth_ - 1; i++)
        {
            int subdir_id;

            if(dir == source_path)
                dir_qty = top_dir_qty;
            else
                dir_qty = CountSubfolders(dir);

            if(dir_qty == 0)
                break;
            else if(dir_qty == 1)
                subdir_id = 1;
            else
                subdir_id = randomizer_->GetRandom(0, dir_qty - 1);

            dir = GetSubPathNameByIndex(dir, subdir_id);

            if(dir != "" and i + 1 == file_depth_ - 1)
            {
                if(FolderExistsInTarget(dir))
                {
                    folders_.exists_in_target_hits++;
                    break;
                }
                if(IsDuplicateFolder(dir, folders_.paths))
                {
                    folders_.duplicate_hits++;
                    break;
                }
                if(avoid_last_list and IsDuplicateFolder(dir, last_transfer_list_))
                {
                    folders_.last_list_duplicate_hits++;
                    break;
                }

                float folder_size = GetFileSizesInFolder(dir);

                if(folders_.total_size + folder_size <= copy_size_ and folder_size > 0
                        and FolderContainsFiles(dir))
                {
                    folders_.qty++;
                    folders_.total_size += folder_size;
                    folders_.paths.push_back(dir);
                    folders_.sizes.push_back(folder_size);
                    emit ReportListStatus(folders_.total_size);
                }
            }
        }
    }
    auto end = std::chrono::steady_clock::now();
    folders_.runtime = std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count();
    PrintTransferList();
}


std::vector<std::string> FileTransfer::GetExistingFolders(const std::string &path)
{   
    std::vector<std::string> paths;
    paths.push_back(path);

    for(int i = 0; i < file_depth_ - 1; i++)
        paths = GetAllSubPathNames(paths);
    return paths;
}


std::vector<std::string> FileTransfer::GetAllSubPathNames(const std::vector<std::string> &paths)
{
    std::vector<std::string> paths_out;
    for(auto &path : paths)
        for(auto &subfolder : std::filesystem::directory_iterator(path))
            if(std::filesystem::is_directory(subfolder))
                paths_out.push_back(subfolder.path());
    return paths_out;
}


bool FileTransfer::FolderExistsInTarget(const std::string &path)
{
    int drill_up_depth = file_depth_ - 2;
    std::vector<std::string> folder_struct_source, folder_struct_target;

    folder_struct_source = DrillUpPath(path, drill_up_depth);

    for(auto &target : existing_target_folders_)
    {
        folder_struct_target = DrillUpPath(target, drill_up_depth);
        if(folder_struct_source == folder_struct_target)
            return true;
    }
    return false;
}


std::vector<std::string> FileTransfer::DrillUpPath(const std::string &path, const int drill_up_depth)
{
        std::vector<std::string> folder_struct(drill_up_depth + 1);
        auto path_o = std::filesystem::path(path);
        folder_struct.at(0) = path_o.filename();

        for(int i = 1; i <= drill_up_depth; i++)
        {
            path_o = path_o.parent_path();
            folder_struct.at(i) = path_o.filename();
        }
        return folder_struct;
}


int FileTransfer::CountSubfolders(const std::string &path)
{
    int subfolders = 0;
    for(auto &subfolder : std::filesystem::directory_iterator(path))
        if(std::filesystem::is_directory(subfolder))
            subfolders++;
    return subfolders;
}


std::string FileTransfer::GetSubPathNameByIndex(const std::string &root_path, const int id)
{
    int i = 0;
    for(auto &path : std::filesystem::directory_iterator(root_path))
    {
        if(path.is_directory())
            if(i == id)
                return path.path();
        i++;
    }
    return "";
}


float FileTransfer::GetFileSizesInFolder(const std::string &path)
{
    int size = 0;
    if(not std::filesystem::is_directory(path))
        return size;

    for(auto &file : std::filesystem::directory_iterator(path))
        if(std::filesystem::is_regular_file(file))
                size += file.file_size();
    return size;
}


bool FileTransfer::FolderContainsFiles(const std::string &path)
{
    if(not std::filesystem::is_directory(path))
        return false;;

    for(auto &file : std::filesystem::directory_iterator(path))
        if(std::filesystem::is_regular_file(file))
            return true;
    return false;
}


bool FileTransfer::IsDuplicateFolder(const std::string &path, const std::vector<std::string> &comparison_list)
{
    auto found = find(comparison_list.begin(), comparison_list.end(), path);
    if(found != comparison_list.end())
        return true;
    return false;
}


void FileTransfer::TransferFiles(const std::string &target_path)
{
    int drill_up_depth = file_depth_ - 2;
    std::vector<std::filesystem::path> folder_struct (file_depth_ - 1);
    int copied_size = 0;
    int index = 0;

    for(auto &folder : folders_.paths)
    {
        auto path = std::filesystem::path(folder);
        folder_struct.at(0) = path;

        for(int i = 1; i <= drill_up_depth; i++)
        {
            path = path.parent_path();
            folder_struct.at(i) = path;
        }

        std::filesystem::path target_folder = target_path;
        for(int i = folder_struct.size() - 1; i >= 0 ; i--)
            target_folder /= folder_struct.at(i).filename();

        std::filesystem::create_directories(target_folder);
        std::filesystem::copy(folder, target_folder);
        copied_size += folders_.sizes.at(index);
        emit ReportCopyStatus(copied_size);
        index++;
    }
}


void FileTransfer::ResetTransferList()
{
    folders_ = { 0, 0, 0, 0, 0, 0, 0, {}, {} };
}


void FileTransfer::PrintTransferList()
{
//    for(unsigned long i = 0; i < folders_.paths.size(); i++)
//        std::cout << "Folder: " << folders_.paths.at(i) << " / Size: " << Format::GetReadableBytes(folders_.sizes.at(i)) << std::endl <<
//                    "root_name: " << std::filesystem::path(folders_.paths.at(i)).root_name() << std::endl <<
//                    "root_directory: " << std::filesystem::path(folders_.paths.at(i)).root_directory() << std::endl <<
//                    "root_path: " << std::filesystem::path(folders_.paths.at(i)).root_path() << std::endl <<
//                    "relative_path: " << std::filesystem::path(folders_.paths.at(i)).relative_path() << std::endl <<
//                    "parent_path: " << std::filesystem::path(folders_.paths.at(i)).parent_path() << std::endl <<
//                    "file_name: " << std::filesystem::path(folders_.paths.at(i)).filename() << std::endl <<
//                    "stem: " <<#pragma once
// std::filesystem::path(folders_.paths.at(i)).stem() << std::endl;
    std::cout << "______________" << std::endl <<
                 "NUMBER DIRS TO COPY: " << folders_.qty << std::endl <<
                 "FILE DEPTH: " << file_depth_ << std::endl <<
                 "TOTAL SIZE: " << Format::GetReadableBytes(folders_.total_size) << std::endl <<
                 "DUPLIACTE HITS: " << folders_.duplicate_hits << std::endl <<
                 "ALREADY IN TARGET HITS: " << folders_.exists_in_target_hits << std::endl <<
                 "ITERATIONS: " << folders_.iterations << std::endl <<
                 "EXECUTION TIME: " << Format::GetReadableNanoSec(folders_.runtime) << std::endl <<
                 "______________" << std::endl;
}
