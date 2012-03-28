#include "animtimer.h"

AnimTimer::AnimTimer() : timeLastStop(0.0), clockLastStart(0.0)
{

}

void AnimTimer::start()
{
    clockLastStart = getClock();
}

void AnimTimer::stop()
{
    timeLastStop = curTime();
}

double AnimTimer::curTime()
{
    double t = getClock();
    return timeLastStop + (t - clockLastStart);
}

double AnimTimer::getClock()
{
    static double clockFac = 1.0 / CLOCKS_PER_SEC;
    return clock() * clockFac;
}
