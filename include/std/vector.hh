#pragma once

#include <cstddef>
#include <std/memory.hh>
#include <std/cassert.hh>
#include <std/move.hh>

namespace std2
{
    template <typename T>
    class Vector
    {
        static_assert(is_same_v<T, remove_cv_t<T>>, "T cannot be const or volatile");
        size_t m_Size = 0;
        size_t m_Capacity = 1;
        T *m_Data = (T *)::operator new(sizeof(T));

        inline void Grow()
        {
            SetCapacity(m_Capacity * 3); // will be optimized to (m_Capacity << 1) + m_Capacity
        }

        inline void SetCapacity(size_t size)
        {
            T *newData = (T *)::operator new(size * sizeof(T));
            for (size_t i = 0; i < m_Size; i++)
                newData[i] = Move(m_Data[i]);
            ::operator delete(m_Data, m_Capacity * sizeof(T));
            m_Data = newData;
            m_Capacity = size;
        }

    public:
        using iterator = T *;
        using const_iterator = T const *;

        inline Vector() = default;
        inline Vector(Vector const &other)
        {
            *this = other;
        }
        inline Vector(Vector &&other)
        {
            *this = Move(other);
        }
        // clone if it is number, otherwise const & it
        inline Vector(size_t size, typename Conditional<is_number_v<T>, T, T const &>::type value = T())
        {
            m_Data = (T *)::operator new(size * sizeof(T));
            m_Capacity = size;
            m_Size = size;
            for (size_t i = 0; i < m_Size; i++)
                m_Data[i] = value;
        }

        inline ~Vector()
        {
            // should not use delete[] because m_Data was not allocated using new[]
            for (size_t i = 0; i < m_Size; i++)
                m_Data[i].~T();
            ::operator delete(m_Data, m_Capacity * sizeof(T));
        }
        inline Vector<T> &operator=(Vector<T> &&other)
        {
#ifdef STD2_VECTOR_SAFE
            Assert(this != &other);
#endif
            m_Capacity = other.m_Capacity;
            m_Data = other.m_Data;
            m_Size = other.m_Size;
            other.m_Data = nullptr;
            other.m_Capacity = 0;
            other.m_Size = 0;
            return *this;
        }

        inline Vector<T> &operator=(Vector<T> const &other)
        {
#ifdef STD2_VECTOR_SAFE
            Assert(this != &other);
#endif
            SetCapacity(other.m_Capacity);
            for (size_t i = 0; i < other.m_Size; i++)
                m_Data[i] = (other.m_Data[i]);
            return *this;
        }

        inline void Clear()
        {
            for (size_t i = 0; i < m_Size; i++)
                m_Data[i].~T();
            m_Size = 0;
        }

        inline void Reserve(size_t size)
        {
#ifdef STD2_VECTOR_SAFE
            Assert(size > m_Capacity)
#endif
                SetCapacity(size);
        }

        inline void Resize(size_t size, typename Conditional<is_number_v<T>, T, T const &>::type v = T())
        {
            Reserve(size);
            m_Size = size;
            for (size_t i = 0; i < size; i++)
                m_Data[i] = v;
        }

        inline iterator begin() { return m_Data; }
        inline iterator end() { return m_Data + m_Size; }
        inline const_iterator cbegin() const { return m_Data; }
        inline const_iterator cend() const { return m_Data + m_Size; }

        inline T &Front()
        {
#ifdef STD2_VECTOR_SAFE
            Assert(m_Size);
#endif
            return *m_Data;
        }
        inline T const &Front() const
        {
#ifdef STD2_VECTOR_SAFE
            Assert(m_Size);
#endif
            return *m_Data;
        }
        inline T &Back() { return m_Data[m_Size - 1]; }
        inline T const &Back() const { return m_Data[m_Size - 1]; }

        inline T &Push(typename Conditional<is_number_v<T>, T, T const &>::type value)
        {
            if (m_Size == m_Capacity)
                Grow();
            m_Data[m_Size] = value;
            m_Size++;

            return Back();
        }

        inline T &Push(T &&x)
        {
            // call move constructor
            Emplace(Move(x));
        }

        template <typename... Arguments>
        inline T &Emplace(Arguments const &...arguments)
        {
            if (m_Size == m_Capacity)
                Grow();
            new (&m_Data[m_Size]) T(arguments...);
            m_Size++;
            return m_Data[m_Size - 1];
        }

        inline void ShrinkToOptimalCapacity()
        {
            SetCapacity(m_Size);
        }

        inline T &At(size_t i)
        {
#ifdef STD2_VECTOR_SAFE
            Assert(i < m_Size);
#endif
            return m_Data[i];
        }

        inline T const &At(size_t i) const
        {
#ifdef STD2_VECTOR_SAFE
            Assert(i < m_Size);
#endif
            return m_Data[i];
        }

        template <bool fast = false>
        inline void Erase(size_t index)
        {
#ifdef STD2_VECTOR_SAFE
            Assert(index < m_Size);
#endif
            m_Size--;
            m_Data[index].~T();
            if constexpr (fast)
                new (m_Data + index - 1) T(Move(m_Data[m_Size]));
            else
                for (size_t i = index; i < m_Size; i++)
                    m_Data[i - 1] = Move(m_Data[i]);
        }

        inline void Pop()
        {
            m_Size--;
            m_Data[m_Size].~T();
        }

        inline T &operator[](size_t i)
        {
            return At(i);
        }

        inline T const &operator[](size_t i) const
        {
            return At(i);
        }

        inline size_t Capacity() const
        {
            return m_Capacity;
        }

        inline size_t Size() const
        {
            return m_Size;
        }

        inline T *Data()
        {
            return m_Data;
        }

        inline T const *Data() const
        {
            return m_Data;
        }
    };
}
