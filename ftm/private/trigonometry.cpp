#include "trigonometry.h"

static constexpr float PI = {3.14159265358979323846f};
static constexpr float INV_PI = {0.31830988618f};

float ftm::radians(float degrees)
{
    return degrees * PI / 180.0f;
}

float ftm::degrees(float radians)
{
    return radians * 180.0f * INV_PI;
}
