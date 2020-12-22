#include "config.h"

Config::Config(){
    path_ = std::filesystem::current_path();
    path_ += file_name_lnx_;
}

void Config::SetAvoidLastFileList(const bool flag){
    data_.avoid_last_file_list = flag;
}

int Config::AddParam(const std::string &param_token){
    params_.tokens.push_back(param_token);
    params_.values.push_back("");
    return params_.tokens.size() - 1;
}

void Config::ReadConfig(int &ec){
    if(not std::filesystem::exists(path_) or params_.tokens.size() == 0){
        ec = Error::Log(1, typeid(this).name(), __func__,
                        "Config file does not exist or no parameter "
                        "tokens were defined");
        return;
    }

    std::ifstream data(path_);
    std::string line;

    while(getline(data, line)){
        if(not ValidateConfigLine(line))
            continue;

        for(size_t i = 0; i < params_.tokens.size(); i++)
            if(line.find(params_.tokens.at(i)) == 0)
                params_.values.at(i) =
                        line.substr(
                            params_.tokens.at(i).size(), line.size() -
                            params_.tokens.at(i).size());
    }
    ec = Error::SetOkay();
}

bool Config::ValidateConfigLine(std::string &line){
    if(line.empty())
        return false;

    Trim(line);

    if((line.find_first_of("\"") or line.find_first_of("//")) == 0)
        return false;
    return true;
}

void Config::Trim(std::string &str){
    str = str.substr(str.find_first_not_of(" "), str.find_last_not_of(" ") + 1);
}

const std::string Config::GetParam(const int param_index){
    if(param_index > (int)params_.values.size())
        return {""};
    return params_.values.at(param_index);
}
