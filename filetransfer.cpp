#include "filetransfer.h"


void FileTransfer::SetCopyList(const std::string &source_path, const int &file_depth)
{
    file_depth_ = file_depth;

    if(randomizer_ == nullptr)
        randomizer_ = new Random;

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    while(folders_.total_size < copy_size_ - 100000000)
        // 100 mb landing zone, size depens on target folders we're dealing with
        // don't want to make it too small, to avoid excessive recursions
        // don't want to make it too large, to avoid excessive deviation from user intent
    {
        std::string dir = source_path;

        for(int i = 0; i < file_depth - 1; i++)
        {
            int dir_qty = CountSubfolders(dir);
            int randomizer_id;
            int subdir_id;

            if(dir_qty == 0)
                break;
            else if(dir_qty == 1)
                subdir_id = 1;
            else
            {
                randomizer_id = randomizer_->GetRandomizer(0, dir_qty - 1);
                if(randomizer_id == -1)
                    randomizer_id = randomizer_->NewRandomizer(0, dir_qty - 1);
                subdir_id = randomizer_->GetRandom(randomizer_id);
            }
            dir = GetSubPathNameByIndex(dir, subdir_id);    

            if(dir != "" and i + 1 == file_depth - 1)
            {
                // if folder is duplicate, get new one, the lesser the diff between available source
                // folders and target space is, the more this will be likely to trigger and the
                // more we need to think about implementing a different randomizing technique
                // because the runtime will increase unacceptably
                if(IsDuplicateFolder(dir))
                    break;

                float folder_size = GetFileSizesInFolder(dir);
                if(folders_.total_size + folder_size <= copy_size_ and folder_size > 0
                        and FolderContainsFiles(dir))
                {
                    folders_.qty++;
                    folders_.total_size += folder_size;
                    folders_.paths.push_back(dir);
                    folders_.sizes.push_back(folder_size);
                }
            }
        }
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    folders_.runtime = std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count();

    PrintTransferList();
}


int FileTransfer::CountSubfolders(const std::string &path)
{
    int subfolders = 0;
    for(auto subfolder : std::filesystem::directory_iterator(path))
        if(std::filesystem::is_directory(subfolder))
            subfolders++;
    return subfolders;
}


std::string FileTransfer::GetSubPathNameByIndex(const std::string &root_path, const int &id)
{
    int i = 0;
    for(auto path : std::filesystem::directory_iterator(root_path))
        if(std::filesystem::is_directory(path))
        {
            if(i == id)
                return path.path();
            i++;
        };
    return "";
}


float FileTransfer::GetFileSizesInFolder(const std::string &path)
{
    int size = 0;
    if(not std::filesystem::is_directory(path))
        return size;

    for(auto file : std::filesystem::directory_iterator(path))
        if(std::filesystem::is_regular_file(file))
                size += file.file_size();
    return size;
}


bool FileTransfer::FolderContainsFiles(const std::string &path)
{
    if(not std::filesystem::is_directory(path))
        return false;;

    for(auto file : std::filesystem::directory_iterator(path))
        if(std::filesystem::is_regular_file(file))
            return true;
    return false;
}


bool FileTransfer::IsDuplicateFolder(const std::string &path)
{
    std::vector<std::string>::iterator found = find(folders_.paths.begin(), folders_.paths.end(), path);
    if(found != folders_.paths.end())
        return true;
    return false;
}


void FileTransfer::TransferFiles(const std::string &target_path)
{
    int resolve_n_times = file_depth_ - 1;

    for(int i = 0; i < resolve_n_times; i++)
    {
        std::string path = std::filesystem::

    }
}




void FileTransfer::PrintTransferList()
{
    for(unsigned long i = 0; i < folders_.paths.size(); i++)
        std::cout << "Folder: " << folders_.paths.at(i) << " / Size: " << Format::GetReadableBytes(folders_.sizes.at(i)) << std::endl;
    std::cout << "______________" << std::endl <<
                 "NUMBER DIRS TO COPY: " << folders_.qty << std::endl <<
                 "FILE DEPTH: " << file_depth_ << std::endl <<
                 "TOTAL SIZE: " << Format::GetReadableBytes(folders_.total_size) << std::endl <<
                 "EXECUTION TIME: " << Format::GetReadableNanoSec(folders_.runtime) << std::endl <<
                 "______________";
}
