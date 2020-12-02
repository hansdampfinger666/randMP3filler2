#include <error.h>


void Error::Reset(){
    data_ = { 0, "", "", "" };
}

int Error::Log(const int code, const std::string &caller, const std::string &method, const std::string &message){
    Reset();
    if(verbose_logging_active_)
            data_ = { code, typeid(caller).name(), method, message };
    else
        data_.code = code;
    return code;
}

int Error::SetOkay(){
    data_ = { 0, "", "", "" };
    return 0;
}

void Error::OutputLast(){
    std::cout << "Error code: " << data_.code << " thrown by class: " << data_.class_name <<
              " in method: " << data_.method_name << " with message : " << data_.message << std::endl;
};
