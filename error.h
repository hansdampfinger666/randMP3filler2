#ifndef ERROR_H
#define ERROR_H

#pragma once
#include <x_pch.h>

class Error
{
public:
    typedef struct{
        int code;
        std::string class_name;
        std::string method_name;
        std::string message;
    } Data;

    static int Log(const int code, const std::string &caller, const std::string &method, const std::string &message);
    static int SetOkay();
    static void SetVerboseLogging(const bool flag){ verbose_logging_active_ = flag; };
    static void OutputLast();

private:
    inline static Data data_ = { 0, "", "", "" };
    inline static bool verbose_logging_active_ = false;

    static void Reset();
};

#endif // ERROR_H
