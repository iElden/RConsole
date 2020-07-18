#ifndef VECTOR2_HPP
#define VECTOR2_HPP

template <typename T>
struct Vector2 {
public:
    T x;
    T y;

    bool isInside(Vector2<T> min, Vector2<T> max) {
        return
            this->x >= min.x &&
            this->x <= max.x &&
            this->y >= min.y &&
            this->y <= max.y;
    }
};

#define VINT Vector2<int>
#define VNUL {-1, -1}

#endif // VECTOR2_HPP
