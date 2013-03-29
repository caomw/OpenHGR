#ifndef ABSTRACTFILTER_H
#define ABSTRACTFILTER_H


class AbstractFilter
{
    public:
        void setDebug (char debug);
    protected:
        char isDebug ();
    private:
        char debug = 0;
};

#endif // ABSTRACTFILTER_H
