#pragma once
#include <iostream>

// POST: generates a random float in the range [-1.0, 1.0]
float generateRandomNormal()
{
    float random = (float)rand() / (float)RAND_MAX;
    //random *= 10;
    float sign = (float)rand() / (float)RAND_MAX;

    if (sign > 0.5)
    {
        random *= -1;
    }

    return random;
}