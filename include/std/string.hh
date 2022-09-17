#pragma once

#include <cstdint>
#include <std/vector.hh>

namespace std2
{
    template <typename CharType>
    class BasicString : public Vector<CharType>
    {
    public:
        using Char = CharType;
        using Base = Vector<CharType>;
        using Type = BasicString<CharType>;

        inline BasicString() = default;
        inline BasicString(CharType const *bytes)
        {
            for (size_t i = 0; bytes[i] != 0; i++)
            {
                if (bytes[i] == 0)
                    break;
                this->Push(bytes[i]);
            }
        }
        inline BasicString(CharType const *bytes, size_t size)
        {
            for (size_t i = 0; i < size; i++)
                this->Push(bytes[i]);
        }
        inline BasicString(Base const &bytes) : Base(bytes)
        {
        }
        inline BasicString(Base &&bytes) : Base(bytes)
        {
        }
        inline BasicString(size_t initialCapacity) : Base(initialCapacity)
        {
        }
        inline BasicString(BasicString const &other) : Base(other.Bytes())
        {
        }
        inline BasicString(BasicString &&other) : Base(other.Bytes())
        {
        }

        inline void Append(CharType character) { this->Push(character); }
        inline void Append(Type const &other)
        {
            this->Reserve(this->Size() + other.Size());
            for (size_t i = 0; i < other.Size(); i++)
                Append(other[i]);
        }

        inline Type Plus(Type const &other) const
        {
            Type newString;
            newString.Reserve(this->Size() + other.Size());
            for (CharType character : *this)
                newString.Push(character);
            for (size_t i = 0; i < other.Size(); i++)
                newString.Push(other[i]);

            return newString;
        }

        inline CharType *AsCString() { return Bytes().Data(); }
        inline CharType const *AsCString() const { return Bytes().Data(); }
        inline Base const &Bytes() const { return *static_cast<Base const *>(this); }
        inline Base &Bytes() { return *static_cast<Base *>(this); }
        inline void operator+=(CharType character) { Append(character); }
        inline void operator+=(Type const &other) { Append(other); }
        inline Type operator+(Type const &other) const { return Plus(other); }
        inline operator Base &() { return Bytes(); }
        inline operator Base const &() const { return Bytes(); }
    };

    using String = BasicString<char>;
    using WideString = BasicString<wchar_t>;
}
