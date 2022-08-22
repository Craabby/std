#pragma once

#include <std/cstdint.hh>
#include <std/vector.hh>

namespace std2
{
    template <typename CharType>
    class BasicString : public Vector<CharType>
    {
    public:
        using Base = Vector<CharType>;
        using Type = BasicString<CharType>;

        inline BasicString() = default;
        inline BasicString(CharType const *bytes)
        {
            size_t i = 0;
            while (true)
            {
                if (bytes[i] == 0)
                    break;
                this->Push(bytes[i]);
                i++;
            }
        }
        inline BasicString(CharType const *bytes, size_t size)
        {
            for (size_t i = 0; i < size; i++)
                Push(bytes[i]);
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
        inline void Append(Type &other)
        {
            this->Reserve(this->Size() + other.Size());
            for (CharType character : other.Bytes())
                Append(character);
        }

        inline Type Plus(Type const &other)
        {
            Type newString;
            newString.Reserve(this->Size() + other.Size());
            for (CharType character : *this)
                newString.Push(character);
            for (size_t i = 0; i < other.Size(); i++)
                newString.Push(other[i]);

            return newString;
        }
        inline Type Plus(Type &other)
        {

            Type newString;
            newString.Reserve(this->Size() + other.Size());
            for (CharType character : *this)
                newString.Push(character);
            for (CharType character : other)
                newString.Push(character);

            return newString;
        }

        inline Base const &Bytes() const { return *static_cast<Base const *>(this); }
        inline Base &Bytes() { return *static_cast<Base *>(this); }
        inline void operator+=(CharType character) { Append(character); }
        inline void operator+=(Type const &other) { Append(other); }
        inline void operator+=(Type &other) { Append(other); }
        inline Type operator+(Type const &other) { return Plus(other); }
        inline Type operator+(Type &other) { Plus(other); }
        inline CharType &operator[](size_t index) { return this->At(index); }
        inline CharType const &operator[](size_t index) const { return this->At(index); }
        inline operator Base &() { return Bytes(); }
        inline operator Base const &() const { return Bytes(); }
        inline operator CharType const *() const { return Bytes().Data(); }
        inline operator CharType *() { return Bytes().Data(); }
    };

    using String = BasicString<char>;
    using WideString = BasicString<wchar_t>;
}
