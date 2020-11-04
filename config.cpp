#include "config.h"

Config::Config()
{
    path_ = std::filesystem::current_path();
    path_ += file_name_lnx_;
}


unsigned int Config::add_param(const std::string &param_token)
{
    params_.tokens.push_back(param_token);
    params_.values.push_back("");
    return params_.tokens.size() - 1;
}


bool Config::read_config()
{    
    if(!std::filesystem::exists(path_) || params_.tokens.size() == 0)
        return false;

    std::ifstream data(path_);
    std::string line;

    while(getline(data, line))
    {
        if(!validate_config_line(line))
            continue;

        for(size_t i = 0; i < params_.tokens.size(); i++)
            if(line.find(params_.tokens.at(i)) == 0)
                params_.values.at(i) = line.substr(params_.tokens.at(i).size(), line.size() - params_.tokens.at(i).size());
    }
    return
            true;
}


bool Config::validate_config_line(std::string &line)
{
    if(line.empty())
        return false;

    trim(line);

    if((line.find_first_of("\"") || line.find_first_of("//")) == 0)
        return false;
    return true;
}


void Config::trim(std::string &str)
{
    str = str.substr(str.find_first_not_of(" "), str.find_last_not_of(" ") + 1);
}


std::string Config::get_param(const unsigned int &param_index)
{
    if(param_index > params_.values.size())
        return {""};
    return params_.values.at(param_index);
}
