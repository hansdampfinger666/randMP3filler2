#include "filetransfer.h"

void FileTransfer::SetCopyList(const std::string &source_path,
                               const std::string &target_path,
                               const int file_depth,
                               const bool avoid_last_list){

    // multi-threading seems pretty effective to parallelize idle times for
    // I/O or transmission latency for server calls of std::filesystem
    // especially if it's a remote filesystem (over wireless LAN or the net):
    // optimal relation between thread count, bucket size
    // tolerances and +- distribution, average source folder
    // size, ratio between available source folders/target transfer size
    // etc. has to be formalized to get consistently high performance
    // the primary thing to reduce is iterations without size changes (i.e.
    // idle std::filesystem queries, without a folder hit)
    // because trying to repeatedly get better data sets from the server/disk is
    // painfully slow, just get more than we need and stop, then discard it
    // afterwards if not needed and sifting through data we already have on the
    // local machine is very fast
    // TODO: get random samples for unkown source folders, to gauge avg.
    //      folder size
    //  optimize threading options with that information
    //  remember known source folder attributes and serialize as part of the
    // application options
    //  this makes sense for common use cases, maybe add option to reanalyze
    // source folder manually

    source_path_ = source_path;
    target_path_ = target_path;
    file_depth_ = file_depth;
    avoid_last_list_ = avoid_last_list;

    int top_dir_qty = CountSubfolders(source_path);
    existing_target_folders_ = GetExistingFolders(target_path);

    // formalize these params
    // GetRandomFolderSample()
    int number_threads = 32;
    float bucket_size = copy_size_ / number_threads;
    float size_tolerance = 200000000;
    folders_.thread_qty = number_threads;

    std::vector<std::jthread*> threads(number_threads);
    std::vector<Bucket*> buckets(number_threads);

    auto start = std::chrono::steady_clock::now();

    for(int i = 0; i < number_threads; i++){
        buckets.at(i) = new Bucket { 0, 0, 0, 0, 0, 0, 0, 0, {}, {} };
        threads.at(i) = new std::jthread(&FileTransfer::FillBucketList, this,
                                         buckets.at(i), bucket_size,
                                         size_tolerance,
                                         top_dir_qty, source_path_,
                                         avoid_last_list_);
    }

    int joined_threads = 0;
    while(joined_threads < number_threads)
        for(int thread_index = 0;
            thread_index < number_threads; thread_index++){
            if(threads.at(thread_index)->joinable()){
                threads.at(thread_index)->join();
                joined_threads++;
            }
        }

    auto end = std::chrono::steady_clock::now();
    folders_.total_runtime = std::chrono::duration_cast<
            std::chrono::nanoseconds>(
                end - start).count();

    for(auto bucket : buckets){
        for(int i = 0; i < bucket->qty; i++){
            if(folders_.total_size + bucket->sizes.at(i) > copy_size_)
                continue;
            if(not IsDuplicateFolder(bucket->paths.at(i), folders_.paths)){
                folders_.qty++;
                folders_.paths.push_back(bucket->paths.at(i));
                folders_.sizes.push_back(bucket->sizes.at(i));
                folders_.total_size += bucket->sizes.at(i);
            }
        }
        folders_.duplicate_hits += bucket->duplicate_hits;
        folders_.last_list_duplicate_hits += bucket->last_list_duplicate_hits;
        folders_.exists_in_target_hits += bucket->exists_in_target_hits;
        folders_.iterations += bucket->iterations;
        folders_.iterations_without_size_chg +=
                bucket->iterations_without_size_chg;
        folders_.avg_thread_runtime += bucket->thread_runtime;
    }
    folders_.avg_thread_runtime /= number_threads;

    auto end_merge = std::chrono::steady_clock::now();
    auto end_total = std::chrono::duration_cast<std::chrono::nanoseconds>(
                end_merge - end).count();

    folders_.thread_merge_runtime = end_total;
    PrintTransferList();

    for(auto bucket : buckets)
        delete bucket;
    for(auto thread : threads)
        delete thread;
}

