#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <iostream>

class Error
{
public:
    typedef struct{
        int code;
        std::string class_name;
        std::string method_name;
        std::string message;
    } Data;

    template<typename T>
    static int LogOne(const int code, const T *caller, const std::string &method_name, const std::string &message){
        ResetData();
        if(verbose_logging_active_)
            data_ = { code, typeid(caller).name(), method_name, message };
        else
            data_.code = code;
        return code;
    }
    static int SetOkay(){ data_ = { 0, "", "", "" }; return 0; };
    static void SetVerboseLogging(const bool flag){ verbose_logging_active_ = flag; };
    static void CoutLatest(){ std::cout << "Error code: " << data_.code << " thrown by class: " << data_.class_name <<
                                           " in method: " << data_.method_name << " with message : " << data_.message << std::endl; };

private:
    inline static Data data_ = { 0, "", "", "" };
    inline static bool verbose_logging_active_ = false;

    static void ResetData(){ data_ = { 0, "", "", "" }; };
};

#endif // ERROR_H
