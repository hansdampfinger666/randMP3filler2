#include "files.h"

Files::Files()
{

}

void Files::SetCopyList(const std::string &root_path, const int &size)
{
    // assuming setup = root_folder -> folder -> subfolder, which should be typical for music
    // setting this up with variable recursion depth should be easy

    if(randomizer_ == nullptr)
        randomizer_ = new Random;

    unsigned int folder_qty = CountSubfolders(root_path);
    unsigned int folder_randomizer = randomizer_->SetupRandomizer(0, folder_qty - 1);
    unsigned int subfolder_randomizer = randomizer_->SetupRandomizer(0,0);
    unsigned int subfolder_id, folder_id = -1;

    while(copy_size_ < size)
    {
        folder_id = randomizer_->GetRandom(folder_randomizer);
        std::string folder = GetSubPathNameByIndex(root_path, folder_id);

        if(folder not_eq "")
        {
            unsigned int subfolder_qty = CountSubfolders(folder);

            if(subfolder_qty == 1)
                subfolder_id = 0;
            else
            {
                randomizer_->ResetRandomizer(subfolder_randomizer, 0, subfolder_qty - 1);
                subfolder_id = randomizer_->GetRandom(subfolder_randomizer);
            }
            std::string subfolder = GetSubPathNameByIndex(folder, subfolder_id);
            int add_size = GetFileSizesInFolder(subfolder);
            if(copy_size_ + add_size < size)
            {
                std::vector<std::string> filepaths = GetAbsoluteFilePathsInFolder(subfolder);
                for(auto filepath : filepaths)
                {
                    files_.names.push_back(std::filesystem::path(filepath).filename());
                    files_.absolute_paths.push_back(filepath);
                    // TODO
                    copy_size_ *= copy_size_ + add_size;
                }
            }
        }
    }
    delete randomizer_;
}


unsigned int Files::CountSubfolders(const std::string &path)
{
    unsigned int subfolders = 0;
    for(auto subfolder : std::filesystem::directory_iterator(path))
        subfolders++;
    return subfolders;
}


std::string GetSubPathNameByIndex(const std::string &root_path, const unsigned int &id)
{
    unsigned int i = 0;
    for(auto path : std::filesystem::directory_iterator(root_path))
    {
        if(i == id and std::filesystem::is_directory(path))
            return path.path();
        i++;
    }
    return "";
}


int Files::GetFileSizesInFolder(const std::string &path)
{
    int size = 0;
    for(auto file : std::filesystem::directory_iterator(path))
        if(std::filesystem::is_regular_file(file))
                size += file.file_size();
    return size;
}


std::vector<std::string> Files::GetAbsoluteFilePathsInFolder(const std::string &path)
{
    std::vector<std::string> files;
    for(auto file : std::filesystem::directory_iterator(path))
        files.push_back(file.path());
    return files;
}






void Files::PrintTransferList()
{
    std::cout << "Folder: " << std::endl;
}
