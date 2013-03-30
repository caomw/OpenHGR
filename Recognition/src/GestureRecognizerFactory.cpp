#include "GestureRecognizerFactory.h"

#include <AbstractGestureRecognizer.h>
#include <SIFTGestureRecognizer.h>

GestureRecognizerFactory::GestureRecognizerFactory()
{
    //ctor
}

GestureRecognizerFactory::~GestureRecognizerFactory()
{
    //dtor
}

AbstractGestureRecognizer* GestureRecognizerFactory::createInstance ( GestureRecognizerType type )
{
    switch ( type )
    {
        case SIFT:
        default: return new SIFTGestureRecognizer ();
    }
}
