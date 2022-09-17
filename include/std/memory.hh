#pragma once

#include <cstddef>
#include <std/move.hh>
#ifdef STD2_SIMD
#include <x86intrin.h>
#endif

namespace std2
{
    template <typename T>
    class SmartPointer
    {
        T *m_Data;

    public:
        SmartPointer(SmartPointer const &) = delete;
        T &operator=(SmartPointer const &) = delete;
        SmartPointer(SmartPointer &&) = delete;
        T &operator=(SmartPointer &&) = delete;
        explicit SmartPointer(T *x)
        {
            m_Data = x;
        }
        ~SmartPointer()
        {
            delete m_Data;
        }

        T Get() { return m_Data; }
        T const &Get() const { return *Get(); }
        T &operator*() { return *Get(); }
        T const &operator*() const { return *Get(); }
        T *operator->() { return Get(); }
        T const *operator->() const { return Get(); }
    };

    template <typename T>
    class SmartPointer<T[]>
    {
        T *m_Data;

    public:
        SmartPointer(SmartPointer const &) = delete;
        T &operator=(SmartPointer const &) = delete;
        SmartPointer(SmartPointer &&) = delete;
        T &operator=(SmartPointer &&) = delete;

        explicit SmartPointer(T *x)
        {
            m_Data = x;
        }
        ~SmartPointer()
        {
            delete[] m_Data;
        }

        T Get() { return m_Data; }
        T const &Get() const { return *Get(); }
        T &operator*() { return *Get(); }
        T const &operator*() const { return *Get(); }
        T *operator->() { return Get(); }
        T const *operator->() const { return Get(); }
    };
}

namespace std2::memory
{
    inline void Move(void *destination, void const *source, size_t size)
    {
        size_t i = 0;

#ifdef STD2_SIMD
        for (; i + 32 <= size; i += 32)
        {
            __m256i source_vec = _mm256_loadu_si256((__m256i_u *)(((uint8_t *)source)[i]));
            _mm256_storeu_si256((__m256i_u *)(((uint8_t *)destination)[i]), source_vec);
        }

        for (; i + 16 <= size; i += 16)
        {
            __m128i source_vec = _mm_loadu_si128((__m128i_u *)(((uint8_t *)source)[i]));
            _mm_storeu_si128((__m128i_u *)(((uint8_t *)destination)[i]), source_vec);
        }
#endif

        for (; i < size; i++)
            ((uint8_t *)destination)[i] = ((uint8_t *)source)[i];
    }
}
