#pragma once

#include <cmath>
#include <std/cassert.hh>

namespace std2
{
    template <typename T>
    class Vector2
    {
    public:
        T x;
        T y;

        Vector2() = default;
        Vector2(T x, T y)
            : x(x),
              y(y)
        {
        }
        ~Vector2()
        {
        }
        Vector2(Vector2<T> const &other)
        {
            x = other.x;
            y = other.y;
            AssertValid();
        }
        Vector2(Vector2<T> &&other)
        {
            x = other.x;
            y = other.y;
            AssertValid();
        }
        Vector2<T> &operator=(Vector2<T> const &other)
        {
            x = other.x;
            y = other.y;

            AssertValid();
            return *this;
        }
        Vector2<T> &operator=(Vector2<T> &&other)
        {
            x = other.x;
            y = other.y;

            AssertValid();
            return *this;
        }
        void AssertValid()
        {
#ifdef STD2_VECTOR2_SAFE
            Assert(x == x);
            Assert(y == y);
#endif
        }
        Vector2<T> Clone()
        {
            AssertValid();
            return {x, y};
        }

        Vector2<T> &Set(Vector2<T> other)
        {
            x = other.x;
            y = other.y;

            AssertValid();
            return *this;
        }

        Vector2<T> &Set(T x, T y)
        {
            this->x = x;
            this->y = y;

            return *this;
            AssertValid();
        }

        Vector2<T> &Add(Vector2<T> other)
        {
            x += other.x;
            y += other.y;

            AssertValid();
            return *this;
        }

        Vector2<T> &Subtract(Vector2<T> other)
        {
            x -= other.x;
            y -= other.y;

            AssertValid();
            return *this;
        }

        Vector2<T> &Multiply(T x)
        {
            this->x *= x;
            y *= x;

            AssertValid();
            return *this;
        }

        Vector2<T> &Multiply(Vector2<T> other)
        {
            x *= other.x;
            y *= other.y;

            AssertValid();
            return *this;
        }

        T Magnitude()
        {
            AssertValid();
            return std::sqrt(x * x + y * y);
        }

        Vector2<T> UnitVector()
        {
            if (x == 0 && y == 0)
                return {1, 0};
            T magnitude = Magnitude();

            AssertValid();
            return {x / magnitude, y / magnitude};
        }

        Vector2<T> &Magnitude(T r)
        {
            Vector2<T> unitVector = UnitVector();

            x = r * unitVector.x;
            y = r * unitVector.y;

            AssertValid();
            return *this;
        }

        T Theta()
        {
            AssertValid();
            return std::atan2(x, y);
        }

        Vector2<T> &operator+=(Vector2<T> other) { return Add(other); }
        Vector2<T> &operator-=(Vector2<T> other) { return Subtract(other); }
        Vector2<T> &operator*=(Vector2<T> other) { return Multiply(other); }
        Vector2<T> &operator*=(T x) { return Multiply(x); }
        Vector2<T> operator+(Vector2<T> other) { return Clone().Add(other); }
        Vector2<T> operator-(Vector2<T> other) { return Clone().Subtract(other); }
        Vector2<T> operator*(Vector2<T> other) { return Clone().Multiply(other); }
        Vector2<T> operator*(T x) { return Clone().Multiply(x); }
    };

    using Vector2f = Vector2<float>;
}
