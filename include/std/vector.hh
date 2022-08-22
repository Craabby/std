#pragma once

#include <std/cstddef.hh>
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
            SetCapacity(m_Capacity << 1);
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
        inline Vector() = default;
        inline Vector(Vector const &other)
        {
            SetCapacity(other.m_Capacity);
            for (size_t i = 0; i < other.m_Size; i++)
                m_Data[i] = (other.m_Data[i]);
        };
        inline Vector(Vector &&other)
        {
            m_Capacity = other.m_Capacity;
            m_Data = other.m_Data;
            m_Size = other.m_Size;
            other.m_Data = nullptr;
            other.m_Capacity = 0;
            other.m_Size = 0;
        };

        class Iterator
        {
            size_t m_Index;
            T *m_Data;

        public:
            inline Iterator(T *data, size_t index)
                : m_Data(data),
                  m_Index(index)
            {
            }

            inline bool operator<(Iterator other) const { return m_Index < other.m_Index; }
            inline bool operator>(Iterator other) const { return m_Index > other.m_Index; }
            inline bool operator!=(Iterator other) const { return m_Index != other.m_Index; }
            inline bool operator==(Iterator other) const { return m_Index == other.m_Index; }
            inline T &operator*() { return m_Data[m_Index]; }
            inline T const &operator*() const { return m_Data[m_Index]; }
            inline Iterator &operator++()
            {
                m_Index++;
                return *this;
            }

            inline Iterator operator++(int)
            {
                Iterator i{m_Data, m_Index};
                m_Index++;
                return i;
            }
        };

        inline Vector(size_t size, const T &value = T{})
        {
            m_Data = (T *)::operator new(size * sizeof(T));
            m_Capacity = size;
            m_Size = size;
            for (size_t i = 0; i < m_Size; i++)
                m_Data[i] = value;
        }
        inline ~Vector()
        {
            // cannot use delete[] because m_Data was not allocated using new[]
            for (size_t i = 0; i < m_Size; i++)
                m_Data[i].~T();
            ::operator delete(m_Data, m_Capacity * sizeof(T));
        }

        inline void Reserve(size_t size)
        {
#ifdef STD2_VECTOR_SAFE
            Assert(size > m_Capacity)
#endif
                SetCapacity(size);
        }

        inline Iterator begin()
        {
            return Iterator{m_Data, 0};
        }

        inline Iterator end()
        {
            return Iterator{m_Data, m_Size};
        }

        inline T const &Push(T const &value)
        {
            if (m_Size == m_Capacity)
                Grow();
            m_Data[m_Size] = value;
            m_Size++;

            return value;
        }

        inline T &Push(T &&value)
        {
            if (m_Size == m_Capacity)
                Grow();
            new (&m_Data[m_Size]) T(Move(value));
            m_Size++;

            return value;
        }

        template <typename... Arguments>
        inline T &Emplace(Arguments &&...arguments)
        {
            if (m_Size == m_Capacity)
                Grow();
            new (&m_Data[m_Size]) T(Forward<Arguments>(arguments)...);
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
