#include "format.h"

std::string Format::GetReadableBytes(const float bytes)
{
    float size = 0.0f;
    std::string suffix;

    if(bytes > 999999999999.0f)
    {
        size = bytes * b_tb_;
        suffix = tb_;
    }
    else if(bytes > 999999999.0f)
    {
        size = bytes * b_gb_;
        suffix = gb_;
    }
    else if(bytes > 999999.0f)
    {
        size = bytes * b_mb_;
        suffix = mb_;
    }
    else if(bytes > 999.f)
    {
        size = bytes * b_kb_;
        suffix = kb_;
    }
    else
    {
        size = bytes;
        suffix = b_;
    };

    std::ostringstream trunc;
    trunc.precision(2);
    trunc << std::fixed << size;
    return trunc.str() + suffix;
}


std::string Format::GetReadableNanoSec(const float nanosec)
{
    float time = 0.0f;
    std::string suffix;

    if(nanosec > 999999999.0f)
    {
        time = nanosec * ns_s_;
        suffix = s_;
    }
    else if(nanosec > 999999.0f)
    {
        time = nanosec * ns_ms_;
        suffix = ms_;
    }
    else if(nanosec > 999.0f)
    {
        time = nanosec * ns_us_;
        suffix = us_;
    }
    else
    {
        time = nanosec;
        suffix = ns_;
    }
    std::ostringstream trunc;
    trunc.precision(2);
    trunc << std::fixed << time;
    return trunc.str() + suffix;
}
