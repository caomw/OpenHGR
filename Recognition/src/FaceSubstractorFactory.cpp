#include "FaceSubstractorFactory.h"

#include <AbstractFaceSubstractor.h>
#include <HaarFaceSubstractor.h>
#include <LBPFaceSubstractor.h>

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
        case LBP_FACE_SUBSTRACTOR:
                 return new LBPFaceSubstractor();
        case HAAR_FACE_SUBSTRACTOR:
        default: return new HaarFaceSubstractor();
    }
}
