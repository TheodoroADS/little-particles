#ifndef VEC_UTILS_H
#define VEC_UTILS_H

#include "include/raylib.h"


inline float vec_norm(Vector2 v) {
    return sqrtf(v.x*v.x + v.y*v.y);
}


Vector2 vec_mul(Vector2 vector, float scalar) {

    vector.x *= scalar;
    vector.y *= scalar;
    return vector;

}

Vector2 vec_diff(Vector2 vector1, Vector2 vector2) {
    vector1.x -= vector2.x;
    vector1.y -= vector2.y;
    return vector1;
}

Vector2 vec_add_scalar(Vector2 v, float scalar) {
    v.x += scalar;
    v.y += scalar;
    return v;
}

Vector2 vec_add(Vector2 v1, Vector2 v2) {
    v1.x += v2.x;
    v1.y += v2.y;
    return v1;
}

float dot_product(Vector2 v1, Vector2 v2) {
    return v1.x*v2.x + v2.y*v2.y;
}

#endif // VEC_UTILS_H