#ifndef NOISETEX_H
#define NOISETEX_H

#include "cookbookogl.h"

namespace NoiseTex
{
    int generate2DTex(bool seamless = false, float baseFreq = 4.0f, int w = 128, int h = 128);
};

#endif // NOISETEX_H
