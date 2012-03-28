#ifndef VBOPLANE_H
#define VBOPLANE_H

#include "drawable.h"

class VBOPlane : public Drawable
{
private:
    unsigned int vaoHandle;
    int faces;

public:
    VBOPlane(float, float, int, int);

    void render() const;
};

#endif // VBOPLANE_H
