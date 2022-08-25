#pragma once

namespace std2
{
    template <typename T, typename Y>
    T Find(T begin, T end, const Y &value)
    {
        for (T i = begin; i < end; i++)
            if (*i == value)
                return i;
        return end;
    }
}
