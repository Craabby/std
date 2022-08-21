#pragma once

#include <std/cstddef.hh>

namespace std2::memory
{   
    template <typename T>
    void Move(T *destination, T const *source, size_t size)
    {
        for (uintptr_t i = 0; i < size; i++)
            ((uint8_t *)destination)[i] = ((uint8_t *)source)[i];
    }
}
