//
//
//
//
#ifndef INITED_IFS
#define INITED_IFS
#include "ifs.h"
class InitedIfs 
{
    public:
        InitedIfs():ifs_(new Ifs())
        {
            ifs_->init();
        }
        Ifs* ifs()
        {
            return ifs_;
        }
    private:
        Ifs* ifs_;
};
#endif
