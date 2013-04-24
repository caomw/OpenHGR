#include "FaceSubstractorFactory.h"

#include <AbstractFaceSubstractor.h>
#include <HaarFaceSubstractor.h>

FaceSubstractorFactory::FaceSubstractorFactory()
{
    //ctor
}

FaceSubstractorFactory::~FaceSubstractorFactory()
{
    //dtor
}

AbstractFaceSubstractor* FaceSubstractorFactory::createInstance ( FaceSubstractorType type )
{
    switch ( type )
    {
        case HAAR_FACE_SUBSTRACTOR:
        default: return new HaarFaceSubstractor();
    }
}
