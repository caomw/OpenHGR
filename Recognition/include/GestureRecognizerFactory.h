#ifndef GESTURERECOGNIZERFACTORY_H
#define GESTURERECOGNIZERFACTORY_H

#include <AbstractGestureRecognizer.h>

enum GestureRecognizerType { SIFT };

class GestureRecognizerFactory
{
    public:
        GestureRecognizerFactory();
        ~GestureRecognizerFactory();
        AbstractGestureRecognizer* createInstance ( GestureRecognizerType type );
    protected:
    private:
};

#endif // GESTURERECOGNIZERFACTORY_H
