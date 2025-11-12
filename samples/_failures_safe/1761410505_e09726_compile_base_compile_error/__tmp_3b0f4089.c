//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

struct Vector3D {
    double x;
    double y;
    double z;
};

struct Matrix3x3 {
    double elements[3][3];
};

struct LinearSystem {
    struct Matrix3x3 coefficients;
    struct Vector3D constants;
    struct Vector3D solution;
};

struct GeometryData {
    struct Vector3D points[4];
    double distances[6];
    double areas[4];
    double volume;
};

int validate_double(double value) {
    return !isnan(value) && !isinf(value);
}

int validate_vector3d(const struct Vector3D *v) {
    return validate_double(v->x) && validate_double(v->y) && validate_double(v->z);
}

double vector3d_dot(const struct Vector3D *a, const struct Vector3D *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

struct Vector3D vector3d_cross(const struct Vector3D *a, const struct Vector3D *b) {
    struct Vector3D result;
    result.x = a->y * b->z - a->z * b->y;
    result.y = a->z * b->x - a->x * b->z;
    result.z = a->x * b->y - a->y * b->x;
    return result;
}

double vector3d_length(const struct Vector3D *v) {
    double len_sq = vector3d_dot(v, v);
    if (len_sq < 0.0) return 0.0;
    return sqrt(len_sq);
}

struct Vector3D vector3d_subtract(const struct Vector3D *a, const struct Vector3D *b) {
    struct Vector3D result;
    result.x = a->x - b->x;
    result.y = a->y - b->y;
    result.z = a->z - b->z;
    return result;
}

double vector3d_triple_product(const struct Vector3D *a, const struct Vector3D *b, const struct Vector3D *c) {
    struct Vector3D cross_result = vector3d_cross(b, c);
    return vector3d_dot(a, &cross_result);
}

int solve_linear_system(struct LinearSystem *system) {
    struct Matrix3x3 *m = &system->coefficients;
    struct Vector3D *c = &system->constants;
    
    double det = m->elements[0][0] * (m->elements[1][1] * m->elements[2][2] - m->elements[1][2] * m->elements[2][1])
               - m->elements[0][1] * (m->elements[1][0] * m->elements[2][2] - m->elements[1][2] * m->elements[2][0])
               + m->elements[0][2] * (m->elements[1][0] * m->elements[2][1] - m->elements[1][1] * m->elements[2][0]);
    
    if (fabs(det) < 1e-12) {
        return 0;
    }
    
    double inv_det = 1.0 / det;
    
    system->solution.x = inv_det * (
        c->x * (m->elements[1][1] * m->elements[2][2] - m->elements[1][2] * m->elements[2][1]) -
        c->y * (m->elements[0][1] * m->elements[2][2] - m->elements[0][2] * m->elements[2][1]) +
        c->z * (m->elements[0][1] * m->elements[1][2] - m->elements[0][2] * m->elements[1][1])
    );
    
    system->solution.y = inv_det * (
        m->elements[0][0] * (c->y * m->elements[2][2] - c->z * m->elements[2][1]) -
        m->elements[1][0] * (c->x * m->elements[2][2] - c->z * m->elements[2][0]) +
        m->elements[2][0] * (c->x * m->elements[1][2] - c->y * m->elements[1][0])
    );
    
    system->solution.z = inv_det * (
        m->elements[0][0] * (m->elements[1][1] * c->z - m->elements[1][2] * c->y) -
        m->elements[0][1] * (m->elements[1][0] * c->z - m->elements[1][2] * c->x) +
        m->elements[0][2] * (m->elements[1][0] * c->y - m->elements[1][1] * c->x)
    );
    
    return validate_vector3d(&system->solution);
}

void compute_geometry(struct GeometryData *data) {
    for (int i = 0; i < 4; i++) {
        if (!validate_vector3d(&data->points[i])) {
            return;
        }
    }
    
    int idx =