#ifndef ANIMTIMER_H
#define ANIMTIMER_H

#include <ctime>

class AnimTimer
{
private:
    double timeLastStop;
    double clockLastStart;

    double getClock();

public:
    AnimTimer();

    void start();
    void stop();
    double curTime();
};

#endif // ANIMTIMER_H
