#ifndef FORMAT_H
#define FORMAT_H

#pragma once
#include <x_pch.h>

class Format
{
public:
    static std::string GetReadableBytes(const float bytes);
    static std::string GetReadableNanoSec(const float nanosec);

private:
    static constexpr float b_kb_ = 0.001f;
    static constexpr float b_mb_ = 0.000001f;
    static constexpr float b_gb_ = 0.000000001f;
    static constexpr float b_tb_ = 0.000000000001f;
    static constexpr char b_[] = " B";
    static constexpr char kb_[] = " KB";
    static constexpr char mb_[] = " MB";
    static constexpr char gb_[] = " GB";
    static constexpr char tb_[] = " TB";
    static constexpr float ns_us_ = 0.001f;
    static constexpr float ns_ms_ = 0.000001f;
    static constexpr float ns_s_ = 0.000000001f;
    static constexpr char ns_[] = " ns";
    static constexpr char us_[] = " us";
    static constexpr char ms_[] = " ms";
    static constexpr char s_[] = " s";
};

#endif // FORMAT_H
