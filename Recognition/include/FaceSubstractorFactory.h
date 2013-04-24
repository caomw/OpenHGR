#ifndef FACESUBSTRACTORFACTORY_H
#define FACESUBSTRACTORFACTORY_H

#include <AbstractFaceSubstractor.h>

enum FaceSubstractorType { HAAR_FACE_SUBSTRACTOR };

class FaceSubstractorFactory
{
    public:
        FaceSubstractorFactory();
        virtual ~FaceSubstractorFactory();
        AbstractFaceSubstractor* createInstance ( FaceSubstractorType type );
    protected:
    private:
};

#endif // FACESUBSTRACTORFACTORY_H