void FileTransfer::FillBucketList(Bucket *bucket, const float bucket_size,
                                  const float size_tolerance,
                                  const int top_dir_qty,
                                  const std::string &source_path,
                                  const bool avoid_last_list){

    auto start = std::chrono::steady_clock::now();
    Random randomizer;

    float total_size_old = 0, total_size_new = 0;
    bucket->iterations_without_size_chg = -1;

    while(bucket->total_size < bucket_size + size_tolerance){

        if(total_size_old == total_size_new)
            bucket->iterations_without_size_chg++;
        else
            total_size_old = total_size_new;

        bucket->iterations++;
        std::string dir = source_path;
        int dir_qty = 0;

        for(int i = 0; i < file_depth_ - 1; i++){
            int subdir_id;

            dir_qty = (dir == source_path) ? top_dir_qty : CountSubfolders(dir);

            if(dir_qty == 0)
                break;
            else
                subdir_id = (dir_qty == 1) ? 0 : randomizer.GetRandom(
                                                 0, dir_qty - 1);

            dir = GetSubPathNameByIndex(dir, subdir_id);

            if(dir == "" or i + 1 != file_depth_ - 1){
                continue;
            }
            if(FolderExistsInTarget(dir)){
                bucket->exists_in_target_hits++;
                break;
            }
            if(IsDuplicateFolder(dir, bucket->paths)){
                bucket->duplicate_hits++;
                break;
            }
            if(avoid_last_list and IsDuplicateFolder(dir, last_transfer_list_)){
                bucket->last_list_duplicate_hits++;
                break;
            }

            float folder_size = GetFileSizesInFolder(dir);

            if(folder_size > 0 and FolderContainsFiles(dir)){
                bucket->qty++;
                bucket->total_size += folder_size;
                bucket->paths.push_back(dir);
                bucket->sizes.push_back(folder_size);
                total_size_new = bucket->total_size;
            }
        }
    }
    auto end = std::chrono::steady_clock::now();
    bucket->thread_runtime = std::chrono::duration_cast<
            std::chrono::nanoseconds>(
                end - start).count();
}

std::vector<std::string> FileTransfer::GetExistingFolders(
        const std::string &path){
    std::vector<std::string> paths;
    paths.push_back(path);

    for(int i = 0; i < file_depth_ - 1; i++)
        paths = GetAllSubPathNames(paths);
    return paths;
}

std::vector<std::string> FileTransfer::GetAllSubPathNames(
        const std::vector<std::string> &paths){

    std::vector<std::string> paths_out;
    for(auto &path : paths)
        for(auto &subfolder : std::filesystem::directory_iterator(path))
            if(std::filesystem::is_directory(subfolder))
                paths_out.push_back(subfolder.path());
    return paths_out;
}

bool FileTransfer::FolderExistsInTarget(const std::string &path){
    int drill_up_depth = file_depth_ - 2;
    std::vector<std::string> folder_struct_source, folder_struct_target;

    folder_struct_source = DrillUpPath(path, drill_up_depth);

    for(auto &target : existing_target_folders_){
        folder_struct_target = DrillUpPath(target, drill_up_depth);
        if(folder_struct_source == folder_struct_target)
            return true;
    }
    return false;
}

std::vector<std::string> FileTransfer::DrillUpPath(const std::string &path,
                                                   const int drill_up_depth){
    std::vector<std::string> folder_struct(drill_up_depth + 1);
    auto path_o = std::filesystem::path(path);
    folder_struct.at(0) = path_o.filename();

    for(int i = 1; i <= drill_up_depth; i++){
        path_o = path_o.parent_path();
        folder_struct.at(i) = path_o.filename();
    }
    return folder_struct;
}

int FileTransfer::CountSubfolders(const std::string &path){
    if(not std::filesystem::is_directory(path))
        return 0;

    int subfolders = 0;
    for(auto &subfolder : std::filesystem::directory_iterator(path))
        if(std::filesystem::is_directory(subfolder))
            subfolders++;
    return subfolders;
}

std::string FileTransfer::GetSubPathNameByIndex(const std::string &root_path,
                                                const int id){
    if(not std::filesystem::is_directory(root_path))
        return "";

    int i = 0;
    for(auto &path : std::filesystem::directory_iterator(root_path)){
        if(path.is_directory())
            if(i == id)
                return path.path();
        i++;
    }
    return "";
}

float FileTransfer::GetFileSizesInFolder(const std::string &path){
    int size = 0;
    if(not std::filesystem::is_directory(path))
        return size;

    for(auto &file : std::filesystem::directory_iterator(path))
        if(std::filesystem::is_regular_file(file))
            size += file.file_size();
    return size;
}

bool FileTransfer::FolderContainsFiles(const std::string &path){
    if(not std::filesystem::is_directory(path))
        return false;;

    for(auto &file : std::filesystem::directory_iterator(path))
        if(std::filesystem::is_regular_file(file))
            return true;
    return false;
}

bool FileTransfer::IsDuplicateFolder(const std::string &path,
                                     const std::vector<std::string>
                                     &comparison_list){
    auto found = find(comparison_list.begin(), comparison_list.end(), path);
    if(found != comparison_list.end())
        return true;
    return false;
}

