#pragma once

#include <std/cstddef.hh>
#ifdef STD2_SIMD
    #include <x86intrin.h>
#endif

namespace std2::memory
{   
    void Move(void *destination, void const *source, size_t size)
    {
        size_t i = 0;

#ifdef STD2_SIMD
        for (; i + 32 <= size; i += 32)
        {
            __m256i source_vec = _mm256_loadu_si256((__m256i_u*) (((uint8_t*) source)[i]));
            _mm256_storeu_si256((__m256i_u*) (((uint8_t*) destination)[i]), source_vec);
        }

        for (; i + 16 <= size; i += 16)
        {
            __m128i source_vec = _mm_loadu_si128((__m128i_u*) (((uint8_t*) source)[i]));
            _mm_storeu_si128((__m128i_u*) (((uint8_t*) destination)[i]), source_vec);
        }
#endif

        for (; i < size; i++)
        {
            ((uint8_t*) destination)[i] = ((uint8_t*) source)[i];
        }
    }
}
