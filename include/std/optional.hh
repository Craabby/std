#pragma once

#include <std/cassert.hh>
#include <std/move.hh>
#include <std/type_traits.hh>

// #include <optional>
// std::optional<int> a;

namespace std2
{
    template <typename T>
    class Optional
    {
        static_assert(!is_reference_v<T>, "T cannot be a reference");
        static_assert(is_same_v<T, remove_cv_t<T>>, "T cannot be const or volatile");
        bool m_HasValue = false;
        union
        {
            T m_Value;
        };

    public:
        inline Optional()
        {
            m_HasValue = false;
        }
        inline Optional(const Optional &other)
        {
            m_HasValue = other.m_HasValue;
            if (other.m_HasValue)
                m_Value = other.m_Value;
        }
        inline Optional(Optional &&other)
        {
            if (other.m_HasValue)
                m_Value = other.m_Value;
            other.m_HasValue = false;
        }
        inline ~Optional()
        {
            if (m_HasValue)
                m_Value.~T();
        }
        inline Optional(const T &value)
            : m_HasValue(true),
              m_Value(value)
        {
        }

        inline Optional(T &&value)
            : m_HasValue(true),
              m_Value(value)
        {
        }

        inline bool HasValue() const
        {
            return m_HasValue;
        }

        inline T &Set(T &&x)
        {
#ifdef STD2_OPTIONAL_SAFE
            Assert(!m_HasValue);
#endif
            new (&m_Value) T(Move(x));
            m_HasValue = true;
            return m_Value;
        }

        template <typename... Arguments>
        inline T &Emplace(Arguments &&...arguments)
        {
#ifdef STD2_OPTIONAL_SAFE
            Assert(!m_HasValue);
#endif
            m_HasValue = true;
            new (&m_Value) T(Forward<Arguments>(arguments)...);
            return m_Value;
        }

        inline const T &Value() const
        {
#ifdef STD2_OPTIONAL_SAFE
            Assert(m_HasValue);
#endif

            return m_Value;
        }

        inline T &Value()
        {
#ifdef STD2_OPTIONAL_SAFE
            Assert(m_HasValue);
#endif

            return m_Value;
        }

        inline void Delete()
        {
#ifdef STD2_OPTIONAL_SAFE
            Assert(m_HasValue);
#endif
            m_HasValue = false;
            m_Value.~T();
        }

        inline T &operator=(const T &x) { return Set(x); }
        inline T &operator=(T &&x) { return Set(x); }
        inline T &operator*() { return Value(); }
        inline const T &operator*() const { return Value(); }
        inline operator T &() { return Value(); }
        inline operator const T &() const { return Value(); }
    };
}
