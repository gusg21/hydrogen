#pragma once

// Maybe right!
#define MATH_PI            3.14159265358979323846f  // from xavier
#define MATH_TAU           6.28318530717958647692f  // from somewhere!!
#define MATH_EPSILON_FLOAT 1.192092896e-07F         // from climits

#define MATH_MIN(a, b) (a < b ? a : b)
#define MATH_MAX(a, b) (a > b ? a : b)
#define MATH_CLAMP(x, min, max) (MATH_MIN(MATH_MAX((x), (min)), (max)))