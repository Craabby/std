#include <iostream>

#define STD2_VECTOR_SAFE
#define STD2_OPTIONAL_SAFE

#include <std/optional.hh>
#include <std/vector.hh>

void *operator new(size_t a)
{
    std::cout << "allocated " << a << " bytes" << std::endl;
    return malloc(a);
}

void operator delete(void *x, size_t a) noexcept
{
    std::cout << "deleted " << a << " bytes" << std::endl;
    free(x);
}

class Listener
{
public:
    Listener()
    {
        std::cout << "construct" << std::endl;
    }
    ~Listener()
    {
        std::cout << "destroy" << std::endl;
    }
    Listener(const Listener &)
    {
        std::cout << "copy" << std::endl;
    }
    Listener(Listener &&)
    {
        std::cout << "move" << std::endl;
    }

    Listener &operator=(const Listener &)
    {
        std::cout << "copy assign" << std::endl;
        return *this;
    }
    Listener &operator=(Listener &&)
    {
        std::cout << "move assign" << std::endl;
        return *this;
    }
};

int main()
{
    std2::Optional<Listener> test;
    test.Set({});
}
