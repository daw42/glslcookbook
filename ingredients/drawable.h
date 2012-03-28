#ifndef DRAWABLE_H
#define DRAWABLE_H

class Drawable
{
public:
    Drawable();

    virtual void render() const = 0;
};

#endif // DRAWABLE_H
