#include <iostream> // TODO: remove

#define Assert(expression)                                                                                     \
    if ((bool)(expression))                                                                                    \
        void(0);                                                                                               \
    else                                                                                                       \
    {                                                                                                          \
        std::cerr << "assertion failed: `" << #expression << "` " << __FILE__ << ":" << __LINE__ << std::endl; \
        exit(1);                                                                                               \
    }
