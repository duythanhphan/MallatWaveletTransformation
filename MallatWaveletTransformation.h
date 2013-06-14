#ifndef mallat_wavelet_transformation_h__

#define mallat_wavelet_transformation_h__

#include <cstddef>

#include "DiscreteFunction.h"

class MallatWaveletTransformation
{

private:
    DiscreteFunction* originalFunction; 
    DiscreteFunction* cachedTransformedFunction;

public:
    MallatWaveletTransformation(DiscreteFunction* function = NULL);

    DiscreteFunction* getOriginalFunction();

    DiscreteFunction* getTransformedFunction();

    void setOriginalFunction(DiscreteFunction* function);

    DiscreteFunction* getFunctions();

};

#endif