void FileTransfer::TransferFiles(const std::string &target_path){
    int drill_up_depth = file_depth_ - 2;
    std::vector<std::filesystem::path> folder_struct (file_depth_ - 1);
    int copied_size = 0;
    int index = 0;

    for(auto &folder : folders_.paths){
        auto path = std::filesystem::path(folder);
        folder_struct.at(0) = path;

        for(int i = 1; i <= drill_up_depth; i++){
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

void FileTransfer::ResetTransferList(){
    folders_ = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {}, {} };
}

void FileTransfer::PrintTransferList(){
    std::cout << "______________"
              << std::endl <<
                 "NUMBER DIRS TO COPY: "
              << folders_.qty << std::endl <<
                 "FILE DEPTH: "
              << file_depth_ << std::endl <<
                 "TOTAL SIZE: "
              << Format::GetReadableBytes(folders_.total_size) << std::endl <<
                 "DUPLIACTE HITS: "
              << folders_.duplicate_hits << std::endl <<
                 "ALREADY IN TARGET HITS: "
              << folders_.exists_in_target_hits << std::endl <<
                 "ITERATIONS: "
              << folders_.iterations << std::endl <<
                 "ITERATIONS WITHOUT SIZE CHANGE: "
              << folders_.iterations_without_size_chg << std::endl <<
                 "NUMBER OF THREADS: "
              << folders_.thread_qty << std::endl <<
                 "AVG THREAD RUNTIME: "
              << Format::GetReadableNanoSec(folders_.avg_thread_runtime) <<
                 std::endl <<
                 "______________"
              << std::endl <<
                 "THREAD MERGE TIME: "
              << Format::GetReadableNanoSec(folders_.thread_merge_runtime) <<
                 std::endl <<
                 "TOTAL EXECUTION TIME: "
              << Format::GetReadableNanoSec(folders_.total_runtime) <<
                 std::endl;
}

// single threaded solution for comparisons
//void FileTransfer::SetCopyList(const std::string &source_path, const std::string &target_path,
//                               const int file_depth, const bool avoid_last_list){

//    auto start = std::chrono::steady_clock::now();
//    file_depth_ = file_depth;
//    existing_target_folders_ = GetExistingFolders(target_path);

//    if(randomizer_ == nullptr)
//        randomizer_ = new Random;

//    int top_dir_qty = CountSubfolders(source_path);
//    float total_size_old = 0;
//    float total_size_new = 0;
//    int iterations_without_change = -1;

//    while(folders_.total_size < copy_size_ - 100000000  // 100 mb landing zone
//          and iterations_without_change < 20 * file_depth_ - 1){    // per 1 folder depth minus one there will be one idle run

//        if(total_size_old == total_size_new)
//            iterations_without_change++;
//        else
//            total_size_old = total_size_new;

//        folders_.iterations++;
//        std::string dir = source_path;
//        int dir_qty = 0;

//        for(int i = 0; i < file_depth_ - 1; i++){
//            int subdir_id;

//            if(dir == source_path)
//                dir_qty = top_dir_qty;
//            else
//                dir_qty = CountSubfolders(dir);

//            if(dir_qty == 0)
//                break;
//            else if(dir_qty == 1)
//                subdir_id = 0;
//            else
//                subdir_id = randomizer_->GetRandom(0, dir_qty - 1);

//            dir = GetSubPathNameByIndex(dir, subdir_id);

//            if(dir != "" and i + 1 == file_depth_ - 1){
//                if(FolderExistsInTarget(dir)){
//                    folders_.exists_in_target_hits++;
//                    break;
//                }
//                if(IsDuplicateFolder(dir, folders_.paths)){
//                    folders_.duplicate_hits++;
//                    break;
//                }
//                if(avoid_last_list and IsDuplicateFolder(dir, last_transfer_list_)){
//                    folders_.last_list_duplicate_hits++;
//                    break;
//                }

//                float folder_size = GetFileSizesInFolder(dir);

//                if(folders_.total_size + folder_size <= copy_size_ and folder_size > 0
//                        and FolderContainsFiles(dir)){
//                    folders_.qty++;
//                    folders_.total_size += folder_size;
//                    folders_.paths.push_back(dir);
//                    folders_.sizes.push_back(folder_size);
//                    total_size_new = folders_.total_size;
//                    emit ReportListStatus(folders_.total_size);
//                }
//            }
//        }
//    }
//    folders_.iterations_without_size_chg = iterations_without_change;
//    auto end = std::chrono::steady_clock::now();
//    folders_.runtime = std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count();
//    PrintTransferList();
//}
