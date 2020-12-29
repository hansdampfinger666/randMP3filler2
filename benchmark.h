#ifndef BENCHMARK_H
#define BENCHMARK_H

#pragma once
#include <x_pch.h>
#include <format.h>

class Benchmark
{
public:
    static void StartBenchmark(const std::string &benchmark_name){
        data_.benchmark_name = benchmark_name;
        data_.benchmark_start = std::chrono::steady_clock::now();
    }

    static void EndBenchmark(bool output_file){
        data_.benchmark_end = std::chrono::steady_clock::now();
        data_.benchmark_runtime =
                std::chrono::duration_cast<std::chrono::nanoseconds>
                (data_.benchmark_end - data_.benchmark_start).count();
        OutputBenchmark();
        if(output_file)
            OutputFile();
    }

    template<typename T>
    static void StartClock(T *caller, const std::string &method_name){
        std::string class_name = typeid(caller).name();
        int clock_index = GetClockIndex(class_name, method_name);
        data_.start.at(clock_index) = std::chrono::steady_clock::now();
    }

    template<typename T>
    static void StopClock(T *caller, const std::string &method_name){
        std::string class_name = typeid(caller).name();
        int clock_index = GetClockIndex(class_name, method_name);
        data_.end.at(clock_index) = std::chrono::steady_clock::now();
        data_.total_runtimes.at(clock_index) +=
                std::chrono::duration_cast<std::chrono::nanoseconds>
                (data_.end.at(clock_index) -
                 data_.start.at(clock_index)).count();
        data_.executions.at(clock_index)++;
    }

    static void ResetBenchmark(){
        data_ = { {}, 0, 0, {}, {}, {}, {}, {}, {}, {}, {}, {} };
    }

private:
    struct Data{
        std::string benchmark_name;
        int clock_instances;
        float benchmark_runtime;
        std::chrono::steady_clock::time_point benchmark_start;
        std::chrono::steady_clock::time_point benchmark_end;
        std::vector<std::string> class_names;
        std::vector<std::string> method_names;
        std::vector<std::chrono::steady_clock::time_point> start;
        std::vector<std::chrono::steady_clock::time_point> end;
        std::vector<float> total_runtimes;
        std::vector<float> percentages_of_total;
        std::vector<int> executions;
    };
    inline static Data data_;

    static int GetClockIndex(const std::string &class_name,
                             const std::string &method_name){
        for(int i = 0; i < data_.clock_instances; i++)
            if(data_.class_names.at(i) == class_name and
                    data_.method_names.at(i) == method_name)
                return i;
        return AddClock(class_name, method_name);
    }

    static int AddClock(const std::string &class_name,
                        const std::string &method_name){
        data_.class_names.push_back(class_name);
        data_.method_names.push_back(method_name);
        data_.start.push_back({});
        data_.end.push_back({});
        data_.total_runtimes.push_back(0);
        data_.percentages_of_total.push_back(0);
        data_.executions.push_back(0);
        data_.clock_instances++;
        return data_.clock_instances - 1;
    }

    static void OutputBenchmark(){
        SetBenchmarkTotals();
        for(int i = 0; i < data_.clock_instances; i++)
            std::cout << "Class: " << data_.class_names.at(i) << " Method: " <<
                         data_.method_names.at(i) <<
                         " Executions: " << data_.executions.at(i) <<
                         " Total Runtime: " <<
                         Format::GetReadableNanoSec(
                             data_.total_runtimes.at(i)) <<
                         " Percentage of total: " <<
                         data_.percentages_of_total.at(i) << "%\n";
        std::cout << "---------------------------\n" <<
                     "TOTAL RUNTIME: " <<
                     Format::GetReadableNanoSec(data_.benchmark_runtime) <<
                     "\n";
    }

    static void SetBenchmarkTotals(){
        for(int i = 0; i < data_.clock_instances; i++)
            data_.percentages_of_total.at(i) =
                    data_.total_runtimes.at(i) / data_.benchmark_runtime * 100;
    }

    static void OutputFile(){
        std::string path = std::filesystem::current_path() /=
                "benchmark_" + data_.benchmark_name;
        std::ofstream ofs(path, std::ofstream::app);
        if(ofs.is_open())
            ofs << data_.benchmark_runtime << "\n";
    }
};

#endif // BENCHMARK_H
