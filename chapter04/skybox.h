#ifndef SKYBOX_H
#define SKYBOX_H

#include "drawable.h"

class SkyBox : public Drawable
{
private:
    unsigned int vaoHandle;

public:
    SkyBox();

    void render() const;
};


#endif // SKYBOX_H
