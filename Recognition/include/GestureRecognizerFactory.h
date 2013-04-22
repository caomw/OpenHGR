#ifndef GESTURERECOGNIZERFACTORY_H
#define GESTURERECOGNIZERFACTORY_H

#include <AbstractGestureRecognizer.h>
#include <vector>

enum GestureRecognizerType { SIFT_Recognizer };

class GestureRecognizerFactory
{
    public:
        GestureRecognizerFactory();
        ~GestureRecognizerFactory();
        AbstractGestureRecognizer* createInstance ( GestureRecognizerType type, std::vector<std::string> trainingFolders );
    protected:
    private:
};

#endif // GESTURERECOGNIZERFACTORY_H
