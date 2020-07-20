#ifndef UTILS_HPP
#define UTILS_HPP

#include "Vector2.hpp"

namespace Utils {

    template <typename T>
    bool isInside(Vector2<T> point, Vector2<T> min, Vector2<T> max)
    {
        return
            point.x >= min.x &&
            point.x <= max.x &&
            point.y >= min.y &&
            point.y <= max.y;
    }
};

#endif // UTILS_HPP
