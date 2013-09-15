#ifndef NOISETEX_H
#define NOISETEX_H

#include "cookbookogl.h"

namespace NoiseTex
{
  int storeTex( GLubyte * data, int w, int h );

  int generate2DTex(float baseFreq = 4.0f, float persistence = 0.5f, int w = 128, int h = 128, bool periodic = false);

  int generatePeriodic2DTex(float baseFreq = 4.0f, float persist = 0.5f, int w = 128, int h = 128);
};

#endif // NOISETEX_H
