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

AbstractGestureRecognizer* GestureRecognizerFactory::createInstance ( GestureRecognizerType type, vector<string> trainingFolders )
{
    switch ( type )
    {
        case SIFT_Recognizer:
        default: return new SIFTGestureRecognizer (SURF_Detector,SURF_Extractor,FlannBased,SVM_Model,2500,trainingFolders);
    }
}
