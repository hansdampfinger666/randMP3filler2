#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QDir>

#include <vector>
#include <filesystem>
#include <fstream>


class Config : public QObject
{
    Q_OBJECT

public:

    typedef struct{
      std::vector<std::string> tokens;
      std::vector<std::string> values;
    } params;

    Config();
    unsigned int add_param(const std::string &param_token);
    bool read_config();
    std::string get_param(const unsigned int &param_index);

private:

    const std::string file_name_lnx_ = "/config";
    std::string path_;
    params params_;

    bool validate_config_line(std::string &line);
    void trim(std::string &str);
};

#endif // CONFIG_H
