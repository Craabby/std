#pragma once

#include <cstdint>
#include <std/type_traits.hh>

namespace std2
{
    template <typename T, T t_Seed, T t_Add, T t_Mul, T t_Mod>
    class _LinearCongruentialGenerator
    {
        T m_Seed;

    public:
        using type = T;

        _LinearCongruentialGenerator()
            : m_Seed(t_Seed)
        {
        }

        _LinearCongruentialGenerator(T seed)
            : m_Seed(seed)
        {
        }

        void Seed(T v) { m_Seed = v; }
        constexpr T Seed() const { return t_Seed; }
        constexpr T Add() const { return t_Add; }
        constexpr T Mul() const { return t_Mul; }
        constexpr T Mod() const { return t_Mod; }
        constexpr T Next()
        {
            m_Seed = (m_Seed * t_Mul + t_Add) % t_Mod;
            return m_Seed;
        }

        constexpr T operator()() { return Next(); }
        constexpr operator T() { return Next(); }
    };

    template <typename T>
    struct LinearCongruentialGenerator;
    template <>
    struct LinearCongruentialGenerator<uint64_t> : _LinearCongruentialGenerator<uint64_t, 4096437239ull, 3152234867ull, 4039735907ull, 18446744073709551557ull>
    {
    };
    template <>
    struct LinearCongruentialGenerator<uint32_t> : _LinearCongruentialGenerator<uint32_t, 245683, 578719, 65533, 2147483647>
    {
    };
    template <>
    struct LinearCongruentialGenerator<bool> : _LinearCongruentialGenerator<uint8_t, 3, 7, 11, 251>
    {
        using Base = _LinearCongruentialGenerator<uint8_t, 3, 7, 11, 251>;
        constexpr bool Next() { return Base::Next() & 1; }
        constexpr bool operator()() { return Base::operator()() & 1; }
        constexpr operator bool() { return Base::operator uint8_t() & 1; }
    };
    template <>
    struct LinearCongruentialGenerator<float> : LinearCongruentialGenerator<uint32_t>
    {
        using Base = LinearCongruentialGenerator<uint32_t>;
        constexpr bool Next() { return ((float)Base::Next()) / 2147483647.0f; }
        constexpr bool operator()() { return ((float)Base::operator()()) / 2147483647.0f; }
        constexpr operator float() { return ((float)Base::operator uint32_t()) / 2147483647.0f; }
    };

    static LinearCongruentialGenerator<uint64_t> randomUint64Helper;
    static LinearCongruentialGenerator<uint32_t> randomUint32Helper;
    static LinearCongruentialGenerator<float> randomFloatHelper;
    static LinearCongruentialGenerator<bool> randomBoolHelper;

    template <typename T>
    void Seed(T x);
    template <typename T>
    T Random();
    template <typename T>
    T Random(T max) { return Random<T>() % max; }
    template <typename T>
    T Random(T min, T max) { return Random<T>(max - min) + min; }

    template <>
    uint64_t Random<uint64_t>() { return randomUint64Helper; }
    template <>
    uint32_t Random<uint32_t>() { return randomUint32Helper; }
    template <>
    float Random<float>() { return randomFloatHelper; }
    template <>
    bool Random<bool>() { return randomBoolHelper; }

    template <>
    float Random<float>(float max) { return Random<float>() * max; }

    template <>
    void Seed<uint64_t>(uint64_t x) { randomUint64Helper.Seed(x); }
    template <>
    void Seed<uint32_t>(uint32_t x) { randomUint32Helper.Seed(x); }
    template <>
    void Seed<float>(float x) { randomFloatHelper.Seed(x); }
    template <>
    void Seed<bool>(bool x) { randomBoolHelper.Seed(x); }
}
