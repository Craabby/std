#pragma once

#include <std/type_traits.hh>

namespace std2
{
    // forward an lvalue
    template <typename T>
    inline T &&Forward(remove_reference_t<T> &x) noexcept
    {
        return static_cast<T &&>(x);
    }

    // forward an rvalue
    template <typename T>
    inline T &&Forward(remove_reference_t<T> &&x) noexcept
    {
        static_assert(!IsLvalueReference<T>::value);
        return static_cast<T &&>(x);
    }

    template <typename T>
    inline remove_reference_t<T> &&Move(T &&x) noexcept
    {
        return static_cast<remove_reference_t<T> &&>(x);
    }
}
