#include "AbstractFilter.h"

void AbstractFilter::setDebug(char debug)
{
    this->debug = debug;
}

char AbstractFilter::isDebug()
{
    return this->debug;
}
