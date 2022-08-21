#pragma once

namespace std2
{
    template <typename T, T x>
    struct IntegralConstant
    {
        using type = IntegralConstant<T, x>;
        static constexpr T value = x;
        constexpr operator T() const noexcept { return value; }
        constexpr T operator()() const noexcept { return value; }
    };

    template <bool x>
    using BoolConstant = IntegralConstant<bool, x>;
    using TrueType = BoolConstant<true>;
    using FalseType = BoolConstant<false>;

    template <typename, typename>
    struct IsSame : FalseType
    {
    };

    template <typename T>
    struct IsSame<T, T> : TrueType
    {
    };

    template <bool condition, typename IfTrue, typename IfFalse>
    struct Conditional
    {
        using type = IfTrue;
    };

    template <typename IfTrue, typename IfFalse>
    struct Conditional<false, IfTrue, IfFalse>
    {
        using type = IfFalse;
    };

    template <typename...>
    struct Or;

    template <>
    struct Or<> : FalseType
    {
    };

    template <typename A>
    struct Or<A> : A
    {
    };

    template <typename A, typename B>
    struct Or<A, B> : Conditional<A::value, A, B>::type
    {
    };

    template <typename A, typename B, typename C, typename... D>
    struct Or<A, B, C, D...> : Conditional<A::value, A, Or<B, C, D...>>::type
    {
    };

    template <typename>
    struct IsLvalueReference : FalseType
    {
    };

    template <typename T>
    struct IsLvalueReference<T &> : TrueType
    {
    };

    template <typename>
    struct IsRvalueReference : FalseType
    {
    };

    template <typename T>
    struct IsRvalueReference<T &&> : TrueType
    {
    };

    template <typename T>
    struct IsReference : Or<IsLvalueReference<T>, IsRvalueReference<T>>::type
    {
    };

    // RemoveReference
    template <typename T>
    struct RemoveReference
    {
        using type = T;
    };
    template <typename T>
    struct RemoveReference<T &>
    {
        using type = T;
    };
    template <typename T>
    struct RemoveReference<T &&>
    {
        using type = T;
    };

    // RemoveConst
    template <typename T>
    struct RemoveConst
    {
        using type = T;
    };

    template <typename T>
    struct RemoveConst<T const>
    {
        using type = T;
    };

    // RemoveVolatile
    template <typename T>
    struct RemoveVolatile
    {
        using type = T;
    };

    template <typename T>
    struct RemoveVolatile<T volatile>
    {
        using type = T;
    };

    // RemoveCV
    template <typename T>
    struct RemoveCV
    {
        using type = typename RemoveConst<typename RemoveVolatile<T>::type>::type;
    };

    template <typename T>
    inline constexpr bool is_reference_v = IsReference<T>::value;
    template <typename T>
    inline constexpr bool is_rvalue_reference_v = IsRvalueReference<T>::value;
    template <typename T, typename U>
    inline constexpr bool is_same_v = IsSame<T, U>::value;
    template <typename T>
    inline constexpr bool is_lvalue_reference_v = IsLvalueReference<T>::value;
    template <typename T>
    using remove_reference_t = typename RemoveReference<T>::type;
    template <typename T>
    using remove_const_t = typename RemoveConst<T>::type;
    template <typename T>
    using remove_volatile_t = typename RemoveVolatile<T>::type;
    template <typename T>
    using remove_cv_t = typename RemoveCV<T>::type;
}
